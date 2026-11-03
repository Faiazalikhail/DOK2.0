#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DOKCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UPoseableMeshComponent;
class USlingshotComponent;
struct FInputActionValue;

UCLASS()
class DOK2_API ADOKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADOKCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyControllerChanged() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed = 700.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slingshot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPoseableMeshComponent> SlingshotMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Slingshot", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USlingshotComponent> Slingshot;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AimWalkSpeed = 250.f;

	bool bWantsToSprint = false;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSprint();
	void StopSprint();

	void StartAim();
	void StopAim();
	void UpdateMoveSpeed();
	void UpdatePouch();
};