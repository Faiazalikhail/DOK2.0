// Fill out your copyright notice in the Description page of Project Settings.


#include "DOKAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SlingshotComponent.h"
#include "Components/PoseableMeshComponent.h"

void UDOKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());

	if (Character)
	{
		Movement = Character->GetCharacterMovement();
		Slingshot = Character->FindComponentByClass<USlingshotComponent>();
		SlingshotMesh = Character->FindComponentByClass<UPoseableMeshComponent>();
	}
}

void UDOKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character || !Movement)
	{
		return;
	}

	const FVector Velocity = Character->GetVelocity();
	Speed = Velocity.Size2D();
	VerticalSpeed = Velocity.Z;
	bIsFalling = Movement->IsFalling();
	bIsCrouching = Movement->IsCrouching();

	//Velocity is seen from the character: x is forward and y is right

	const FVector LocalVelocity = Character->GetActorTransform().InverseTransformVectorNoScale(Velocity);
	Direction = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));

	// 0 = lowered, 1 = raised, eased so the arms rise instead of snapping
	if (Slingshot)
	{
		const bool bArmsUp = Slingshot->IsRaised() && Slingshot->GetState() != ESlingshotState::Realoading;
		const float Target = bArmsUp ? 1.0f : 0.0f;
		SlingAlpha = FMath::FInterpTo(SlingAlpha, Target, DeltaSeconds, 10.0f);
		DrawProgress = Slingshot->GetDrawProgress();
	}

	// Camera pitch as -180..180 instead of 0..360, so the aim offset can use it
	AimPitch = FRotator::NormalizeAxis(Character->GetBaseAimRotation().Pitch);

	UpdateRightHand();

}

// Puts the right hand on the pouch and pulls it straight back as the draw grows
void UDOKAnimInstance::UpdateRightHand()
{
	if(!SlingshotMesh)
	{
		return;
	}

	const FTransform SlingTransform = SlingshotMesh->GetComponentTransform();

	// where the slack pouch is, and the direction from the fork back towards the shooter

	const FVector PouchRest = SlingTransform.TransformPosition(PouchRestOffset);


	// IK moves the right hand bone

	USkeletalMeshComponent* Body = GetSkelMeshComponent();
	// Full draw ends at the cheek, under the aiming eye, like a real slingshot shot
	const FVector Anchor = Body->GetBoneLocation(TEXT("head")) + Character->GetActorTransform().TransformVectorNoScale(AnchorOffset);
	const FVector Pinch = FMath::Lerp(PouchRest, Anchor, DrawProgress);

	const FVector PinchOffset = Body->GetSocketLocation(TEXT("PouchSocket")) - Body->GetBoneLocation(TEXT("hand_r"));
	RightHandTarget = Pinch - PinchOffset;

	// Elbow out to the right and slightly back, like drawing a bow
	RightElbowTarget = Body->GetBoneLocation(TEXT("upperarm_r")) + Character->GetActorRightVector() * 40.f - Character->GetActorForwardVector() * 20.f;
}