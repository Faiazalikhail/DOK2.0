// Fill out your copyright notice in the Description page of Project Settings.


#include "DOKAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SlingshotComponent.h"

void UDOKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());

	if (Character)
	{
		Movement = Character->GetCharacterMovement();
		Slingshot = Character->FindComponentByClass<USlingshotComponent>();
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

	//Velocity is seen from the character: x is forward and y is right

	const FVector LocalVelocity = Character->GetActorTransform().InverseTransformVectorNoScale(Velocity);
	Direction = FMath::RadiansToDegrees(FMath::Atan2(LocalVelocity.Y, LocalVelocity.X));

	// 0 = lowered, 1 = raised, eased so the arms rise instead of snapping
	if (Slingshot)
	{
		const float Target = Slingshot->IsRaised() ? 1.0f : 0.0f;
		SlingAlpha = FMath::FInterpTo(SlingAlpha, Target, DeltaSeconds, 10.0f);
		DrawProgress = Slingshot->GetDrawProgress();
	}

	// Camera pitch as -180..180 instead of 0..360, so the aim offset can use it
	AimPitch = FRotator::NormalizeAxis(Character->GetBaseAimRotation().Pitch);
}