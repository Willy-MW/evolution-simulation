// © 2020 Marcin Witek 


#include "BTTask_EatFood.h"

#include "AIController.h"
#include "Animal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_EatFood::UBTTask_EatFood()
{
	NodeName = "Eat Food";

	FoodToEatKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EatFood, FoodToEatKey), AActor::StaticClass());
}

void UBTTask_EatFood::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		FoodToEatKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_EatFood::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(!MyController || !MyBlackboard)
	{
		return EBTNodeResult::Failed;
	}

	AAnimal* Animal = Cast<AAnimal>(MyController->GetPawn());
	UObject* FoodObject = MyBlackboard->GetValueAsObject(FoodToEatKey.SelectedKeyName);
	AActor* Food = Cast<AActor>(FoodObject);
	
	if(!Animal || !Food)
	{
		return EBTNodeResult::Failed;
	}

	Animal->HungerComponent->Eat(Food);
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_EatFood::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (FoodToEatKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
	{
		KeyDesc = FoodToEatKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Eat: %s"), *KeyDesc);
}


