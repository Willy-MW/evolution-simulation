// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetFoodLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API UBTTask_GetFoodLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetFoodLocation();
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FoodToEatKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector FoodLocationKey;
};
