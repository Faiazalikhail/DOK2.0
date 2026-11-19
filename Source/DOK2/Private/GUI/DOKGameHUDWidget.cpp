// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/DOKGameHUDWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UDOKGameHUDWidget::UpdateHealthBar(float Percent)
{
	HealthArc->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), Percent);
}

void UDOKGameHUDWidget::UpdateItem(int32 Item, float CountPercent)
{
	AmmoArc->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Percent"), CountPercent);

	if (ItemTextures.IsValidIndex(Item))
	{
		ItemIcon->SetBrushFromTexture(ItemTextures[Item]);
	}
}
