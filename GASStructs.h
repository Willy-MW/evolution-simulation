// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"
#include "GASStructs.generated.h"

USTRUCT(BlueprintType)
struct FAnimalTraits
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Sight;

	UPROPERTY(BlueprintReadWrite)
	float Speed;

	UPROPERTY(BlueprintReadWrite)
	float Size;

	FAnimalTraits()
	{
		Sight = 1.f;
		Speed = 1.f;
		Size = 1.f;
	}

	FAnimalTraits(float NewSight, float NewSpeed, float NewSize)
	{
		Sight = NewSight;
		Speed = NewSpeed;
		Size = NewSize;
	}
};

USTRUCT(BlueprintType)
struct FTraitsInheritance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool Sight;

	UPROPERTY(BlueprintReadWrite)
	bool Speed;

	UPROPERTY(BlueprintReadWrite)
	bool Size;

	FTraitsInheritance()
	{
		Sight = false;
		Speed = false;
		Size = false;
	}

	FTraitsInheritance Randomize()
	{
		Sight = FMath::RandBool();
		Speed = FMath::RandBool();
		Size = FMath::RandBool();

		return *this;
	}

	FTraitsInheritance Inverse()
	{
		Sight = !Sight;
		Speed = !Speed;
		Size = !Size;

		return *this;
	}
};

USTRUCT(BlueprintType)
struct FUnitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float HungerPercent;

	UPROPERTY(BlueprintReadWrite)
	float TimeAlive;

	UPROPERTY(BlueprintReadWrite)
	FAnimalTraits UnitTraits;

	FUnitInfo()
	{
		HungerPercent = 0.f;
		TimeAlive = 0.f;
		UnitTraits = FAnimalTraits();
	}

	explicit FUnitInfo(const FAnimalTraits Traits)
	{
		HungerPercent = 0.f;
		TimeAlive = 0.f;
		UnitTraits = Traits;
	}
};

USTRUCT(BlueprintType)
struct FRabbitInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int Id;

	UPROPERTY(BlueprintReadWrite)
	FUnitInfo UnitInfo;
};