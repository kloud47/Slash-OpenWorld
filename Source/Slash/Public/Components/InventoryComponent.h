// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

struct FInputActionValue;
class UInputAction;
class UInventory;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess = true))
	UInputAction* InventoryAction;

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(BlueprintReadWrite)
	float MoneyAmount;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenInventory(const FInputActionValue& Value);

	TObjectPtr<UInventory> InventoryMenu;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> InventoryClass;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
		
};
