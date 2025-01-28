// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		SlashCharaMovement = SlashCharacter->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// Setting the velocity to use it in animation BP:
	if (SlashCharacter)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(SlashCharaMovement->Velocity);
		IsFalling = SlashCharaMovement->IsFalling();
		CharaState = SlashCharacter->GetCharaState();
	}
}