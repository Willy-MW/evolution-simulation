// © 2020 Marcin Witek 


#include "BTTask_SmoothMoveTo.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_SmoothMoveTo::UBTTask_SmoothMoveTo()
{
	NodeName = "Smooth Move To";

	MoveToKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SmoothMoveTo, MoveToKey));
}

void UBTTask_SmoothMoveTo::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		MoveToKey.ResolveSelectedKey(*BBAsset);
	}
}

EBTNodeResult::Type UBTTask_SmoothMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(!MyController || !MyBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task SmoothMoveTo failed on getting Controller or Blackboard"))
		return EBTNodeResult::Failed;
	}

	const FVector Location = MyBlackboard->GetValueAsVector(MoveToKey.SelectedKeyName);
	if(MyController->MoveToLocation(Location) == EPathFollowingRequestResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task SmoothMoveTo failed on MoveToLocation"))
	}
	
	return EBTNodeResult::Succeeded; 
}

FString UBTTask_SmoothMoveTo::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (MoveToKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = MoveToKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("Move To: %s"), *KeyDesc);
}
