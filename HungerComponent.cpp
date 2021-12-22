// © 2020 Marcin Witek 

#include "HungerComponent.h"
#include "EdibleInterface.h"

// Sets default values for this component's properties
UHungerComponent::UHungerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Hunger = 50.f;
	MaxHunger = 100.f;
	BiteStrength = 50.f;
	bHungry = false;
	bCanStarve = true;
	HungerThreshold = 0.666;
}

void UHungerComponent::IncreaseHunger(const float Amount)
{
	if (!bCanStarve)
	{
		return;
	}

	Hunger = FMath::Clamp(Hunger + Amount, 0.f, MaxHunger);

	EvaluateHunger();
}

void UHungerComponent::DecreaseHunger(const float Amount)
{
	if (!bCanStarve)
	{
		return;
	}

	Hunger = FMath::Clamp(Hunger - Amount, 0.f, MaxHunger);

	EvaluateHunger();
}

void UHungerComponent::SetMaxHunger(const float NewMaxHunger)
{
	MaxHunger = NewMaxHunger;
	Hunger = MaxHunger / 2.f;

	EvaluateHunger();
}

float UHungerComponent::GetHungerPercentage() const
{
	return Hunger/MaxHunger;
}

void UHungerComponent::SetIsHungry(const bool bIsHungry)
{
	bHungry = bIsHungry;

	OnHungryChange.Broadcast(bHungry);
}

bool UHungerComponent::IsHungry() const
{
	return bHungry;
}

void UHungerComponent::EvaluateHunger()
{
	const bool CurrentlyHungry = Hunger > MaxHunger * HungerThreshold;

	if(bHungry != CurrentlyHungry)
	{
		SetIsHungry(CurrentlyHungry);
	}
	
	if (Hunger >= MaxHunger)
	{
		OnFullyHungry.Broadcast();
	}
}

void UHungerComponent::Eat(AActor* ActorToEat)
{
	IEdibleInterface* EdibleActor = Cast<IEdibleInterface>(ActorToEat);
	
	if(EdibleActor == nullptr)
	{
		return;
	}

	if(bCanStarve)
	{
		DecreaseHunger(EdibleActor->GetHungerSatisfactionValue());
	}

	EdibleActor->GetBitten(BiteStrength);
}
