// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DOKMenuHUD.h"

void ADOKMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	SpawnMainMenuWidget();
}

void ADOKMenuHUD::SpawnMainMenuWidget()
{
	if (!MainMenuWidgetClass) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	MainMenuWidgetContainer = CreateWidget<UDOKMainMenuWidget>(PC, MainMenuWidgetClass);
	if (!MainMenuWidgetContainer) return;

	MainMenuWidgetContainer->AddToViewport();

	PC->SetInputMode(FInputModeUIOnly());
	PC->SetShowMouseCursor(true);
}
