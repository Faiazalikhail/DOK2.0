// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GUI/DOKMainMenuWidget.h"
#include "DOKMenuHUD.generated.h"

/**
 *
 */
UCLASS()
class DOK2_API ADOKMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDOKMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UDOKMainMenuWidget> MainMenuWidgetContainer;

	void SpawnMainMenuWidget();
};
