// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DOKGameHUD.h"

void ADOKGameHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
}