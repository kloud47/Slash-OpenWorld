#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/AttributesComponent.h"
#include "HUD/HealthBarComponent.h"
#include "items/Weapons/Weapon.h"



AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;	
	PawnSensing->SetPeripheralVisionAngle(75.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// OnSeePawn in PawnSensing:
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	InitializeEnemy();

	Tags.Add(FName("Enemy"));
}

void AEnemy::SpawnedDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* SpawnedWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		if (SpawnedWeapon) SpawnedWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon = SpawnedWeapon;
	}
}

// Function called after the timer finished:
void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget, 125.f);
}

void AEnemy::HideHealthBar()
{
	if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget) HealthBarWidget->SetVisibility(true);
}

void AEnemy::LooseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	MoveToTarget(PatrolTarget, PatrollingSpeed);
}

void AEnemy::StartChasing()
{
	EnemyState = EEnemyState::EES_Chasing;
	MoveToTarget(CombatTarget, ChasingSpeed);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}



void AEnemy::MoveToTarget(AActor* Target, float Speed)
{
	if (EnemyController == nullptr && Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(55.f);
	//FNavPathSharedPtr NavPath;
	//EnemyController->MoveTo(MoveRequest, &NavPath);
	EnemyController->MoveTo(MoveRequest);
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	//if (NavPath.IsValid())
	//{
		//const TArray<FNavPathPoint>& PathPoints = NavPath->GetPathPoints();
		//for (auto& Point : PathPoints)
		//{
			//const FVector& Location = Point.Location;
			//DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		//}
	//}
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (auto patrol : PatrolTargets)
	{
		if (patrol != PatrolTarget)
		{
			ValidTargets.AddUnique(patrol);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}

	return nullptr;
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	return IsInsideAttackRadius() && !IsAttacking() && !IsDead() && !IsEngaged();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Select = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> Pose(Select);
	if (Pose < EDeathPose::EDP_Max)
	{
		DeathPose = Pose;
	}
	return Select;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("SlashCharacter"));
	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		StartChasing();
	}
}

void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	PlayDeathMontage();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsuleCollision();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetEnableWeaponCollison(ECollisionEnabled::NoCollision);
}



bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	//DRAW_SPHERE_SINGLEFRAME(GetActorLocation());
	//DRAW_SPHERE_SINGLEFRAME(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else {
		CheckPatrolTarget();
	}
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	SpawnedDefaultWeapon();
	HideHealthBar();
	MoveToTarget(PatrolTarget, 125.f);
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float TimerLength = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, TimerLength);
		// Setting timer before moving to Target.
	}
}

void AEnemy::CheckCombatTarget()
{

	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LooseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() 
		&& !IsChasing()
		)
	{
		ClearAttackTimer();
	    if (!IsEngaged()) StartChasing();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	StartChasing();

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		//EquippedWeapon->Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		EquippedWeapon->Destroy();
	}
}

