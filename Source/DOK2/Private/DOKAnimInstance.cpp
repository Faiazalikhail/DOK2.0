// Fill out your copyright notice in the Description page of Project Settings.


#include "DOKAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDOKAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());

	if (Character) 
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UDOKAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!Character || !Movement)
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
}

