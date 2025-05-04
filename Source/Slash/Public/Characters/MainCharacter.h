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
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetEquippedWeapon(ECharacterStance Stance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetGroundDistance();  

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

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	/*
	 * Input Callback:
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void BlockStart(const FInputActionValue& Value);
	void BlockEnd(const FInputActionValue& Value);
	void ChangeSpeed(const ECharacterGate);
	void Crouching(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void JumpStart(const FInputActionValue& Value);
	void JumpStop(const FInputActionValue& Value);

	/* Movement Settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Settings")
	ECharacterGate CurrentGate;
	
	UPROPERTY(EditAnywhere, Category="Movement Settings")
	TMap<ECharacterGate, FCharacterData> GateSettings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	bool bIsCrouching = false;
};
