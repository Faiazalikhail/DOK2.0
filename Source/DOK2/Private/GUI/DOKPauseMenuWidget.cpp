// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/DOKPauseMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HUD/DOKGameHUD.h"

void UDOKPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UDOKPauseMenuWidget::OnResumeClicked);
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UDOKPauseMenuWidget::OnMainMenuClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UDOKPauseMenuWidget::OnQuitClicked);
	}
}

void UDOKPauseMenuWidget::OnResumeClicked()
{
	APlayerController* PC = GetOwningPlayer();
	ADOKGameHUD* HUD = PC ? PC->GetHUD<ADOKGameHUD>() : nullptr;
	if (HUD)
	{
		HUD->HidePauseMenu();
	}
}

void UDOKPauseMenuWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, MainMenuLevel);
}

void UDOKPauseMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
