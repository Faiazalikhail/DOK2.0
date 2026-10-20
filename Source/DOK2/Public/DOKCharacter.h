#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DOKCharacter.generated.h"

class UCameraComponent;

UCLASS()
class DOK2_API ADOKCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADOKCharacter();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ArmsMesh;
};