// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_BaseAnimal.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API UBTService_BaseAnimal : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_BaseAnimal();
	
protected:
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY()
	class AAnimal* Animal;

	UPROPERTY()
	UBlackboardComponent* MyBlackboard;
};
