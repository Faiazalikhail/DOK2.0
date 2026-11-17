// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/DOKLevelPlate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DOKCharacter.h"

ADOKLevelPlate::ADOKLevelPlate()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->InitBoxExtent(FVector(75.0f, 75.0f, 50.0f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADOKLevelPlate::OnPlateOverlap);
	RootComponent = TriggerBox;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlateMeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (PlateMeshAsset.Succeeded())
	{
		PlateMesh->SetStaticMesh(PlateMeshAsset.Object);
	}
	PlateMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -48.0f));
	PlateMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.04f));
	PlateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlateMesh->SetupAttachment(RootComponent);

	// Lies flat on the floor in front of the plate, readable as the player walks up to it
	LabelText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LabelText"));
	LabelText->SetRelativeLocation(FVector(-110.0f, 0.0f, -49.0f));
	LabelText->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	LabelText->SetHorizontalAlignment(EHTA_Center);
	LabelText->SetVerticalAlignment(EVRTA_TextCenter);
	LabelText->SetWorldSize(28.0f);
	LabelText->SetupAttachment(RootComponent);
}

// Runs in the editor whenever the plate is placed or changed, so the level shows the real label and colour
void ADOKLevelPlate::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	LabelText->SetText(Label);

	if (PlateMaterial)
	{
		PlateMesh->SetMaterial(0, PlateMaterial);
	}
}

void ADOKLevelPlate::OnPlateOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ADOKCharacter>(OtherActor)) return;
	if (LevelToOpen.IsNull()) return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelToOpen);
}
