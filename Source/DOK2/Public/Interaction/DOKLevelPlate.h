// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOKLevelPlate.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class DOK2_API ADOKLevelPlate : public AActor
{
	GENERATED_BODY()

public:
	ADOKLevelPlate();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	TObjectPtr<UStaticMeshComponent> PlateMesh;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	TObjectPtr<UTextRenderComponent> LabelText;

	UPROPERTY(EditAnywhere, Category = "Plate")
	TSoftObjectPtr<UWorld> LevelToOpen;

	UPROPERTY(EditAnywhere, Category = "Plate")
	FText Label;

	UPROPERTY(EditAnywhere, Category = "Plate")
	TObjectPtr<UMaterialInterface> PlateMaterial;

	UFUNCTION()
	void OnPlateOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
