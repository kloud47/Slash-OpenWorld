#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;



UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* NormAttackAction;

	/*
		*Input Callbacks:
	*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void NormAttack(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);

	/*
		*Play Montage functions:
	*/
	void PlayEquipUnequipMontage(FName SectionName);
	void PlayRollMontage();
	void PlayForwardattackMontage();
	/* Combat */
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	ECharacterState CharaState = ECharacterState::ECS_Unequipped;

	/* Character Components */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ForwardAttackMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* EquipUnequipMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* RollMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharaState() const { return CharaState; } //const means nothing here can be changed:
};
