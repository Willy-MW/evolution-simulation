// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "PopulationManager.h"
#include "GameFramework/GameModeBase.h"
#include "GAS_CPPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAS_CPP_API AGAS_CPPGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGAS_CPPGameModeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APopulationManager> PopulationManagerClass;

	UPROPERTY(BlueprintReadOnly)
	APopulationManager* PM;
	
protected:
	virtual void BeginPlay() override;
};
