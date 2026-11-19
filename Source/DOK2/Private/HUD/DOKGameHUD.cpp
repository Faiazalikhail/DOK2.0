// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DOKGameHUD.h"
#include "SlingshotComponent.h"

void ADOKGameHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);

	if (GameHUDWidgetClass)
	{
		GameHUDWidgetContainer = CreateWidget<UDOKGameHUDWidget>(PC, GameHUDWidgetClass);
		GameHUDWidgetContainer->AddToViewport();
	}
}

void ADOKGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawFeedback();

	FVector2D ReticlePosition;
	if (!GetReticlePosition(ReticlePosition)) return;

	const APawn* Pawn = GetOwningPawn();
	const USlingshotComponent* Slingshot = Pawn ? Pawn->FindComponentByClass<USlingshotComponent>() : nullptr;
	const float DrawProgress = Slingshot ? Slingshot->GetDrawProgress() : 0.0f;

	const float RingScale = FMath::Lerp(RingMaxScale, RingMinScale, DrawProgress);

	DrawAt(CrosshairRingTexture, RingScale, ReticlePosition);
	DrawAt(CrosshairDotTexture, DotScale, ReticlePosition);
}

void ADOKGameHUD::ShowPauseMenu()
{
	if (!PauseMenuWidgetClass || PauseMenuWidgetContainer) return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	PauseMenuWidgetContainer = CreateWidget<UDOKPauseMenuWidget>(PC, PauseMenuWidgetClass);
	if (!PauseMenuWidgetContainer) return;

	PauseMenuWidgetContainer->AddToViewport();

	PC->SetPause(true);
	PC->SetInputMode(FInputModeUIOnly());
	PC->SetShowMouseCursor(true);
}

void ADOKGameHUD::HidePauseMenu()
{
	if (!PauseMenuWidgetContainer) return;

	PauseMenuWidgetContainer->RemoveFromParent();
	PauseMenuWidgetContainer = nullptr;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	PC->SetPause(false);
	PC->SetInputMode(FInputModeGameOnly());
	PC->SetShowMouseCursor(false);
}

bool ADOKGameHUD::GetReticlePosition(FVector2D& OutScreenPosition) const
{
	APlayerController* PC = GetOwningPlayerController();
	APawn* Pawn = GetOwningPawn();
	if (!PC || !Pawn) return false;

	const USlingshotComponent* Slingshot = Pawn->FindComponentByClass<USlingshotComponent>();
	if (!Slingshot || !Slingshot->IsRaised()) return false;

	FVector Start;
	FVector Direction;
	if (!Slingshot->GetAimRay(Start, Direction)) return false;

	const FVector End = Start + Direction * AimTraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Pawn);
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	const FVector AimPoint = bHit ? Hit.ImpactPoint : End;

	return PC->ProjectWorldLocationToScreen(AimPoint, OutScreenPosition);
}

void ADOKGameHUD::ShowHitMarker(bool bHeadshot)
{
	HitMarkerEndTime = GetWorld()->GetTimeSeconds() + FeedbackTime;
	bLastHitWasHeadshot = bHeadshot;
}

void ADOKGameHUD::ShowDamage()
{
	DamageEndTime = GetWorld()->GetTimeSeconds() + FeedbackTime;
}

void ADOKGameHUD::DrawFeedback()
{
	const float Now = GetWorld()->GetTimeSeconds();

	// Red edges flash and fade when the player gets hurt
	if (DamageTexture && Now < DamageEndTime)
	{
		FCanvasTileItem Flash(FVector2D::ZeroVector, DamageTexture->GetResource(), FVector2D(Canvas->ClipX, Canvas->ClipY),
			FLinearColor(1.0f, 1.0f, 1.0f, (DamageEndTime - Now) / FeedbackTime));
		Flash.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(Flash);
	}

	// Hit marker on the reticle, bigger and red with text for a headshot
	if (Now < HitMarkerEndTime)
	{
		FVector2D Position(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
		GetReticlePosition(Position);

		const float Alpha = (HitMarkerEndTime - Now) / FeedbackTime;
		const FLinearColor Color = bLastHitWasHeadshot ? FLinearColor(1.0f, 0.1f, 0.1f, Alpha) : FLinearColor(1.0f, 1.0f, 1.0f, Alpha);
		DrawAt(HitMarkerTexture, bLastHitWasHeadshot ? 0.5f : 0.3f, Position, Color);

		if (bLastHitWasHeadshot)
		{
			DrawText(TEXT("HEADSHOT"), Color, Position.X - 60.0f, Position.Y + 45.0f, nullptr, 1.5f);
		}
	}
}

void ADOKGameHUD::DrawAt(UTexture2D* Texture, float Scale, const FVector2D& ScreenPosition, const FLinearColor& Color)
{
	if (!Texture) return;

	const FVector2D Size(Texture->GetSurfaceWidth() * Scale, Texture->GetSurfaceHeight() * Scale);

	FCanvasTileItem Tile(ScreenPosition - Size * 0.5f, Texture->GetResource(), Size, Color);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}