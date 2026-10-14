#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DOKGameMode.generated.h"

UCLASS()
class DOK2_API ADOKGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected: virtual void BeginPlay() override;
	
};
