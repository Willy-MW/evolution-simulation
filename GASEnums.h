// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "GASEnums.generated.h"

UENUM(BlueprintType)
enum ETraitType
{
	Sight		UMETA(DisplayName = "Sight"),
	Speed		UMETA(DisplayName = "Speed"),
	Size		UMETA(DisplayName = "Size"),
};