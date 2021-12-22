// © 2020 Marcin Witek 


#include "SightComponent.h"

// Sets default values for this component's properties
USightComponent::USightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SightRadius = 100.f;
	
	if(GetOwner())
	{
		SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
		SightSphere->SetupAttachment(this);
		SightSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		SightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SightMesh"));
		SightMesh->SetupAttachment(this);
		SightMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
}


// Called when the game starts
void USightComponent::BeginPlay()
{
	Super::BeginPlay();

	SightSphere->OnComponentBeginOverlap.AddDynamic(this, &USightComponent::OnActorInSight);
	SightSphere->OnComponentEndOverlap.AddDynamic(this, &USightComponent::OnActorOutOfSight);
	
	SightSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SightSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	SetSightSphereVisibility(false);
}

void USightComponent::OnRegister()
{
	Super::OnRegister();

	if(Mesh)
	{
		SightMesh->SetStaticMesh(Mesh);
	}
	
	InitializeSightSphere();
}

void USightComponent::SetSightRadius(float NewSightRadius)
{
	SightRadius = NewSightRadius;

	InitializeSightSphere();
}

float USightComponent::GetSightRadius() const
{
	return SightRadius;
}

void USightComponent::InitializeSightSphere() const
{
	SightSphere->SetSphereRadius(SightRadius);
	SightMesh->SetRelativeScale3D(FVector(SightRadius/50.f));
}

void USightComponent::SetSightSphereVisibility(bool NewVisibility) const
{
	SightMesh->SetVisibility(NewVisibility);
}

void USightComponent::AddFoodInSight(AActor* Food)
{
	FoodInSight.Add(Food);
	//UE_LOG(LogTemp, Warning, TEXT("Food is in sight!"));
}

void USightComponent::RemoveFoodInSight(AActor* Food)
{
	FoodInSight.Remove(Food);
	//UE_LOG(LogTemp, Warning, TEXT("Food is no longer in sight!"));
}

void USightComponent::AddEnemyInSight(AActor* Enemy)
{
	EnemiesInSight.Add(Enemy);
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is in sight!"));
}

void USightComponent::RemoveEnemyInSight(AActor* Enemy)
{
	EnemiesInSight.Remove(Enemy);
	//UE_LOG(LogTemp, Warning, TEXT("Enemy is no longer in sight!"));
}

void USightComponent::OnActorInSight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValid(OtherActor))
	{
		return;
	}
	
	if(OtherActor->GetClass()->IsChildOf(FoodClass))
	{
		AddFoodInSight(OtherActor);
	}

	if(OtherActor->GetClass()->IsChildOf(EnemyClass))
	{
		AddEnemyInSight(OtherActor);
	}
}

void USightComponent::OnActorOutOfSight(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor->GetClass()->IsChildOf(FoodClass))
	{
		RemoveFoodInSight(OtherActor);
	}

	if(OtherActor->GetClass()->IsChildOf(EnemyClass))
	{
		RemoveEnemyInSight(OtherActor);
	}
}

bool USightComponent::IsSeeingFood() const
{
	return FoodInSight.Num() > 0;
}

bool USightComponent::IsSeeingEnemy() const
{
	return EnemiesInSight.Num() > 0;
}

AActor* USightComponent::GetClosestSeenActor(TArray<AActor*> ActorsInSight) const
{
	if(ActorsInSight.Num() < 1 || !GetOwner())
	{
		return nullptr;
	}

	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	AActor* ClosestActor = ActorsInSight[0];

	if(!IsValid(ClosestActor))
	{
		return nullptr;
	}

	float LowestDistance = FVector::Dist(OwnerLocation, ClosestActor->GetActorLocation());
	float TempDistance = 0.f;
	
	for(AActor* Actor : ActorsInSight)
	{
		if(!IsValid(Actor))
		{
			continue;	
		}

		TempDistance = FVector::Dist(OwnerLocation, Actor->GetActorLocation());

		if(TempDistance < LowestDistance)
		{
			LowestDistance = TempDistance;
			ClosestActor = Actor;
		}
	}

	return ClosestActor;
}

AActor* USightComponent::GetClosestFood() const
{
	return GetClosestSeenActor(FoodInSight);
}

AActor* USightComponent::GetClosestEnemy() const
{
	return GetClosestSeenActor(EnemiesInSight);
}

