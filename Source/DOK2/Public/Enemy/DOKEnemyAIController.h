// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DOKEnemyAIController.generated.h"


class UPawnSensingComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class DOK2_API ADOKEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ADOKEnemyAIController();

	void HearAlert(APawn* Player);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> StartingBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName CanSeePlayerKey = TEXT("CanSeePlayer");

	UPROPERTY(EditDefaultsOnly, Category ="AI")
	FName PlayerTargetKey = TEXT("PlayerTarget");

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LastKnownLocationKey = TEXT("LastKnownLocation");

	// How long a zombie keeps chasing after hearing a scream, without seeing the player
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float AlertMemory = 5.0f;

	// How long a zombie keeps chasing after it stops seeing the player
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float LoseSightTime = 4.0f;

	// How long it searches around the last known location before patrolling again
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SearchTime = 10.0f;

	UFUNCTION()
	void OnSeePlayer(APawn* SeenPawn);

	void SetCanSeePlayer(bool bCanSee, AActor* Player);

	void LosePlayer();
	void StopSearching();

private:

	FTimerHandle ForgetHandle;
	FTimerHandle SearchHandle;

};
