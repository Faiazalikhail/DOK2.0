// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_Attack.h"
#include "Enemy/DOKEnemyCharacter.h"
#include "AIController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("ATTACK");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ADOKEnemyCharacter* Enemy = Cast<ADOKEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy) return EBTNodeResult::Failed;

	Enemy->PerformAttack();
	return EBTNodeResult::Succeeded;
}

