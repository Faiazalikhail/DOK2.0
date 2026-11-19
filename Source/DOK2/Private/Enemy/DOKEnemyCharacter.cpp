// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKEnemyCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/DOKGameHUD.h"

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

	// The player's camera arm should not treat zombies as walls, it snaps into their body
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

// Called when the game starts or when spawned
void ADOKEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrawlSpeed;

	HealthComponent->OnDied.AddDynamic(this, &ADOKEnemyCharacter::HandleDied);

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ADOKEnemyCharacter::OnMontageBlendingOut);
	}
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
	const bool bHeadshot = Bone.Contains(TEXT("head")) || Bone.Contains(TEXT("neck"));
	if (bHeadshot)
	{
		DamageAmount = HealthComponent->GetHealth();
	}
	else if (Bone.Contains(TEXT("thigh")) || Bone.Contains(TEXT("calf")) || Bone.Contains(TEXT("leg")) || Bone.Contains(TEXT("foot")))
	{
		DamageAmount *= LegDamageScale;
		StartCrawling();
	}

	HealthComponent->ApplyHealthChange(-DamageAmount);

	// Hit feedback on the shooter's screen
	if (APlayerController* PC = Cast<APlayerController>(EventInstigator))
	{
		if (ADOKGameHUD* HUD = PC->GetHUD<ADOKGameHUD>())
		{
			HUD->ShowHitMarker(bHeadshot);
		}
	}

	if (!HealthComponent->IsDead() && !bIsCrawling)
	{
		PlayEnemyMontage(HitReactMontage);
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
	bChasing = true;
	GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

	if(!bIsCrawling)
	{
		PlayEnemyMontage(DetectMontage);
	}
}

void ADOKEnemyCharacter::OnPlayerLost()
{
	bChasing = false;

	if (!GetCurrentMontage())
	{
		GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
	}
}

void ADOKEnemyCharacter::PlayEnemyMontage(UAnimMontage* Montage)
{
	// Only the walk and run animations move the zombie, it stands still while a montage plays
	if (PlayAnimMontage(Montage) > 0.0f)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
}

void ADOKEnemyCharacter::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->MaxWalkSpeed = bChasing ? ChaseSpeed : PatrolSpeed;
}

void ADOKEnemyCharacter::PerformAttack()
{
	PlayEnemyMontage(AttackMontage);
	GetWorldTimerManager().SetTimer(AttackHandle, this, &ADOKEnemyCharacter::DealAttackDamage, AttackHitDelay, false);
}

void ADOKEnemyCharacter::DealAttackDamage()
{
	if (HealthComponent->IsDead()) return;

	// player should be hurt only when in reach
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if(IsPlayerInReach(Player, AttackRange))
	{
		UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, UDamageType::StaticClass());
	}
}

bool ADOKEnemyCharacter::IsPlayerInReach(AActor* Player, float Range) const
{
	if (!Player) return false;

	const FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
	return ToPlayer.Size2D() <= Range && FMath::Abs(ToPlayer.Z) <= AttackHeight;
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
