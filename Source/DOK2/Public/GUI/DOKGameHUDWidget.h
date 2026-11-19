// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DOKGameHUDWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class DOK2_API UDOKGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateHealthBar(float Percent);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateItem(int32 Item, float CountPercent);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> HealthArc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AmmoArc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	// Centre icon for each item, in EDOKItem order
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TArray<TObjectPtr<UTexture2D>> ItemTextures;
};
