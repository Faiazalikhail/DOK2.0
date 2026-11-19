// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKEnemyCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADOKEnemyCharacter::ADOKEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Stones go through the capsule and hit the mesh, so we know which bone was hit
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
}

// Called when the game starts or when spawned
void ADOKEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrawlSpeed;

	HealthComponent->OnDied.AddDynamic(this, &ADOKEnemyCharacter::HandleDied);
}

float ADOKEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Applied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (HealthComponent->IsDead()) return Applied;

	LastHitBone = NAME_None;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent& PointDamage = static_cast<const FPointDamageEvent&>(DamageEvent);
		LastHitBone = PointDamage.HitInfo.BoneName;
		LastHitDirection = PointDamage.ShotDirection;
	}

	const FString Bone = LastHitBone.ToString().ToLower();
	if (Bone.Contains(TEXT("head")) || Bone.Contains(TEXT("neck")))
	{
		DamageAmount = HealthComponent->GetHealth();
	}
	else if (Bone.Contains(TEXT("thigh")) || Bone.Contains(TEXT("calf")) || Bone.Contains(TEXT("leg")) || Bone.Contains(TEXT("foot")))
	{
		DamageAmount *= LegDamageScale;
		StartCrawling();
	}

	HealthComponent->ApplyHealthChange(-DamageAmount);

	if (!HealthComponent->IsDead() && !bIsCrawling)
	{
		PlayAnimMontage(HitReactMontage);
	}

	return Applied;
}

void ADOKEnemyCharacter::StartCrawling()
{
	if (bIsCrawling) return;

	bIsCrawling = true;
	Crouch();
}

void ADOKEnemyCharacter::OnPlayerSpotted()
{
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

	if(!bIsCrawling)
	{
		PlayAnimMontage(DetectMontage);
	}
}

void ADOKEnemyCharacter::OnPlayerLost()
{
	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
}

void ADOKEnemyCharacter::PerformAttack()
{
	PlayAnimMontage(AttackMontage);
	GetWorldTimerManager().SetTimer(AttackHandle, this, &ADOKEnemyCharacter::DealAttackDamage, AttackHitDelay, false);
}

void ADOKEnemyCharacter::DealAttackDamage()
{
	if (HealthComponent->IsDead()) return;

	// player should be hurt only when in reach
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if(Player && GetDistanceTo(Player) <= AttackRange)
	{
		UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}

void ADOKEnemyCharacter::HandleDied()
{
	GetCharacterMovement()->DisableMovement();
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(LastHitDirection * DeathImpulse, LastHitBone, true);

	SetLifeSpan(10.0f);
}
