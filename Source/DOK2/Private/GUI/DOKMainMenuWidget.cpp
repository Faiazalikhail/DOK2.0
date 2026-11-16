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
}

void UDOKMainMenuWidget::OnBackClicked()
{
	if (Screens) Screens->SetActiveWidgetIndex(0);
}