#include "Characters/MainCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "items/Money.h"
#include "items/Data/ItemsData.h"
#include "Kismet/KismetSystemLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CurrentGate = ECharacterGate::ECG_Jogging;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 65.0f, 45.0f);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("SlashCharacter"));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MainCharaMappingContext, 0);
		}
	}
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::BeginOverlap);
}

void AMainCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMoney* MyTargetActor = Cast<AMoney>(OtherActor))
	{
		FName RowName = MyTargetActor->MoneyData.RowName;
		
		FMoneyData* RowData = MyTargetActor->MoneyData.GetRow<FMoneyData>(RowName.ToString());
		InventoryComponent->MoneyAmount += RowData->Amount;

		MyTargetActor->Destroy();
	}
}

float AMainCharacter::GetGroundDistance_Implementation()
{
	const FVector ActorLocation = GetActorLocation();
    const FVector CapsuleHalfHeight(0.f, 0.f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

    const FVector StartLocation = ActorLocation - CapsuleHalfHeight;
    const FVector EndLocation = ActorLocation - FVector(0.f, 0.f, 1000.f);

    FHitResult HitResult;
    const TArray<AActor*> ActorToIgnore;

    const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
    	GetWorld(),
    	StartLocation,
    	EndLocation,
    	0.f,
    	ETraceTypeQuery::TraceTypeQuery1,
    	false,
    	ActorToIgnore,
    	EDrawDebugTrace::None,
    	HitResult,
    	true
    	);

    if (bHit) return HitResult.Distance;
    return 1000.f;
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator ControllerRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);
	const FRotator YawRollRotation(0.f, ControllerRotation.Yaw, ControllerRotation.Roll);

	//Move Right-Left ->
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	//Move Forward-Backward ->
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AMainCharacter::BlockStart(const FInputActionValue& Value)
{
	if (CurrentGate != ECharacterGate::ECG_Crouch)
	{
		CurrentGate = ECharacterGate::ECG_Walking;
		UE_LOG(LogTemp, Display, TEXT("Walking"));
		Execute_GetCurrentGate(this, ECharacterGate::ECG_Walking);
		ChangeSpeed(CurrentGate);
	}
}

void AMainCharacter::BlockEnd(const FInputActionValue& Value)
{
	if (CurrentGate != ECharacterGate::ECG_Crouch)
	{
		CurrentGate = ECharacterGate::ECG_Jogging;
		UE_LOG(LogTemp, Display, TEXT("Jogging"));
		Execute_GetCurrentGate(this, ECharacterGate::ECG_Jogging);
		ChangeSpeed(CurrentGate);
	}
}

void AMainCharacter::ChangeSpeed(const ECharacterGate Gate)
{
	GetCharacterMovement()->MaxWalkSpeed = GateSettings[Gate].MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = GateSettings[Gate].MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = GateSettings[Gate].BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = GateSettings[Gate].BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = GateSettings[Gate].BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettings[Gate].UseSeperateBrakingFriction;
}

void AMainCharacter::Crouching(const FInputActionValue& Value)
{
	if ((CurrentGate != ECharacterGate::ECG_Crouch) && CanCrouch() && !bIsCrouching)
	{
		UE_LOG(LogTemp, Display, TEXT("Crouched"));
		bIsCrouching = true;
		CurrentGate = ECharacterGate::ECG_Crouch;
		Execute_GetCurrentGate(this, ECharacterGate::ECG_Crouch);
		Crouch();
	}
	else if (CurrentGate == ECharacterGate::ECG_Crouch && bIsCrouching)
	{
		UE_LOG(LogTemp, Display, TEXT("Un-Crouched"));
		bIsCrouching = false;
		CurrentGate = ECharacterGate::ECG_Jogging;
		Execute_GetCurrentGate(this, ECharacterGate::ECG_Jogging);
		UnCrouch();
	}
	ChangeSpeed(CurrentGate);
}

void AMainCharacter::Dash(const FInputActionValue& Value)
{
	const FRotator ControlerRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, ControlerRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddActorWorldOffset(ForwardDirection*500, true);
}

void AMainCharacter::JumpStart(const FInputActionValue& Value)
{
	Jump();
}

void AMainCharacter::JumpStop(const FInputActionValue& Value)
{
	StopJumping();
}

void AMainCharacter::SwitchWeapon(const FInputActionValue& Value)
{
	float Axis1DValue = Value.Get<float>();
	FString IntegerString = FString::FromInt(Axis1DValue);
	UE_LOG(LogTemp, Warning, TEXT("Weapon -> %s"), *IntegerString)
	
	if ( Axis1DValue == 1.f)
	{
		CharacterStance = ECharacterStance::ECS_Unarmed;
		Execute_GetEquippedWeapon(this, ECharacterStance::ECS_Unarmed);
	}
	else if ( Axis1DValue == 2.f)
	{
		CharacterStance = ECharacterStance::ECS_Pistol;
		Execute_GetEquippedWeapon(this, ECharacterStance::ECS_Pistol);
	}
	else if ( Axis1DValue == 3.f)
	{
		CharacterStance = ECharacterStance::ECS_Rifle;
		Execute_GetEquippedWeapon(this, ECharacterStance::ECS_Rifle);
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
		
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &AMainCharacter::BlockStart);
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AMainCharacter::BlockEnd);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMainCharacter::Crouching);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AMainCharacter::Dash);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainCharacter::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainCharacter::JumpStop);
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Started, this, &AMainCharacter::SwitchWeapon);
	}
}
