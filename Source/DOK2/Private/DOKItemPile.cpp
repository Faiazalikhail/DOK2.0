// Fill out your copyright notice in the Description page of Project Settings.


#include "DOKItemPile.h"
#include "Components/SphereComponent.h"

ADOKItemPile::ADOKItemPile()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(100.0f);
	Trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ADOKItemPile::OnOverlap);
	RootComponent = Trigger;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADOKItemPile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ADOKCharacter* Player = Cast<ADOKCharacter>(OtherActor))
	{
		Player->RefillItem(Item);
	}
}
