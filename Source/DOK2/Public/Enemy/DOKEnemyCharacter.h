// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DOKEnemyCharacter.generated.h"

class UHealthComponent;
class UAnimMontage;

UCLASS()
class DOK2_API ADOKEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADOKEnemyCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnPlayerSpotted();
	void OnPlayerLost();

	virtual void PerformAttack();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	bool bIsCrawling = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleDied();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float PatrolSpeed = 120.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float ChaseSpeed = 450.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	float CrawlSpeed = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Damage")
	float LegDamageScale = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Damage")
	float DeathImpulse = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Attack")
	float AttackDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Attack")
	float AttackRange = 200.0f;

	// time from the stat of attack montage to the when hit lands
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Attack")
	float AttackHitDelay = 0.5f;

	void DealAttackDamage();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Animation")
	TObjectPtr<UAnimMontage> DetectMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

private:
	FName LastHitBone;
	FVector LastHitDirection = FVector::ZeroVector;

	FTimerHandle AttackHandle;

	void StartCrawling();
};
