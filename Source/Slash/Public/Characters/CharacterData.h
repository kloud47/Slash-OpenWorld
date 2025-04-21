// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterData.generated.h"

USTRUCT(BlueprintType)
struct SLASH_API FCharacterData
{
	GENERATED_BODY()

	FCharacterData();
	FCharacterData(float MaxWalkSpeed, float MaxAcceleration, float BrakingDeceleration, float BrakingFriction, bool UseSeperateBrakingFriction)
		: MaxWalkSpeed(MaxWalkSpeed), MaxAcceleration(MaxAcceleration), BrakingDeceleration(BrakingDeceleration), BrakingFriction(UseSeperateBrakingFriction) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFriction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseSeperateBrakingFriction;
};
