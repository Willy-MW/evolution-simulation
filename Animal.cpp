// © 2020 Marcin Witek 


#include "Animal.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AAnimal::AAnimal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Collision->SetCanEverAffectNavigation(false);
	Collision->AreaClass = nullptr;
	RootComponent = Collision;

	AnimalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AnimalMesh"));
	AnimalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AnimalMesh->SetupAttachment(Collision);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = 200.f;
	MovementComponent->MovementState.bCanWalk = true;

	SightComponent = CreateDefaultSubobject<USightComponent>(TEXT("SightComponent"));
	SightComponent->SetupAttachment(Collision);

	HungerComponent = CreateDefaultSubobject<UHungerComponent>(TEXT("HungerComponent"));

	BaseSight = 100.f;
	BaseSize = 100.f;
	BaseSpeed = 300.f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
}

void AAnimal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	UpdateAnimalAttributes();
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAnimal::SetTraits(const FAnimalTraits NewTraits)
{
	Traits = NewTraits;

	UpdateAnimalAttributes();
}

FAnimalTraits AAnimal::GetTraits() const
{
	return Traits;
}

float AAnimal::GetSight() const
{
	return BaseSight * Traits.Sight;
}

float AAnimal::GetSpeed() const
{
	return BaseSpeed * Traits.Speed;
}

float AAnimal::GetSize() const
{
	return BaseSize * Traits.Size;
}

void AAnimal::UpdateAnimalAttributes() const
{
	MovementComponent->MaxSpeed = GetSpeed();
	SightComponent->SetSightRadius(GetSight());
	HungerComponent->SetMaxHunger(GetSize());
}

bool AAnimal::IsMoving() const
{
	return MovementComponent->Velocity.Size() > 0.02f;
}

bool AAnimal::IsFoodInRange() const
{
	AActor* ClosestFood = SightComponent->GetClosestFood();

	if(!IsValid(ClosestFood))
	{
		return false;
	}

	float DistanceToFood = FVector::Dist(ClosestFood->GetActorLocation(), GetActorLocation());
	
	float FoodBoundsRadius = ClosestFood->GetRootComponent()->Bounds.SphereRadius;
	float AnimalRadius = Collision->GetScaledCapsuleRadius();
	
	float TimeDilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	float TimeDilationFactor = 0.1f * FMath::Pow(TimeDilation, 2) + 0.3f * TimeDilation + 50.f;
	
	float ActorsTouchDistance = FoodBoundsRadius + AnimalRadius + TimeDilationFactor;

	return DistanceToFood <= ActorsTouchDistance;
}
