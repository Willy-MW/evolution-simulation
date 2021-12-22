// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"

#include "EdibleInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Grass.generated.h"

UCLASS()
class GAS_CPP_API AGrass : public AActor, public IEdibleInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrass();

protected:
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	float HungerSatisfaction;

public:
	virtual float GetHungerSatisfactionValue() override;

	virtual void GetBitten(float BiteStrength) override;
};
