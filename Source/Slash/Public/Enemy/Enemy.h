#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	/* <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void Destroyed() override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	/* </IHitInterface> */

protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	virtual int32 PlayDeathMontage() override;
	/* </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/* AI Behaviour */
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LooseInterest();
	void StartPatrolling();
	void StartChasing();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target, float Speed);
	AActor* ChoosePatrolTarget();
	void SpawnedDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);// Callback for UPawnSensing component:

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)//For  Spawning Weapon class:
		TSubclassOf<class AWeapon> WeaponClass;

	/* Combat Mechanics */
	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "Combat Radius")
	float CombatRadius = 800.f;

	UPROPERTY(EditAnywhere, Category = "Attack Radius")
	float AttackRadius = 150.f;

	UPROPERTY()
	class AAIController* EnemyController;

	// Current Patrol Targets:
	UPROPERTY(EditAnywhere, Category = "AI Naviagtion")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI Naviagtion")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 8.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;
};
