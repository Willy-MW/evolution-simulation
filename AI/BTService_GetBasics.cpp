// © 2020 Marcin Witek 


#include "BTService_GetBasics.h"
#include "Animal.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_GetBasics::UBTService_GetBasics() : Super()
{
	NodeName = "Get Animal Basics";

	IsHungryKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetBasics, IsHungryKey));
	SeesFoodKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetBasics, SeesFoodKey));
	SeesEnemyKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetBasics, SeesEnemyKey));
	SightRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_GetBasics, SightRadiusKey));

}

void UBTService_GetBasics::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		IsHungryKey.ResolveSelectedKey(*BBAsset);
		SeesFoodKey.ResolveSelectedKey(*BBAsset);
		SeesEnemyKey.ResolveSelectedKey(*BBAsset);
		SightRadiusKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_GetBasics::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(!IsValid(Animal))
	{
		UE_LOG(LogTemp, Warning, TEXT("Animal NOT valid"));
		return;
	}
	
	MyBlackboard->SetValueAsBool(IsHungryKey.SelectedKeyName, Animal->HungerComponent->IsHungry());
	MyBlackboard->SetValueAsBool(SeesFoodKey.SelectedKeyName, Animal->SightComponent->IsSeeingFood());
	MyBlackboard->SetValueAsBool(SeesEnemyKey.SelectedKeyName, Animal->SightComponent->IsSeeingEnemy());
	MyBlackboard->SetValueAsFloat(SightRadiusKey.SelectedKeyName, Animal->SightComponent->GetSightRadius());
}
