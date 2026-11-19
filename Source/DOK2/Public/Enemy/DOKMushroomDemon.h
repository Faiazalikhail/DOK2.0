// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DOKEnemyCharacter.h"
#include "DOKMushroomDemon.generated.h"

UCLASS()
class DOK2_API ADOKMushroomDemon : public ADOKEnemyCharacter
{
	GENERATED_BODY()

public:
	ADOKMushroomDemon();

protected:
	virtual void DealAttackDamage() override;

	// Everything inside this distance gets hit by the ground slam
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Slam")
	float SlamRadius = 350.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Slam")
	float KnockbackStrength = 1200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Slam")
	float KnockbackUpStrength = 400.0f;
};
