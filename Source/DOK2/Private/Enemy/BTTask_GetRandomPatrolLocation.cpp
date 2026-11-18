// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_GetRandomPatrolLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_GetRandomPatrolLocation::UBTTask_GetRandomPatrolLocation()
{
	NodeName = TEXT("Get Random Patrol Location");
}

EBTNodeResult::Type UBTTask_GetRandomPatrolLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	if (!Pawn || !NavSys) return EBTNodeResult::Failed;

	FNavLocation Result;
	if(!NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, Result))
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RandomPatrolLocation"), Result.Location);
	return EBTNodeResult::Succeeded;
}
