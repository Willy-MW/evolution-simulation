// © 2020 Marcin Witek 


#include "BTTask_GetRandomLocation.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = "Get Random Location";

	RandomLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomLocation, RandomLocationKey));
	SightRadiusKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_GetRandomLocation, SightRadiusKey));
}

void UBTTask_GetRandomLocation::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		RandomLocationKey.ResolveSelectedKey(*BBAsset);
		SightRadiusKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(!MyController || !MyBlackboard)
	{
		return EBTNodeResult::Failed;
	}
	
	const FVector Origin = MyController->GetPawn()->GetActorLocation();
	const float Radius = MyBlackboard->GetValueAsFloat(SightRadiusKey.SelectedKeyName);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation Nav;

	if(!NavSys->GetRandomPointInNavigableRadius(Origin, Radius, Nav))
	{
		return EBTNodeResult::Failed;
	}

	MyBlackboard->SetValueAsVector(RandomLocationKey.SelectedKeyName, Nav.Location);
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_GetRandomLocation::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (RandomLocationKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = RandomLocationKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Chosen Location: %s"), *KeyDesc);
}
