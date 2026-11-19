// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKZombieMale.h"
#include "Kismet/GameplayStatics.h"

ADOKZombieMale::ADOKZombieMale()
{
	// The lunge carries him further, so his hit reaches further too
	AttackRange = 300.0f;
}

void ADOKZombieMale::PerformAttack()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Player)
	{
		FVector Direction = Player->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.0f;

		LaunchCharacter(Direction.GetSafeNormal() * LungeStrength + FVector(0.0f, 0.0f, LungeUpStrength), true, true);
	}

	Super::PerformAttack();
}
