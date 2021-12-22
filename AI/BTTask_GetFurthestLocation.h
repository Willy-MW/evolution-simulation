// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetFurthestLocation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API UBTTask_GetFurthestLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetFurthestLocation();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector EnemyObjectKey;
	
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector AnimalSightRadiusKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector ChosenLocationKey;

private:
	UPROPERTY()
	FVector EnemyLocation;

	UPROPERTY()
	float SightRadius;
	
	UFUNCTION()
	TArray<FVector> GetValidLocationsAroundPawn(const int Number, APawn* Pawn);

	UFUNCTION()
	FVector GetFurthestLocation(const TArray<FVector>& Locations);
};
