#include "Characters/MainCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CurrentGate = ECharacterGate::ECG_Jogging;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 65.0f, 45.0f);
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom);
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
	CurrentGate = ECharacterGate::ECG_Walking;
	GetCharacterMovement()->MaxWalkSpeed = GateSettings[ECharacterGate::ECG_Walking].MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = GateSettings[ECharacterGate::ECG_Walking].MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = GateSettings[ECharacterGate::ECG_Walking].BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = GateSettings[ECharacterGate::ECG_Walking].BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = GateSettings[ECharacterGate::ECG_Walking].BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettings[ECharacterGate::ECG_Walking].UseSeperateBrakingFriction;
}

void AMainCharacter::BlockEnd(const FInputActionValue& Value)
{
	CurrentGate = ECharacterGate::ECG_Jogging;
	GetCharacterMovement()->MaxWalkSpeed = GateSettings[ECharacterGate::ECG_Jogging].MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = GateSettings[ECharacterGate::ECG_Jogging].MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = GateSettings[ECharacterGate::ECG_Jogging].BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = GateSettings[ECharacterGate::ECG_Jogging].BrakingFrictionFactor;
	GetCharacterMovement()->BrakingFriction = GateSettings[ECharacterGate::ECG_Jogging].BrakingFriction;
	GetCharacterMovement()->bUseSeparateBrakingFriction = GateSettings[ECharacterGate::ECG_Jogging].UseSeperateBrakingFriction;
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
		
	}
}

