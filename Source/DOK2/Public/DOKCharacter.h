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
class UTraversalComponent;
class UHealthComponent;
struct FInputActionValue;


UCLASS()
class DOK2_API ADOKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADOKCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Health")
	TObjectPtr<UHealthComponent> HealthComponent;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Traversal", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTraversalComponent> Traversal;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float RestartDelay = 3.0f;

	FTimerHandle RestartHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CrouchSpeed = 225.f;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float AimWalkSpeed = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float NormalArmLength = 350.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector NormalSocketOffset = FVector(0.f, 60.f, 70.f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float NormalFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float AimArmLength = 75.f;

	// Measured from the spine pivot, not the capsule centre
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector AimSocketOffset = FVector(0.f, 45.f, 40.f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float AimFOV = 70.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float CameraBlendSpeed = 10.f;

	bool bWantsToSprint = false;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpPressed();
	void ToggleCrouch();
	void PauseGame();
	void StartSprint();
	void StopSprint();

	void StartAim();
	void StopAim();
	void UpdateMoveSpeed();
	void UpdatePouch();
	void UpdateCamera(float DeltaSeconds);

	UFUNCTION()
	void HandleDied();

	void RestartLevel();
};