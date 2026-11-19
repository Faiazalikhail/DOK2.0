// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/DOKMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDOKMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnStartClicked);
	}

	if (QuitButton) 
	{
		QuitButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnQuitClicked);
	}

	if (InstructionsButton)
	{
		InstructionsButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnInstructionsClicked);
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnBackClicked);
	}

	if (ControlsButton)
	{
		ControlsButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnControlsClicked);
	}

	if (AimButton)
	{
		AimButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnAimClicked);
	}

	if (ItemsButton)
	{
		ItemsButton->OnClicked.AddDynamic(this, &UDOKMainMenuWidget::OnItemsClicked);
	}
}

void UDOKMainMenuWidget::OnStartClicked()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, PlayLevel);
}

void UDOKMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UDOKMainMenuWidget::OnInstructionsClicked()
{
	if (Screens) Screens->SetActiveWidgetIndex(1);
	OnControlsClicked();
}

void UDOKMainMenuWidget::OnControlsClicked()
{
	if (Pages) Pages->SetActiveWidgetIndex(0);
}

void UDOKMainMenuWidget::OnAimClicked()
{
	if (Pages) Pages->SetActiveWidgetIndex(1);
}

void UDOKMainMenuWidget::OnItemsClicked()
{
	if (Pages) Pages->SetActiveWidgetIndex(2);
}

void UDOKMainMenuWidget::OnBackClicked()
{
	if (Screens) Screens->SetActiveWidgetIndex(0);
}