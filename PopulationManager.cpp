// © 2020 Marcin Witek 


#include "PopulationManager.h"

#include "GeneratedCodeHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Matinee/InterpFilter_Classes.h"

// Sets default values
APopulationManager::APopulationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PopulationNumber = 12;
	MutationChance = 0.1f;
	MutationRange = 0.1f;
	GenerationNumberGoal = 100;
	SingleGenerationTime = 30.f;
	bIntroduceFood = false;
	bIntroducePredator = false;
	FoodAmount = 0;
	PredatorsNumber = 0;
}

// Called when the game starts or when spawned
void APopulationManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void APopulationManager::OnConstruction(const FTransform& Transform)
{
	SetPopulationNumber(PopulationNumber);
}

// Called every frame
void APopulationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bSimulating && CurrentPopulation <= 0)
	{
		EvaluateGeneration();
	}	
}

void APopulationManager::InitializePopulation()
{
	PopulationInfos.Empty();

	for(int i = 0; i < PopulationNumber; i++)
	{
		FAnimalTraits NewTraits;
		Mutate(NewTraits);

		FUnitInfo NewInfo;
		NewInfo.UnitTraits = NewTraits;

		PopulationInfos.Add(NewInfo);
	}
}

void APopulationManager::ReinitializePopulation()
{
	PopulationInfos.SetNum(PopulationNumber/2);

	for (int i = 0; i < PopulationNumber / 2; i += 2) {
		CreateNewUnit(PopulationInfos[i], PopulationInfos[i+1]);
	}
}

void APopulationManager::Populate()
{
	SpawnUnits(PopulationUnitClass, PopulationNumber, TArrayCaster<AAnimal_Rabbit*>(PopulationUnits).Get<AActor*>());

	int i = 0;
	for(AAnimal_Rabbit* Rabbit : PopulationUnits)
	{
		Rabbit->SetId(i);
		Rabbit->SetTraits(PopulationInfos[i].UnitTraits);
		Rabbit->OnRabbitDeath.AddDynamic(this, &APopulationManager::OnUnitDeath);
		i++;
	}

	CurrentPopulation = PopulationUnits.Num();

	if(bIntroduceFood)
	{
		SpawnUnits(FoodClass, FoodAmount, TArrayCaster<AGrass*>(Food).Get<AActor*>());
	}

	if(bIntroducePredator)
	{
		SpawnUnits(PredatorClass, PredatorsNumber, TArrayCaster<AAnimal*>(Predators).Get<AActor*>());
	}
}

void APopulationManager::Mutate(FAnimalTraits& Traits) const
{
	if(MutationChance <= 0.f)
	{
		return;
	}

	Traits.Sight += GetRandomTraitChange();
	Traits.Speed += GetRandomTraitChange();
	Traits.Size += GetRandomTraitChange();
}

void APopulationManager::CreateNewUnit(const FUnitInfo FirstParentInfo, const FUnitInfo SecondParentInfo)
{ 
	FAnimalTraits FirstChildTraits, SecondChildTraits;
	FTraitsInheritance Inheritance;
	
	FirstChildTraits = CrossoverTraits(FirstParentInfo.UnitTraits, SecondParentInfo.UnitTraits, Inheritance.Randomize());
	SecondChildTraits = CrossoverTraits(FirstParentInfo.UnitTraits, SecondParentInfo.UnitTraits, Inheritance.Inverse());

	Mutate(FirstChildTraits);
	Mutate(SecondChildTraits);

	PopulationInfos.Add(FUnitInfo(FirstChildTraits));
	PopulationInfos.Add(FUnitInfo(SecondChildTraits));
}

FAnimalTraits APopulationManager::CrossoverTraits(FAnimalTraits FirstParentTraits, FAnimalTraits SecondParentTraits,
                                                  FTraitsInheritance WillInheritFromFirstParent)
{
	return FAnimalTraits(WillInheritFromFirstParent.Sight ? FirstParentTraits.Sight : SecondParentTraits.Sight,
	              WillInheritFromFirstParent.Speed ? FirstParentTraits.Speed : SecondParentTraits.Speed,
	              WillInheritFromFirstParent.Size ? FirstParentTraits.Size : SecondParentTraits.Size
	);
}

