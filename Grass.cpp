// © 2020 Marcin Witek 


#include "Grass.h"

// Sets default values
AGrass::AGrass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HungerSatisfaction = 50.f;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->SetSphereRadius(20.f);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetupAttachment(SphereCollision);
}

float AGrass::GetHungerSatisfactionValue()
{
	return HungerSatisfaction;
}

void AGrass::GetBitten(float BiteStrength)
{
	Destroy();
}

