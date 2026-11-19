// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "GUI/DOKPauseMenuWidget.h"
#include "GUI/DOKGameHUDWidget.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UDOKGameHUDWidget> GameHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UDOKGameHUDWidget> GameHUDWidgetContainer;

	UDOKGameHUDWidget* GetGameHUDWidget() const { return GameHUDWidgetContainer; }

	void ShowPauseMenu();
	void HidePauseMenu();
	bool IsPauseMenuOpen() const { return PauseMenuWidgetContainer != nullptr; }

	UPROPERTY(EditDefaultsOnly, Category = "Feedback")
	TObjectPtr<UTexture2D> HitMarkerTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Feedback")
	TObjectPtr<UTexture2D> DamageTexture;

	// How long the hit marker and the red damage flash take to fade out
	UPROPERTY(EditDefaultsOnly, Category = "Feedback")
	float FeedbackTime = 0.4f;

	void ShowHitMarker(bool bHeadshot);
	void ShowDamage();

	bool GetReticlePosition(FVector2D& OutScreenPosition) const;
	void DrawAt(UTexture2D* Texture, float Scale, const FVector2D& ScreenPosition, const FLinearColor& Color = FLinearColor::White);

private:
	float HitMarkerEndTime = 0.0f;
	float DamageEndTime = 0.0f;
	bool bLastHitWasHeadshot = false;

	void DrawFeedback();
	
};
