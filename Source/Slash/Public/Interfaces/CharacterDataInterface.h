// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "UObject/Interface.h"
#include "CharacterDataInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SLASH_API ICharacterDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintImplementableEvent)
	void GetCurrentGate(ECharacterGate Gate);

	UFUNCTION(BlueprintImplementableEvent)
	void GetEquippedWeapon(ECharacterStance Stance);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetGroundDistance();
};
