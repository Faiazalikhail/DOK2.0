// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKZombieFemale.h"
#include "Enemy/DOKEnemyAIController.h"
#include "Kismet/GameplayStatics.h"

void ADOKZombieFemale::OnPlayerSpotted()
{
	Super::OnPlayerSpotted();

	APawn* Player = UGameplayStatics::GetPlayerPawn(this, 0);

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, ADOKEnemyCharacter::StaticClass(), Enemies);

	for (AActor* Actor : Enemies)
	{
		if (Actor == this || GetDistanceTo(Actor) > AlertRadius) continue;

		ADOKEnemyAIController* AI = Cast<ADOKEnemyAIController>(Cast<APawn>(Actor)->GetController());
		if (AI)
		{
			AI->HearAlert(Player);
		}
	}
}
