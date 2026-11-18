// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomPatrolLocation.generated.h"

/**
 * 
 */
UCLASS()
class DOK2_API UBTTask_GetRandomPatrolLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_GetRandomPatrolLocation();

	UPROPERTY(EditAnywhere, Category = "AI")
	float Radius = 1000.0f;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
