// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HungerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFullyHungryDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHungryChangeDelegate, bool, bHungry);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_CPP_API UHungerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHungerComponent();

	UPROPERTY()
	FFullyHungryDelegate OnFullyHungry;

	UPROPERTY()
	FHungryChangeDelegate OnHungryChange;

protected:
	UPROPERTY()
	float Hunger;

	UPROPERTY()
	float MaxHunger;

	UPROPERTY()
	float BiteStrength;

	UPROPERTY()
	bool bHungry;

public:
	UPROPERTY(EditDefaultsOnly)
	float HungerThreshold;

	UPROPERTY(EditDefaultsOnly)
	bool bCanStarve;
	
	UFUNCTION()
	void IncreaseHunger(float Amount);

	UFUNCTION()
	void DecreaseHunger(float Amount);

	UFUNCTION()
	void SetMaxHunger(float NewMaxHunger);

	UFUNCTION()
	float GetHungerPercentage() const;

	UFUNCTION()
	void SetIsHungry(bool bIsHungry);

	UFUNCTION()
	bool IsHungry() const;

	UFUNCTION()
	void EvaluateHunger();

	UFUNCTION()
	void Eat(AActor* ActorToEat);
};