void APopulationManager::SaveSurvivorsInfo()
{
	for(AAnimal_Rabbit* Rabbit : PopulationUnits)
	{
		if(IsValid(Rabbit))
		{
			SaveUnitInfo(Rabbit->GetRabbitInfo());
		}
	}
}

void APopulationManager::SaveUnitInfo(FRabbitInfo RabbitInfo)
{
	PopulationInfos[RabbitInfo.Id] = RabbitInfo.UnitInfo;
}

void APopulationManager::DestroyAllUnits()
{
	for(AAnimal_Rabbit* Rabbit : PopulationUnits)
	{
		if(IsValid(Rabbit))
		{
			Rabbit->Destroy();
		}
	}

	for(AAnimal* Predator : Predators)
	{
		if(IsValid(Predator))
		{
			Predator->Destroy();
		}
	}

	for(AActor* Piece : Food)
	{
		if(IsValid(Piece))
		{
			Piece->Destroy();
		}
	}
}

float APopulationManager::CalculateFitness(const FUnitInfo UnitInfo) const
{
	return (1 - UnitInfo.HungerPercent) + UnitInfo.TimeAlive / SingleGenerationTime;
}

void APopulationManager::SpawnUnits(TSubclassOf<AActor> UnitClass, int Number, TArray<AActor*>& UnitArray)
{
	if(Number <= 0 || !UnitClass || !NavMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnUnits: Number <= 0 | Null UnitClass | Null NavMesh"));
		return;
	}

	UnitArray.Empty();

	FVector Origin, Extent;
	NavMesh->GetActorBounds(false, Origin, Extent);
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	for(int i = 0; i < Number; i++)
	{		
		FRotator SpawnRotation(0.f);
		FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
		SpawnLocation.Z = 5.0f;
		
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(UnitClass, SpawnLocation, SpawnRotation, SpawnParameters); //.Get()

		UnitArray.Add(SpawnedActor);
	}

	UE_LOG(LogTemp, Warning, TEXT("Successfully spawned %d actors"), Number);
}

void APopulationManager::SortByTheFittest()
{
	int ArrayLength = PopulationInfos.Num();
	float FirstElementFitness, SecondElementFitness;
	bool Swapped;

	for(int i=0; i<ArrayLength-1; i++)
	{
		Swapped = false;
		for(int j=0; j<ArrayLength-i-1; j++)
		{
			FirstElementFitness = CalculateFitness(PopulationInfos[j]);
			SecondElementFitness = CalculateFitness(PopulationInfos[j+1]);

			if(FirstElementFitness > SecondElementFitness)
			{
				PopulationInfos.Swap(j, j+1);
				Swapped = true;
			}
		}

		if(!Swapped)
		{
			break;
		}
	}
}

void APopulationManager::OnUnitDeath(const FRabbitInfo RabbitInfo)
{
	SaveUnitInfo(RabbitInfo);
	CurrentPopulation--;
}

void APopulationManager::SetPopulationNumber(const int Number)
{
	PopulationNumber = (Number + 3) & ~0x03;
}

