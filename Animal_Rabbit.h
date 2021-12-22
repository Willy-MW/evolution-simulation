// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "EdibleInterface.h"

#include "Animal_Rabbit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRabbitDeathDelegate, FRabbitInfo, RabbitInfo);

UCLASS()
class GAS_CPP_API AAnimal_Rabbit : public AAnimal, public IEdibleInterface
{
	GENERATED_BODY()
	AAnimal_Rabbit();
	
protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY()
	float HungerSatisfaction;

	UPROPERTY()
	int Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void GetBitten(float BiteStrength) override;

	virtual float GetHungerSatisfactionValue() override;

	UPROPERTY()
	FRabbitDeathDelegate OnRabbitDeath;

	UFUNCTION()
	void SpendEnergy() const;

	UFUNCTION()
	void UpdateColor() const;

	UFUNCTION()
	FRabbitInfo GetRabbitInfo() const;

	UFUNCTION()
	void Die();
	
	UFUNCTION()
	void OnFullyHungry();

	UFUNCTION()
	void SetId(int NewId);
};
