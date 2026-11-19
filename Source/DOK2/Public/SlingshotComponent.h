// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SlingshotComponent.generated.h"

class ADOKStone;

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

	void StartDraw();
	void Release();

	bool GetAimRay(FVector& OutStart, FVector& OutDirection) const;

	UFUNCTION(BlueprintPure, Category = "Slingshot")
	ESlingshotState GetState() const { return State; }

	UFUNCTION(BlueprintPure, Category = "Slingshot")
	bool IsRaised() const { return State != ESlingshotState::Lowered; }

	UFUNCTION(BlueprintPure, Category = "Slingshot")
	float GetDrawProgress() const { return DrawProgress; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Slingshot")
	ESlingshotState State = ESlingshotState::Lowered;

	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	float FullDrawTime = 1.5f;

	UPROPERTY(VisibleInstanceOnly, Category = "Slingshot")
	float DrawProgress = 0.0f;

	float DrawTime = 0.0f;

	void StopDrawing();

	void SetState(ESlingshotState NewState);


	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	TSubclassOf<ADOKStone> StoneClass;

	// Shot speed at a tiny draw and at full draw (cm/s)
	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	float MinShotSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	float MaxShotSpeed = 4000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	float AimTraceDistance = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	FVector ForkOffset = FVector(0.0f, 0.0f, 15.7f);

	void Fire();

	// Arms drop to take a new stone and come back up while aim is still held
	UPROPERTY(EditDefaultsOnly, Category = "Slingshot")
	float ReloadTime = 0.8f;

	FTimerHandle ReloadHandle;

	void FinishReload();

};
