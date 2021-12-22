// © 2020 Marcin Witek 


#include "BTService_GetClosestEnemy.h"
#include "Animal.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetClosestEnemy::UBTService_GetClosestEnemy() : Super()
{
	NodeName = "Get Closest Enemy";

	ClosestEnemyKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetClosestEnemy, ClosestEnemyKey), AActor::StaticClass());
}

void UBTService_GetClosestEnemy::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		ClosestEnemyKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_GetClosestEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(!IsValid(Animal))
	{
		return;
	}

	MyBlackboard->SetValueAsObject(ClosestEnemyKey.SelectedKeyName, Animal->SightComponent->GetClosestEnemy());
}
