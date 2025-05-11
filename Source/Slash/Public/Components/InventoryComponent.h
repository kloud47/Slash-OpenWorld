// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FInputActionValue;
class UInputAction;
class UInventory;

UCLASS(Blueprintable)
class SLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess = true))
	UInputAction* InventoryAction;

public:	
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite)
	float MoneyAmount;

protected:
	virtual void BeginPlay() override;

	void OpenInventory(const FInputActionValue& Value);

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventory> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> InventoryClass;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
