// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"

#include "Animal.h"
#include "Animal_Rabbit.h"
#include "GASEnums.h"
#include "Grass.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"

#include "PopulationManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInitializePopulationDelegate);

UCLASS()
class GAS_CPP_API APopulationManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APopulationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	FTimerHandle MainLoopTimer;

public:
	UPROPERTY(BlueprintAssignable)
	FInitializePopulationDelegate OnInitializePopulation;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Population individual class
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAnimal> PopulationUnitClass;

	//Number of evaluated critters
	UPROPERTY(BlueprintReadOnly)
	int PopulationNumber;

	//Chance for single trait to mutate on replication
	UPROPERTY(BlueprintReadWrite)
	float MutationChance;

	//Determines the range of mutation that occurs from -value to +value
	UPROPERTY(BlueprintReadWrite)
	float MutationRange;

	//Time limit for one generation to live in seconds
	UPROPERTY(BlueprintReadWrite)
	float SingleGenerationTime;

	UPROPERTY(BlueprintReadWrite)
	int GenerationNumberGoal;

	UPROPERTY(BlueprintReadOnly)
	int CurrentGeneration;

	UPROPERTY(BlueprintReadOnly)
	int CurrentPopulation;

	UPROPERTY(BlueprintReadOnly)
	bool bSimulationStarted;

	UPROPERTY(BlueprintReadOnly)
	bool bSimulating;

	UPROPERTY(BlueprintReadOnly)
	bool bLastGeneration;

	//Controls simulation having food as evolution environmental variable
	UPROPERTY(BlueprintReadWrite)
	bool bIntroduceFood;

	UPROPERTY(BlueprintReadWrite)
	int FoodAmount;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FoodClass;

	//Controls simulation having predator as evolution environmental variable
	UPROPERTY(BlueprintReadWrite)
	bool bIntroducePredator;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAnimal> PredatorClass;

	UPROPERTY(BlueprintReadWrite)
	int PredatorsNumber;

	UPROPERTY(BlueprintReadWrite)
	AStaticMeshActor* NavMesh;

	UPROPERTY()
	TArray<FUnitInfo> PopulationInfos;

	UPROPERTY()
	TArray<AAnimal_Rabbit*> PopulationUnits;

	UPROPERTY()
	TArray<AGrass*> Food;

	UPROPERTY()
	TArray<AAnimal*> Predators;

	UFUNCTION()
	void InitializePopulation();

	UFUNCTION()
	void ReinitializePopulation();

	UFUNCTION()
	void Populate();

	UFUNCTION()
	void Mutate(FAnimalTraits& Traits) const;

	UFUNCTION()
	void CreateNewUnit(FUnitInfo FirstParentInfo, FUnitInfo SecondParentInfo);

	UFUNCTION()
	FAnimalTraits CrossoverTraits(FAnimalTraits FirstParentTraits, FAnimalTraits SecondParentTraits,
								FTraitsInheritance WillInheritFromFirstParent);

	UFUNCTION()
	void SaveSurvivorsInfo();

	UFUNCTION()
	void SaveUnitInfo(FRabbitInfo RabbitInfo);

	UFUNCTION()
	void DestroyAllUnits();

	UFUNCTION()
	float CalculateFitness(const FUnitInfo UnitInfo) const;

	UFUNCTION()
	void SpawnUnits(TSubclassOf<AActor> UnitClass, int Number, TArray<AActor*>& UnitArray);

	UFUNCTION()
	void SortByTheFittest();

	UFUNCTION()
	void OnUnitDeath(FRabbitInfo RabbitInfo);

	UFUNCTION(BlueprintCallable)
	void SetPopulationNumber(int Number);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<float, int> GetMultiplierMapForType(ETraitType Type);

	UFUNCTION()
	bool IsMultiplierGreater(FAnimalTraits FirstInfo, FAnimalTraits SecondInfo, ETraitType Type);

	UFUNCTION()
	TArray<FUnitInfo> GetInfoSortedByTrait(ETraitType Type);

	UFUNCTION()
	float GetRandomTraitChange() const;

	UFUNCTION(BlueprintCallable)
	void StartSimulation();

	UFUNCTION()
	void StartNewGeneration();

	UFUNCTION()
	void EvaluateGeneration();

	UFUNCTION(BlueprintCallable)
	void EndSimulation();
};
