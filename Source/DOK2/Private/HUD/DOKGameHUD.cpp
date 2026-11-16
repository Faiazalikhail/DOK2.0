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
}

void ADOKGameHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ReticlePosition;
	if (!GetReticlePosition(ReticlePosition)) return;

	const APawn* Pawn = GetOwningPawn();
	const USlingshotComponent* Slingshot = Pawn ? Pawn->FindComponentByClass<USlingshotComponent>() : nullptr;
	const float DrawProgress = Slingshot ? Slingshot->GetDrawProgress() : 0.0f;

	const float RingScale = FMath::Lerp(RingMaxScale, RingMinScale, DrawProgress);

	DrawAt(CrosshairRingTexture, RingScale, ReticlePosition);
	DrawAt(CrosshairDotTexture, DotScale, ReticlePosition);
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

void ADOKGameHUD::DrawAt(UTexture2D* Texture, float Scale, const FVector2D& ScreenPosition)
{
	if (!Texture) return;

	const FVector2D Size(Texture->GetSurfaceWidth() * Scale, Texture->GetSurfaceHeight() * Scale);

	FCanvasTileItem Tile(ScreenPosition - Size * 0.5f, Texture->GetResource(), Size, FLinearColor::White);
	Tile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Tile);
}