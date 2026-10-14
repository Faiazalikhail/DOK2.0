#include "DOKGameMode.h"
#include "Engine/Engine.h"

void ADOKGameMode::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("This Proves that the Setup works and we are good to go"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, TEXT("This Proves that the Setup works and we are good to go"));
	}
}
