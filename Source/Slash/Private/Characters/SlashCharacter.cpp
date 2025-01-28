#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "items/Weapons/Weapon.h"
#include "items/Item.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 400.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	ViewCamera->AddRelativeLocation(FVector(0.f, 0.f, 150.f));
	ViewCamera->AddRelativeRotation(FRotator(-10.f, 0.f, 0.f));
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("SlashCharacter"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	GetCharacterMovement()->MaxAcceleration = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	if (!this->GetCharacterMovement()->IsFalling() && CharaState != ECharacterState::ESC_Rolling)
	{
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);// We get the forward vector;
		AddMovementInput(ForwardDirection, MovementVector.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Sprint(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Left shift"));
	if (CharaState == ECharacterState::ECS_Unequipped)
	{
		GetCharacterMovement()->MaxAcceleration = 900.f;
		GetCharacterMovement()->MaxWalkSpeed = 700.f;
	}
	else if (CharaState == ECharacterState::ECS_EquippedOneHandedWeapon)
	{
		GetCharacterMovement()->MaxAcceleration = 900.f;
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
	}
}

void ASlashCharacter::Equip(const FInputActionValue& Value)
{
	//if (this->GetCharacterMovement()->IsFalling()) return;
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon && !EquippedWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		CharaState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipUnequipMontage(FName("Unequip"));
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Disarm"));
			CharaState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquipUnequipMontage(FName("Equip"));
			CharaState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ASlashCharacter::NormAttack(const FInputActionValue& Value)
{
	float GroundSpeed = UKismetMathLibrary::VSizeXY(this->GetCharacterMovement()->Velocity);
	if (CanAttack() && (GroundSpeed > 200.f))
	{
		PlayForwardattackMontage();
	}
	else if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::Roll(const FInputActionValue& Value)
{
	const FVector Velocity = GetCharacterMovement()->GetLastUpdateVelocity();
	float VelocityLength = Velocity.Size();
	ECharacterState temp = CharaState;
	CharaState = ECharacterState::ESC_Rolling;
	if (VelocityLength > 1.f)
	{
		FRotator lastMovementRotation = GetLastMovementInputVector().Rotation();
		SetActorRotation(FRotator(0.f, lastMovementRotation.Yaw, 0.f));
		PlayRollMontage();
	}
	CharaState = temp;
}

void ASlashCharacter::PlayRollMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && RollMontage)
	{
		AnimInstance->Montage_Play(RollMontage);
	}
}

void ASlashCharacter::PlayForwardattackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ForwardAttackMontage)
	{
		AnimInstance->Montage_Play(ForwardAttackMontage);
	}
}

void ASlashCharacter::PlayEquipUnequipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipUnequipMontage)
	{
		AnimInstance->Montage_Play(EquipUnequipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipUnequipMontage);
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && CharaState == ECharacterState::ECS_EquippedOneHandedWeapon;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharaState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharaState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
		//EquippedWeapon = nullptr;

	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASlashCharacter::Roll);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Sprint);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &ASlashCharacter::Equip);
		EnhancedInputComponent->BindAction(NormAttackAction, ETriggerEvent::Started, this, &ASlashCharacter::NormAttack);
	}
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	Super::GetHit_Implementation(ImpactPoint);

	ActionState = EActionState::EAS_HitReaction;
}


