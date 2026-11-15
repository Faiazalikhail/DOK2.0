// Fill out your copyright notice in the Description page of Project Settings.


#include "SlingshotComponent.h"
#include "DOKStone.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
USlingshotComponent::USlingshotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// ...
}


// Called when the game starts
void USlingshotComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USlingshotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DrawTime += DeltaTime;
	DrawProgress = FMath::Clamp(DrawTime / FullDrawTime, 0.0f, 1.0f);

	if(State == ESlingshotState::Drawing && DrawProgress >= 1.0f)
	{
		SetState(ESlingshotState::FullDraw);
	}

	// ...
}

void USlingshotComponent::Raise()
{
	if (State == ESlingshotState::Lowered)
	{
		SetState(ESlingshotState::Raised);
	}
}

void USlingshotComponent::Lower()
{
	StopDrawing();
	SetState(ESlingshotState::Lowered);
}

void USlingshotComponent::SetState(ESlingshotState NewState)
{
	if (State == NewState)
	{
		return;
	}

	State = NewState;
	UE_LOG(LogTemp, Log, TEXT("Slingshot state: %s"), *UEnum::GetValueAsString(State));
}

void USlingshotComponent::StartDraw()
{
	// you can only start drawing if the slingshot is raised and not drawn

	if(State!= ESlingshotState::Raised)
	{
		return;
	}

	DrawTime = 0.0f;
	DrawProgress = 0.0f;
	SetComponentTickEnabled(true);
	SetState(ESlingshotState::Drawing);
}

void USlingshotComponent::Release()
{
	if(State != ESlingshotState::Drawing && State != ESlingshotState::FullDraw)
	{
		return;
	}

	Fire();

	StopDrawing();
	// Pouch is empty now; the next RMB press (Lower, then Raise) reloads it
	SetState(ESlingshotState::Realoading);
}

void USlingshotComponent::StopDrawing()
{
	DrawTime = 0.0f;
	DrawProgress = 0.0f;
	SetComponentTickEnabled(false);
}

void USlingshotComponent::Fire()
{
	APawn* Shooter = Cast<APawn>(GetOwner());
	if (!StoneClass || !Shooter)
	{
		return;
	}
	APlayerController* PlayerController = Cast<APlayerController>(Shooter->GetController());
	if (!PlayerController)
	{
		return;
	}

	// 1. Find what the middle of the screen is pointing at
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	const FVector TraceEnd = CameraLocation + CameraRotation.Vector() * AimTraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Shooter);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility, Params);
	const FVector AimPoint = bHit ? Hit.ImpactPoint : TraceEnd;

	// 2. The stone leaves from the pouch and flies toward that point
	UPoseableMeshComponent* SlingMesh = Shooter->FindComponentByClass<UPoseableMeshComponent>();
	const FVector Start = SlingMesh ? SlingMesh->GetBoneLocationByName(TEXT("Pouch"), EBoneSpaces::WorldSpace) : CameraLocation;
	const FVector Direction = (AimPoint - Start).GetSafeNormal();

	// 3. A longer draw means a faster shot
	const float Speed = FMath::Lerp(MinShotSpeed, MaxShotSpeed, DrawProgress);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Shooter;
	SpawnParams.Instigator = Shooter;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ADOKStone* Stone = GetWorld()->SpawnActor<ADOKStone>(StoneClass, Start, Direction.Rotation(), SpawnParams))
	{
		Stone->Launch(Direction * Speed);
	}
}