// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DOKMainMenuWidget.generated.h"

class UButton;
class UWidgetSwitcher;

/**
 *
 */
UCLASS()
class DOK2_API UDOKMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSoftObjectPtr<UWorld> PlayLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Screens;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InstructionsButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnInstructionsClicked();

	UFUNCTION()
	void OnBackClicked();


};
