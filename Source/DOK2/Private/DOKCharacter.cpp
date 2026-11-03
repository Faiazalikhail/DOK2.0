#include "DOKCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "SlingshotComponent.h"

ADOKCharacter::ADOKCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Turning the mouse left/right turns the whole body
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Third person: a spring arm holds the camera behind and over the right
	// shoulder, follows the mouse, and pulls in when something is in the way
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 350.f;
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 70.f);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 12.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Full body mesh, feet on the floor, facing the capsule's forward
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -96.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SlingshotMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SlingshotMesh"));
	SlingshotMesh->SetupAttachment(GetMesh(), TEXT("SlingshotSocket"));
	SlingshotMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Slingshot = CreateDefaultSubobject<USlingshotComponent>(TEXT("Slingshot"));
}

void ADOKCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateMoveSpeed();

	UE_LOG(LogTemp, Warning, TEXT("DOKCharacter spawned: %s"), *GetName());
}

void ADOKCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ADOKCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADOKCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADOKCharacter::Look);

		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ADOKCharacter::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADOKCharacter::StopSprint);

		EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &ADOKCharacter::StartAim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &ADOKCharacter::StopAim);
	}
}

void ADOKCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), Input.Y);
	AddMovementInput(GetActorRightVector(), Input.X);
}

void ADOKCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void ADOKCharacter::StartSprint()
{
	bWantsToSprint = true;
	UpdateMoveSpeed();
}

void ADOKCharacter::StopSprint()
{
	bWantsToSprint = false;
	UpdateMoveSpeed();
}

void ADOKCharacter::StartAim()
{
	Slingshot->Raise();
	UpdateMoveSpeed();
}

void ADOKCharacter::StopAim()
{
	Slingshot->Lower();
	UpdateMoveSpeed();
}

void ADOKCharacter::UpdateMoveSpeed()
{
	float NewSpeed = WalkSpeed;
	if (Slingshot->IsRaised()) 
	{
		NewSpeed = AimWalkSpeed;
	}
	else if (bWantsToSprint) 
	{
		NewSpeed = SprintSpeed;
	}

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}