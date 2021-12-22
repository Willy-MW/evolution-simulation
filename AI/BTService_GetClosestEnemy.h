// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "BTService_BaseAnimal.h"

#include "BTService_GetClosestEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API UBTService_GetClosestEnemy : public UBTService_BaseAnimal
{
	GENERATED_BODY()

public:
	UBTService_GetClosestEnemy();
	
protected:

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector ClosestEnemyKey;
};
