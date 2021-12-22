// © 2020 Marcin Witek 


#include "BTService_GetClosestFood.h"

#include "Animal.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetClosestFood::UBTService_GetClosestFood() : Super()
{
	NodeName = "Get Closest Food";

	ClosestFoodKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetClosestFood, ClosestFoodKey), AActor::StaticClass());
	IsFoodInRangeKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetClosestFood, IsFoodInRangeKey));
}

void UBTService_GetClosestFood::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		ClosestFoodKey.ResolveSelectedKey(*BBAsset);
		IsFoodInRangeKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_GetClosestFood::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(!IsValid(Animal))
	{
		return;
	}

	MyBlackboard->SetValueAsObject(ClosestFoodKey.SelectedKeyName, Animal->SightComponent->GetClosestFood());
	MyBlackboard->SetValueAsBool(IsFoodInRangeKey.SelectedKeyName, Animal->IsFoodInRange());
}
