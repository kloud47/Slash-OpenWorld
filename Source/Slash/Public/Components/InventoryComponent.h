// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "items/Data/ItemsData.h"
#include "InventoryComponent.generated.h"

class AEatable;
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

	UPROPERTY()
	AEatable* OtherActor;

	void AddItem(FSlotData Item , bool &bFoundExistingStack, TArray<FSlotData> &Arr, AActor* ItemActor, int32 &ItemIndex, int32 ItemSize);
	
	int EatableSize = 0;
	int MaxEatableSize = 7;
	int MeleItemSize = 0;
	int MaxMeleItemSize = 5;
	int RangedItemSize = 0;
	int MaxRangedItemSize = 3;

public:	
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void InitializeInput();
	
	UFUNCTION(BlueprintCallable)
	void CreateWidgets();

	UPROPERTY(BlueprintReadWrite)
	float MoneyAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAllItemsData AllItem;

	UPROPERTY(BlueprintReadOnly)
	FSlotData CurrentItem;

	UFUNCTION(BlueprintImplementableEvent)
	void PickedUpMoney(int Money);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItemsInInventory();


protected:
	virtual void BeginPlay() override;

	void OpenInventory(const FInputActionValue& Value);
	void GrabItem(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void TraceToPickUp();

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