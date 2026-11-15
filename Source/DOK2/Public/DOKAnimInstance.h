// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DOKAnimInstance.generated.h"


class ACharacter;
class UCharacterMovementComponent;
class USlingshotComponent;
class UPoseableMeshComponent;

UCLASS()
class DOK2_API UDOKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float VerticalSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, Category = "Slingshot")
	float SlingAlpha = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Slingshot")
	float AimPitch = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Slingshot")
	float DrawProgress = 0.0f;

	// World Space Points the TwoBoneIK node puts the right hand and elbow on
	UPROPERTY(BlueprintReadOnly, Category = "SlingShot")
	FVector RightHandTarget = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "SlingShot")
	FVector RightElbowTarget = FVector::ZeroVector;

	// Full-draw anchor beside the right cheek, measured from the head bone (X forward, Y right, Z up)
	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	FVector AnchorOffset = FVector(10.f, 8.f, 0.f);

	//Where the pouch sits on SK_Slingshot When the bands are slack
	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	FVector PouchRestOffset = FVector(0.0f, 18.8f, 15.7f);

private:

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY()
	TObjectPtr<USlingshotComponent> Slingshot;

	UPROPERTY()
	TObjectPtr<UPoseableMeshComponent> SlingshotMesh;

	void UpdateRightHand();


	
};
