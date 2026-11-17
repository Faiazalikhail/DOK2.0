// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraversalComponent.generated.h"

class ACharacter;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOK2_API UTraversalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTraversalComponent();

	// Looks for a wall in front of the player and starts a vault or a mantle. Returns false if there is nothing to climb
	bool TryTraverse();

	// True when something solid is right in front of the player, used to ignore jump at a wall that can't be climbed
	bool IsWallAhead() const;

	UFUNCTION(BlueprintPure, Category = "Traversal")
	bool IsTraversing() const { return bIsTraversing; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// How far in front of the capsule a wall is looked for
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float ReachDistance = 90.0f;

	// Anything lower than this is just stepped over
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float MinLedgeHeight = 40.0f;

	// Thin obstacles up to this height are vaulted over
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float VaultMaxHeight = 120.0f;

	// Obstacles thinner than this count as a wall to vault, thicker ones as a ledge to mantle onto
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float VaultMaxDepth = 100.0f;

	// The highest ledge the hands can reach
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float MantleMaxHeight = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float VaultDuration = 0.75f;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	float MantleDuration = 0.95f;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	TObjectPtr<UAnimMontage> VaultMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	TObjectPtr<UAnimMontage> MantleMontage;

	// The clips include a run-up and a run-out, so only this window of each one is played
	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FVector2D VaultClipWindow = FVector2D(0.7f, 1.45f);

	UPROPERTY(EditDefaultsOnly, Category = "Traversal")
	FVector2D MantleClipWindow = FVector2D(0.4f, 1.35f);

	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	bool bIsTraversing = false;
	bool bIsVault = false;
	float Duration = 0.0f;
	float Elapsed = 0.0f;
	float SpeedBefore = 0.0f;

	// The eased path: start, a control point above the edge, and the end
	FVector PathStart;
	FVector PathControl;
	FVector PathEnd;

	bool Trace(FHitResult& OutHit, const FVector& Start, const FVector& End) const;
	bool HasRoomFor(const FVector& CapsuleCentre) const;
	void StartTraversal(const FVector& End, float TopZ, bool bVault);
	void FinishTraversal();
};
