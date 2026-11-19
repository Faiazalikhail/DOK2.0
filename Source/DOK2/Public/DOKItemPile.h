// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DOKCharacter.h"
#include "DOKItemPile.generated.h"

class USphereComponent;

UCLASS()
class DOK2_API ADOKItemPile : public AActor
{
	GENERATED_BODY()

public:
	ADOKItemPile();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pile")
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, Category = "Pile")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Which item walking into this pile refills
	UPROPERTY(EditAnywhere, Category = "Pile")
	EDOKItem Item = EDOKItem::Stone;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
