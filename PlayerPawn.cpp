// © 2020 Marcin Witek 


#include "PlayerPawn.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 1300.f;
	SpringArm->SetRelativeRotation(FRotator(-50.f,90.f,0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bDrag = false;
	DragSpeed = 20;
	ScrollSpeed = 120;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void APlayerPawn::SetRabbitInfoPanelVisibility_Implementation(bool NewVisibility)
{
	//UE_LOG(LogTemp, Warning, TEXT("Info Panel Visibility: %s"), ( NewVisibility ? TEXT("true") : TEXT("false") ));
}

void APlayerPawn::SetUIRabbitInfo_Implementation(FRabbitInfo RabbitInfo)
{
	//UE_LOG(LogTemp, Warning, TEXT("SetUIRabbitInfo"));
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(FocusedRabbit)
	{
		SetUIRabbitInfo(FocusedRabbit->GetRabbitInfo());
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	DECLARE_DELEGATE_OneParam(FRightClickDelegate, const bool);
	InputComponent->BindAction<FRightClickDelegate>("RightClick", IE_Pressed, this, &APlayerPawn::OnRightClick, true);
	InputComponent->BindAction<FRightClickDelegate>("RightClick", IE_Released, this, &APlayerPawn::OnRightClick, false);
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &APlayerPawn::OnLeftClick);

	InputComponent->BindAxis("MouseX", this, &APlayerPawn::OnMouseX);
	InputComponent->BindAxis("MouseY", this, &APlayerPawn::OnMouseY);
	InputComponent->BindAxis("MouseWheel", this, &APlayerPawn::OnMouseWheel);
}

void APlayerPawn::OnRightClick(const bool Pressed)
{
	bDrag = Pressed;
}

void APlayerPawn::OnMouseX(float AxisValue)
{
	if (!bDrag)
	{
		return;
	}

	AddActorWorldOffset(FVector(AxisValue * DragSpeed, 0.f, 0.f));
}

void APlayerPawn::OnMouseY(float AxisValue)
{
	if (!bDrag)
	{
		return;
	}

	AddActorWorldOffset(FVector(0.f,AxisValue * DragSpeed, 0.f));
}

void APlayerPawn::OnMouseWheel(float AxisValue)
{
	const float NewLength = AxisValue * ScrollSpeed + SpringArm->TargetArmLength;
	SpringArm->TargetArmLength = FMath::Clamp(NewLength, 400.f, 3000.f);
}

void APlayerPawn::OnLeftClick()
{
	if(!PlayerController)
	{
		return;
	}

	FHitResult Hit;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, Hit);
	AAnimal_Rabbit* HitActor = Cast<AAnimal_Rabbit>(Hit.Actor);
	
	if(!IsValid(HitActor))
	{
		SetRabbitInfoPanelVisibility(false);

		if(FocusedRabbit)
		{
			FocusedRabbit->SightComponent->SetSightSphereVisibility(false);
		}

		FocusedRabbit = nullptr;
		return;
	}

	if(FocusedRabbit)
	{
		FocusedRabbit->SightComponent->SetSightSphereVisibility(false);
	}

	FocusedRabbit = HitActor;
	FocusedRabbit->SightComponent->SetSightSphereVisibility(true);

	SetUIRabbitInfo(FocusedRabbit->GetRabbitInfo());
	SetRabbitInfoPanelVisibility(true);
}

