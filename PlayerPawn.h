// © 2020 Marcin Witek 

#pragma once

#include "CoreMinimal.h"

#include "Animal_Rabbit.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "PlayerPawn.generated.h"

UCLASS()
class GAS_CPP_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	UFUNCTION(BlueprintNativeEvent)
	void SetUIRabbitInfo(FRabbitInfo RabbitInfo);

	UFUNCTION(BlueprintNativeEvent)
	void SetRabbitInfoPanelVisibility(bool NewVisibility);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	bool bDrag;

	UPROPERTY(EditDefaultsOnly)
	float DragSpeed;

	UPROPERTY(EditDefaultsOnly)
	float ScrollSpeed;

	UPROPERTY()
	AAnimal_Rabbit* FocusedRabbit;

	UPROPERTY()
	APlayerController* PlayerController;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnRightClick(bool Pressed);

	UFUNCTION()
	void OnMouseX(float AxisValue);

	UFUNCTION()
	void OnMouseY(float AxisValue);

	UFUNCTION()
	void OnMouseWheel(float AxisValue);

	UFUNCTION()
	void OnLeftClick();
};
