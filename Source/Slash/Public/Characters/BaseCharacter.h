#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class UAttributesComponent;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual void Attack();
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	virtual bool CanAttack();
	bool IsAlive();
	/*
		*Play Montage functions:
	*/
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();

	UFUNCTION(BlueprintCallable)
	void SetEnableWeaponCollison(ECollisionEnabled::Type CollisionEnabled);


	void DisableCapsuleCollision();

	UFUNCTION(BlueprintCallable)// use AnimNotify to call AttackEnd():
	virtual void AttackEnd();


	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	/*
	Components:
	*/
	UPROPERTY(VisibleAnywhere)
	UAttributesComponent* Attributes;

private:
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;
	/*
		*Animation Montage*:
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;
};
