// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "GASStructs.h"
#include "HungerComponent.h"
#include "SightComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "Animal.generated.h"

UCLASS()
class GAS_CPP_API AAnimal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAnimal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* Collision;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* AnimalMesh;

	UPROPERTY()
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	float BaseSight;

	UPROPERTY(EditDefaultsOnly)
	float BaseSpeed;

	UPROPERTY(EditDefaultsOnly)
	float BaseSize;

	UPROPERTY(EditDefaultsOnly)
	FAnimalTraits Traits;

public:
	UPROPERTY(EditDefaultsOnly)
	USightComponent* SightComponent;

	UPROPERTY(EditDefaultsOnly)
	UHungerComponent* HungerComponent;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void SetTraits(FAnimalTraits NewTraits);

	UFUNCTION()
	FAnimalTraits GetTraits() const;

	UFUNCTION()
	float GetSight() const;

	UFUNCTION()
	float GetSpeed() const;

	UFUNCTION()
	float GetSize() const;

	UFUNCTION()
	void UpdateAnimalAttributes() const;

	UFUNCTION()
	bool IsMoving() const;

	UFUNCTION()
	bool IsFoodInRange() const;
};