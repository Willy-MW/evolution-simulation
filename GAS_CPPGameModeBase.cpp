// Copyright Epic Games, Inc. All Rights Reserved.


#include "GAS_CPPGameModeBase.h"

#include "PlayerPawn.h"
#include "PopulationManager.h"
#include "Kismet/GameplayStatics.h"
#include "NavMesh/NavMeshBoundsVolume.h"

AGAS_CPPGameModeBase::AGAS_CPPGameModeBase()
{
	DefaultPawnClass = APlayerPawn::StaticClass();
}

void AGAS_CPPGameModeBase::BeginPlay()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetShowMouseCursor(true);

	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	PlayerController->SetInputMode(Mode);

	UWorld* World = GetWorld();
	if(!World || !PopulationManagerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("World | PopulationManagerClass invalid"));
		return;
	}

	TArray<AActor*> ActorsFound;
	UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), ActorsFound);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(PopulationManagerClass, SpawnParameters);
	PM = Cast<APopulationManager>(SpawnedActor);

	if(!PM)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to PopulationManager failed"));
		return;
	}
	
	PM->NavMesh = Cast<AStaticMeshActor>(ActorsFound[0]);
}
