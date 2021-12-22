// © 2020 Marcin Witek 


#include "BTTask_GetFoodLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetFoodLocation::UBTTask_GetFoodLocation()
{
	NodeName = "Get Food Location";

	FoodToEatKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetFoodLocation, FoodToEatKey), AActor::StaticClass());
	FoodLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetFoodLocation, FoodLocationKey));
}

void UBTTask_GetFoodLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		FoodToEatKey.ResolveSelectedKey(*BBAsset);
		FoodLocationKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GetFoodLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	
	if(!MyBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task GetFoodLocation failed on getting blackboard"))
		return EBTNodeResult::Failed;
	}

	UObject* FoodObject = MyBlackboard->GetValueAsObject(FoodToEatKey.SelectedKeyName);
	AActor* Food = Cast<AActor>(FoodObject);

	if(!Food)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task GetFoodLocation failed on invalid Food"))
		return EBTNodeResult::Failed;
	}
	
	MyBlackboard->SetValueAsVector(FoodLocationKey.SelectedKeyName, Food->GetActorLocation());
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_GetFoodLocation::GetStaticDescription() const
{
	FString FirstKeyDesc("invalid"), SecondKeyDesc("invalid");
	if (FoodToEatKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		FirstKeyDesc = FoodToEatKey.SelectedKeyName.ToString();
	}

	if (FoodLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		SecondKeyDesc = FoodLocationKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Food: %s\nLocation: %s"), *FirstKeyDesc, *SecondKeyDesc);
}
