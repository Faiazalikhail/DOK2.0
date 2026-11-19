// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKMushroomDemon.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ADOKMushroomDemon::ADOKMushroomDemon()
{
	// Slow and tough
	HealthComponent->MaxHealth = 300.0f;
	PatrolSpeed = 90.0f;
	ChaseSpeed = 300.0f;
	AttackDamage = 35.0f;
}

void ADOKMushroomDemon::DealAttackDamage()
{
	if (HealthComponent->IsDead()) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!Player || GetDistanceTo(Player) > SlamRadius) return;

	UGameplayStatics::ApplyDamage(Player, AttackDamage, GetController(), this, UDamageType::StaticClass());

	// Throw the player away from the slam
	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	Player->LaunchCharacter(Direction.GetSafeNormal() * KnockbackStrength + FVector(0.0f, 0.0f, KnockbackUpStrength), true, true);
}
