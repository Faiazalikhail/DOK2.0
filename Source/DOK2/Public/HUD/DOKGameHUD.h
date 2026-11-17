// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "GUI/DOKPauseMenuWidget.h"
#include "DOKGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class DOK2_API ADOKGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

	UPROPERTY(EditDefaultsOnly, Category = "Corsshair")
	TObjectPtr<UTexture2D> CrosshairDotTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Corsshair")
	TObjectPtr<UTexture2D> CrosshairRingTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	float DotScale = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	float RingMaxScale = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	float RingMinScale = 0.14f;

	UPROPERTY(EditDefaultsOnly, Category = "Crosshair")
	float AimTraceDistance = 10000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDOKPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UDOKPauseMenuWidget> PauseMenuWidgetContainer;

	void ShowPauseMenu();
	void HidePauseMenu();
	bool IsPauseMenuOpen() const { return PauseMenuWidgetContainer != nullptr; }

	bool GetReticlePosition(FVector2D& OutScreenPosition) const;
	void DrawAt(UTexture2D* Texture, float Scale, const FVector2D& ScreenPosition);
	
};
