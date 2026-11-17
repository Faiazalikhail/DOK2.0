// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversalComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

UTraversalComponent::UTraversalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTraversalComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}

bool UTraversalComponent::TryTraverse()
{
	if (bIsTraversing || !Character) return false;
	if (Character->GetCharacterMovement()->IsFalling()) return false;

	const float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float Radius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Feet = Character->GetActorLocation() - FVector(0.0f, 0.0f, HalfHeight);

	// Check 1: something solid in front of the player, above step height
	const FVector WallStart = Feet + FVector(0.0f, 0.0f, MinLedgeHeight);
	FHitResult WallHit;
	if (!Trace(WallHit, WallStart, WallStart + Forward * (Radius + ReachDistance))) return false;

	// Check 2: a flat top within reach, found by tracing down just behind the wall face
	const FVector OverWall = WallHit.ImpactPoint + Forward * 25.0f;
	FHitResult TopHit;
	const FVector TopStart(OverWall.X, OverWall.Y, Feet.Z + MantleMaxHeight + 20.0f);
	const FVector TopEnd(OverWall.X, OverWall.Y, Feet.Z + MinLedgeHeight);
	if (!Trace(TopHit, TopStart, TopEnd)) return false;
	if (TopHit.ImpactNormal.Z < 0.7f) return false;
	if (TopHit.ImpactPoint.Z > Feet.Z + MantleMaxHeight) return false;

	const float TopZ = TopHit.ImpactPoint.Z;
	const float LedgeHeight = TopZ - Feet.Z;

	// A low wall with floor behind it is vaulted over
	if (LedgeHeight <= VaultMaxHeight)
	{
		const FVector Behind = WallHit.ImpactPoint + Forward * (VaultMaxDepth + Radius);
		FHitResult LandHit;
		const bool bFoundFloor = Trace(LandHit, FVector(Behind.X, Behind.Y, TopZ + 10.0f), FVector(Behind.X, Behind.Y, Feet.Z - 100.0f));
		if (bFoundFloor && LandHit.ImpactPoint.Z < TopZ - 30.0f)
		{
			const FVector LandCentre = LandHit.ImpactPoint + FVector(0.0f, 0.0f, HalfHeight + 2.0f);
			// Check 3: room for the whole body where it ends up
			if (HasRoomFor(LandCentre))
			{
				StartTraversal(LandCentre, TopZ, true);
				return true;
			}
		}
	}

	// Otherwise climb up onto the top
	const FVector TopCentre = TopHit.ImpactPoint + Forward * Radius + FVector(0.0f, 0.0f, HalfHeight + 2.0f);
	if (!HasRoomFor(TopCentre)) return false;

	StartTraversal(TopCentre, TopZ, false);
	return true;
}

bool UTraversalComponent::IsWallAhead() const
{
	if (!Character) return false;

	const float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float Radius = Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector Forward = Character->GetActorForwardVector();
	const FVector WallStart = Character->GetActorLocation() - FVector(0.0f, 0.0f, HalfHeight - MinLedgeHeight);

	FHitResult WallHit;
	return Trace(WallHit, WallStart, WallStart + Forward * (Radius + ReachDistance));
}

bool UTraversalComponent::Trace(FHitResult& OutHit, const FVector& Start, const FVector& End) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, Params);
}

bool UTraversalComponent::HasRoomFor(const FVector& CapsuleCentre) const
{
	const UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	const FCollisionShape Shape = FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), Capsule->GetScaledCapsuleHalfHeight());

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Character);
	return !GetWorld()->OverlapBlockingTestByChannel(CapsuleCentre, FQuat::Identity, ECC_Pawn, Shape, Params);
}

void UTraversalComponent::StartTraversal(const FVector& End, float TopZ, bool bVault)
{
	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

	bIsTraversing = true;
	bIsVault = bVault;
	Duration = bVault ? VaultDuration : MantleDuration;
	Elapsed = 0.0f;
	SpeedBefore = Movement->Velocity.Size2D();

	PathStart = Character->GetActorLocation();
	PathEnd = End;

	// The curve's highest point is half way between the start/end average and the control point,
	// so the control point is placed to make that peak clear the top by the capsule's half height
	const float HalfHeight = Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const float PeakZ = FMath::Max(TopZ + HalfHeight, PathEnd.Z);
	if (bVault)
	{
		PathControl = (PathStart + PathEnd) * 0.5f;
		PathControl.Z = 2.0f * PeakZ - (PathStart.Z + PathEnd.Z) * 0.5f;
	}
	else
	{
		// A mantle pulls up first and then steps forward, so the control point sits above the start
		PathControl = FVector(PathStart.X, PathStart.Y, PathEnd.Z);
	}

	// Movement is switched off so gravity and input don't fight the path
	Movement->StopMovementImmediately();
	Movement->DisableMovement();

	// Play only the climbing part of the clip, sped up or slowed down to last as long as the move
	UAnimMontage* Montage = bVault ? VaultMontage : MantleMontage;
	const FVector2D Window = bVault ? VaultClipWindow : MantleClipWindow;
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (Montage && AnimInstance)
	{
		AnimInstance->Montage_Play(Montage, (Window.Y - Window.X) / Duration, EMontagePlayReturnType::MontageLength, Window.X);
	}
}

void UTraversalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsTraversing) return;

	Elapsed += DeltaTime;
	const float Alpha = FMath::Clamp(Elapsed / Duration, 0.0f, 1.0f);
	const float T = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);

	// Quadratic Bezier curve: start -> control -> end
	const FVector A = FMath::Lerp(PathStart, PathControl, T);
	const FVector B = FMath::Lerp(PathControl, PathEnd, T);
	Character->SetActorLocation(FMath::Lerp(A, B, T));

	if (Alpha >= 1.0f)
	{
		FinishTraversal();
	}
}

void UTraversalComponent::FinishTraversal()
{
	bIsTraversing = false;

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f);
	}

	UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
	Movement->SetMovementMode(MOVE_Walking);

	// A vault keeps the speed the player had going in
	if (bIsVault)
	{
		Movement->Velocity = Character->GetActorForwardVector() * SpeedBefore;
	}
}
