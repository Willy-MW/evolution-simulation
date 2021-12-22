// © 2020 Marcin Witek 


#include "AIC_Animal.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIC_Animal::AAIC_Animal()
{
}

void AAIC_Animal::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if(!BehaviorTree)
	{
		return;
	}
	
	RunBehaviorTree(BehaviorTree);
}