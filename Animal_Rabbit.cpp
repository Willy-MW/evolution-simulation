// © 2020 Marcin Witek 

#include "Animal_Rabbit.h"

AAnimal_Rabbit::AAnimal_Rabbit()
{
	HungerSatisfaction = 75.f;
	Id = 0;

	Collision->SetCapsuleRadius(21.f);
	Collision->SetCapsuleHalfHeight(21.f);
}

void AAnimal_Rabbit::BeginPlay()
{
	Super::BeginPlay();
	
	HungerComponent->OnFullyHungry.AddDynamic(this, &AAnimal_Rabbit::OnFullyHungry);
}

void AAnimal_Rabbit::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if(Material)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(Material,this);
		AnimalMesh->SetMaterial(0, DynamicMaterial);
	}
}

void AAnimal_Rabbit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SpendEnergy();
	UpdateColor();
}

void AAnimal_Rabbit::GetBitten(float BiteStrength)
{
	HungerComponent->IncreaseHunger(BiteStrength);
}

float AAnimal_Rabbit::GetHungerSatisfactionValue()
{
	return HungerSatisfaction;
}

void AAnimal_Rabbit::SpendEnergy() const
{
	if(!IsValid(HungerComponent))
	{
		return;
	}
	
	float MinimalCost = 0.5f;
	float CostMultiplier = 2.f;
	float MovementCost = IsMoving() ? ((Traits.Size * Traits.Speed + MinimalCost) * CostMultiplier) : 0.f;

	float FinalCost = (MovementCost + MinimalCost) * GetWorld()->GetDeltaSeconds();

	HungerComponent->IncreaseHunger(FinalCost);
	//UE_LOG(LogTemp, Warning, TEXT("Energy Spent: %f, Hunger Level: %f"), FinalCost, HungerComponent->GetHungerPercentage());
}

void AAnimal_Rabbit::UpdateColor() const
{
	if(DynamicMaterial == nullptr || !IsValid(HungerComponent))
	{
		return;
	}

	const float InvHunger = 1.f - HungerComponent->GetHungerPercentage();
	const FLinearColor NewColor = FLinearColor(FVector4(1.f, InvHunger, InvHunger, 1.f));
	
	DynamicMaterial->SetVectorParameterValue("Color", NewColor);
	//UE_LOG(LogTemp, Warning, TEXT("Color level: %f"), InvHunger);
}

FRabbitInfo AAnimal_Rabbit::GetRabbitInfo() const
{
	FRabbitInfo Info;

	Info.Id = Id;
	Info.UnitInfo.HungerPercent = HungerComponent->GetHungerPercentage();
	Info.UnitInfo.TimeAlive = GetGameTimeSinceCreation();
	Info.UnitInfo.UnitTraits = Traits;

	return Info;
}

void AAnimal_Rabbit::Die()
{
	OnRabbitDeath.Broadcast(GetRabbitInfo());
	Destroy();
}

void AAnimal_Rabbit::OnFullyHungry()
{
	Die();
}

void AAnimal_Rabbit::SetId(const int NewId)
{
	Id = NewId;
}