TMap<float, int> APopulationManager::GetMultiplierMapForType(const ETraitType Type)
{
	TMap<float, int> Map;
	TArray<FUnitInfo> Infos = GetInfoSortedByTrait(Type);

	switch (Type)
	{
	case ETraitType::Sight:		
		Map.FindOrAdd(Infos[0].UnitTraits.Sight - MutationRange, 0);
		
		for(FUnitInfo Info : Infos)
		{
			Map.FindOrAdd(Info.UnitTraits.Sight)++;
		}

		Map.FindOrAdd(Infos[Infos.Num()-1].UnitTraits.Sight + MutationRange, 0);
		break;
	case ETraitType::Size:
		Map.FindOrAdd(Infos[0].UnitTraits.Size - MutationRange, 0);
		
		for (FUnitInfo Info : Infos)
		{
			Map.FindOrAdd(Info.UnitTraits.Size)++;
		}

		Map.FindOrAdd(Infos[Infos.Num()-1].UnitTraits.Size + MutationRange, 0);
		break;
	case ETraitType::Speed:
		Map.FindOrAdd(Infos[0].UnitTraits.Speed - MutationRange, 0);
		
		for (FUnitInfo Info : Infos)
		{
			Map.FindOrAdd(Info.UnitTraits.Speed)++;
		}

		Map.FindOrAdd(Infos[Infos.Num()-1].UnitTraits.Speed + MutationRange, 0);
		break;
	}
	
	return Map;
}

bool APopulationManager::IsMultiplierGreater(FAnimalTraits FirstInfo, FAnimalTraits SecondInfo, ETraitType Type)
{
	switch (Type)
	{
		case ETraitType::Sight:
			return FirstInfo.Sight > SecondInfo.Sight;
			break;
		case ETraitType::Size:
			return FirstInfo.Size > SecondInfo.Size;
			break;
		case ETraitType::Speed:
			return FirstInfo.Speed > SecondInfo.Speed;
			break;
		default:
			return false;
	}
}

TArray<FUnitInfo> APopulationManager::GetInfoSortedByTrait(ETraitType Type)
{
	TArray<FUnitInfo> TempInfos = PopulationInfos;
	int ArrayLength = TempInfos.Num();
	FAnimalTraits FirstElementInfo, SecondElementInfo;
	bool Swapped;

	for(int i=0; i<ArrayLength-1; i++)
	{
		Swapped = false;
		for(int j=0; j<ArrayLength-i-1; j++)
		{
			FirstElementInfo = TempInfos[j].UnitTraits;
			SecondElementInfo = TempInfos[j+1].UnitTraits;

			if(IsMultiplierGreater(FirstElementInfo, SecondElementInfo, Type))
			{
				TempInfos.Swap(j, j+1);
				Swapped = true;
			}
		}

		if(!Swapped)
		{
			break;
		}
	}

	return TempInfos;
}

float APopulationManager::GetRandomTraitChange() const
{
	const float Change = MutationRange * (FMath::FRand() <= 0.5f ? -1.f : 1.f);

	return FMath::FRand() <= MutationChance ? Change : 0.f;
}

void APopulationManager::StartSimulation()
{
	bSimulationStarted = true;
	InitializePopulation();
	CurrentGeneration = 0;
	bLastGeneration = false;

	StartNewGeneration();
}

void APopulationManager::StartNewGeneration()
{
	OnInitializePopulation.Broadcast();
	Populate();

	if(CurrentPopulation <= 0)
	{
		DestroyAllUnits();
		bSimulationStarted = false;

		UE_LOG(LogTemp, Warning, TEXT("No population units spawned"));
		return;
	}

	GetWorldTimerManager().SetTimer(MainLoopTimer, this, &APopulationManager::EvaluateGeneration, SingleGenerationTime, false);
	bSimulating = true;

	UE_LOG(LogTemp, Warning, TEXT("Generation %d started"), CurrentGeneration);
}

void APopulationManager::EvaluateGeneration()
{
	GetWorldTimerManager().ClearTimer(MainLoopTimer);
	bSimulating = false;
	SaveSurvivorsInfo();
	DestroyAllUnits();
	SortByTheFittest();

	if(bLastGeneration)
	{
		bSimulationStarted = false;
		return;
	}

	ReinitializePopulation();
	CurrentGeneration++;
	bLastGeneration = CurrentGeneration >= GenerationNumberGoal;
	StartNewGeneration();
}

void APopulationManager::EndSimulation()
{
	bLastGeneration = true;
	EvaluateGeneration();
}

