// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EatFood.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API UBTTask_EatFood : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EatFood();
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FoodToEatKey;
};
