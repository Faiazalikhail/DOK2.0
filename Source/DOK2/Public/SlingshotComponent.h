// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlingshotComponent.generated.h"


UENUM(BlueprintType)
enum class ESlingshotState : uint8	
{
	Lowered,
	Raised,
	Drawing,
	FullDraw,
	Realoading,
	Slipped
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOK2_API USlingshotComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlingshotComponent();
	void Raise();
	void Lower();

	UFUNCTION(BlueprintPure, Category = "Slingshot")
	ESlingshotState GetState() const { return State; }

	UFUNCTION(BlueprintPure, Category = "Slingshot")
	bool IsRaised() const { return State != ESlingshotState::Lowered; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Slingshot")
	ESlingshotState State = ESlingshotState::Lowered;

	void SetState(ESlingshotState NewState);
};
