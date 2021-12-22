// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "AIC_Animal.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API AAIC_Animal : public AAIController
{
	GENERATED_BODY()

public:
	AAIC_Animal();

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
};
