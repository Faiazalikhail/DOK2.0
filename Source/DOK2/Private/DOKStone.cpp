// Fill out your copyright notice in the Description page of Project Settings.


#include "DOKStone.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADOKStone::ADOKStone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(3.0f);
	Collision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = Collision;

	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->bShouldBounce = true;
	Movement->Bounciness = 0.3f;

	// Clean up stones that are never picked up
	InitialLifeSpan = 8.0f;

}

// Called when the game starts or when spawned
void ADOKStone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADOKStone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADOKStone::Launch(const FVector& Velocity)
{
	// Don't collide with the person who fired it
	Collision->IgnoreActorWhenMoving(GetInstigator(), true);
	Movement->Velocity = Velocity;
}
