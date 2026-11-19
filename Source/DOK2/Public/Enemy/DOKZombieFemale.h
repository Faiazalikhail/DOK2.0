// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/DOKEnemyCharacter.h"
#include "DOKZombieFemale.generated.h"

UCLASS()
class DOK2_API ADOKZombieFemale : public ADOKEnemyCharacter
{
	GENERATED_BODY()

public:
	virtual void OnPlayerSpotted() override;

protected:
	// Zombies inside this distance hear her scream
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Scream")
	float AlertRadius = 2500.0f;
};
