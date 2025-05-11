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

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess = true))
	UInputAction* GrabAction;

public:	
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite)
	float MoneyAmount;

protected:
	virtual void BeginPlay() override;

	void OpenInventory(const FInputActionValue& Value);
	void GrabItem(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	FHitResult TraceToPickUp();

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UInventory> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> InventoryClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> GrabWidget;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> GrabWidgetClass;
	

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};