#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterData.h"
#include "CharacterTypes.h"
#include "Interfaces/CharacterDataInterface.h"
#include "MainCharacter.generated.h"

class UInventory;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;

UCLASS()
class SLASH_API AMainCharacter : public ABaseCharacter, public ICharacterDataInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputMappingContext> MainCharaMappingContext;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> SwitchWeaponAction;

	// UPROPERTY(EditAnywhere, Category="Input")
	// TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UInputAction> InventoryAction;

public:
	AMainCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// virtual void GetCurrentGate_Implementation(ECharacterGate Gate) override;
	// virtual void GetEquippedWeapon_Implementation(ECharacterStance Stance) override;
	virtual float GetGroundDistance_Implementation() override;

	// FTimerHandle FireTimerHandle;
	
	UPROPERTY(BlueprintReadWrite)
	float MoneyAmount;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
	void SwitchWeapon(const FInputActionValue& Value);
	void Inventory(const FInputActionValue& Value);
	// void Fire(const FInputActionValue& Value);
	// void FireGun();

	/* Movement Settings */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Settings")
	ECharacterGate CurrentGate;
	
	UPROPERTY(EditAnywhere, Category="Movement Settings")
	TMap<ECharacterGate, FCharacterData> GateSettings;
	
	/* Get Weapon Data */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement Settings")
	ECharacterStance CharacterStance = ECharacterStance::ECS_Unarmed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	TObjectPtr<UInventory> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> InventoryClass;

private:
	bool bIsCrouching = false;
	bool bCanFire = true;
};
