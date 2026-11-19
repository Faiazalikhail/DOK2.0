// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DOKEnemyCharacter.h"
#include "DOKZombieMale.generated.h"

UCLASS()
class DOK2_API ADOKZombieMale : public ADOKEnemyCharacter
{
	GENERATED_BODY()

public:
	ADOKZombieMale();

	virtual void PerformAttack() override;

protected:
	// How hard he throws himself at the player when he attacks
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Lunge")
	float LungeStrength = 900.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Lunge")
	float LungeUpStrength = 200.0f;
};
