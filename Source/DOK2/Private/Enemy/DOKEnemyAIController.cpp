// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKEnemyAIController.h"
#include "Enemy/DOKEnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "DOKCharacter.h"
#include "TimerManager.h"


ADOKEnemyAIController::ADOKEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 2000.0f;
	PawnSensing->SetPeripheralVisionAngle(60.0f);
	PawnSensing->SensingInterval = 0.5f;
}

void ADOKEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PawnSensing->OnSeePawn.AddDynamic(this, &ADOKEnemyAIController::OnSeePlayer);

	if(StartingBehaviorTree)
	{
		RunBehaviorTree(StartingBehaviorTree);
	}
}

void ADOKEnemyAIController::OnSeePlayer(APawn* SeenPawn)
{
	ADOKCharacter* Player = Cast<ADOKCharacter>(SeenPawn);
	if (!Player) return;

	SetCanSeePlayer(true, Player);

	// Keeps chasing for a while after the player slips out of view
	GetWorldTimerManager().SetTimer(ForgetHandle, this, &ADOKEnemyAIController::LosePlayer, LoseSightTime, false);
}

void ADOKEnemyAIController::HearAlert(APawn* Player)
{
	SetCanSeePlayer(true, Player);

	GetWorldTimerManager().SetTimer(ForgetHandle, this, &ADOKEnemyAIController::LosePlayer, AlertMemory, false);
}

void ADOKEnemyAIController::LosePlayer()
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	// Remember where the player was, the tree searches there before going back to patrol
	if (AActor* Player = Cast<AActor>(BB->GetValueAsObject(PlayerTargetKey)))
	{
		BB->SetValueAsVector(LastKnownLocationKey, Player->GetActorLocation());
	}

	SetCanSeePlayer(false, nullptr);

	GetWorldTimerManager().SetTimer(SearchHandle, this, &ADOKEnemyAIController::StopSearching, SearchTime, false);
}

void ADOKEnemyAIController::StopSearching()
{
	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->ClearValue(LastKnownLocationKey);
	}
}

void ADOKEnemyAIController::SetCanSeePlayer(bool bCanSee, AActor* Player)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	const bool bChanged = bCanSee != BB->GetValueAsBool(CanSeePlayerKey);

	// Set before telling the enemy, or two screamers keep alerting each other forever
	BB->SetValueAsBool(CanSeePlayerKey, bCanSee);
	BB->SetValueAsObject(PlayerTargetKey, Player);

	ADOKEnemyCharacter* Enemy = Cast<ADOKEnemyCharacter>(GetPawn());
	if(Enemy && bChanged)
	{
		if(bCanSee)
		{
			Enemy->OnPlayerSpotted();
		}
		else
		{
			Enemy->OnPlayerLost();
		}
	}
}

