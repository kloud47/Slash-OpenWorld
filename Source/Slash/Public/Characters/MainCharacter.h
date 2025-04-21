#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterData.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/CharacterDataInterface.h"
#include "MainCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;

UCLASS()
class SLASH_API AMainCharacter : public ABaseCharacter, public ICharacterDataInterface
{
	GENERATED_BODY()

public:
	AMainCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintImplementableEvent)
	void GetCurrentGate(ECharacterGate Gate);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MainCharaMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> BlockAction;

	/*
	 * Input Callback:
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void BlockStart(const FInputActionValue& Value);
	void BlockEnd(const FInputActionValue& Value);

	/* Movement Settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Settings")
	ECharacterGate CurrentGate;
	
	UPROPERTY(EditAnywhere, Category="Movement Settings")
	TMap<ECharacterGate, FCharacterData> GateSettings;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;
};
