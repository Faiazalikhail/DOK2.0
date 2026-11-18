// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/DOKEnemyAIController.h"
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

	// forget the player if it stop seeing them for 2 sensing ticks
	GetWorldTimerManager().SetTimer(ForgetHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				SetCanSeePlayer(false, nullptr);
			}),
		PawnSensing->SensingInterval * 2.0f, false);
}

void ADOKEnemyAIController::SetCanSeePlayer(bool bCanSee, AActor* Player)
{
	if(UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsBool(CanSeePlayerKey, bCanSee);
		BB->SetValueAsObject(PlayerTargetKey, Player);
	}
}

