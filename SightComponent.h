// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "SightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_CPP_API USightComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

	UPROPERTY()
	USphereComponent* SightSphere;

	UPROPERTY()
	UStaticMeshComponent* SightMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly)
	float SightRadius;

	UPROPERTY()
	TArray<AActor*> FoodInSight;

	UPROPERTY()
	TArray<AActor*> EnemiesInSight;

public:	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FoodClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> EnemyClass;

	UFUNCTION(BlueprintCallable) //TODO: Delete property
	void SetSightRadius(float NewSightRadius);

	UFUNCTION()
	float GetSightRadius() const;

	UFUNCTION()
	void InitializeSightSphere() const;

	UFUNCTION()
	void SetSightSphereVisibility(bool NewVisibility) const;

	UFUNCTION()
	void AddFoodInSight(AActor* Food);

	UFUNCTION()
	void RemoveFoodInSight(AActor* Food);
	
	UFUNCTION()
	void AddEnemyInSight(AActor* Enemy);

	UFUNCTION()
	void RemoveEnemyInSight(AActor* Enemy);

	UFUNCTION()
	void OnActorInSight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnActorOutOfSight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool IsSeeingFood() const;

	UFUNCTION()
	bool IsSeeingEnemy() const;

	UFUNCTION()
	AActor* GetClosestSeenActor(TArray<AActor*> ActorsInSight) const;

	UFUNCTION()
	AActor* GetClosestFood() const;

	UFUNCTION()
	AActor* GetClosestEnemy() const;
};

