// © 2020 Marcin Witek 


#include "BTService_BaseAnimal.h"
#include "AIController.h"
#include "Animal.h"

UBTService_BaseAnimal::UBTService_BaseAnimal() : Super()
{
	NodeName = "Base Animal";

	bCreateNodeInstance = true;
	bNotifyBecomeRelevant = true;
	//bNotifyCeaseRelevant = true;
}

void UBTService_BaseAnimal::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	MyBlackboard = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	
	if(!MyController || !MyBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller | Blackboard invalid"));
		return;
	}

	Animal = Cast<AAnimal>(MyController->GetPawn());
}
