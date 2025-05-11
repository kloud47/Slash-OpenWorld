// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/MainCharacter.h"
#include "HUD/Inventory/Inventory.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MoneyAmount = 0;
	InventoryAction = nullptr;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* Owner = Cast<AMainCharacter>(GetOwner()))
	{
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Owner->InputComponent))
		{
			Input->BindAction(InventoryAction, ETriggerEvent::Started, this, &UInventoryComponent::OpenInventory);
		}	
	}
}

void UInventoryComponent::OpenInventory(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Open Inventory"));
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetNetOwningPlayer()->PlayerController))
	{
		if (InventoryMenu)
		{
			PC->SetShowMouseCursor(true);
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(InventoryMenu->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PC->SetInputMode(InputMode);
			InventoryMenu->AddToViewport();
		} else
		{
			InventoryMenu = CreateWidget<UInventory>(PC, InventoryClass);
			if (InventoryMenu)
			{
				PC->SetShowMouseCursor(true);
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(InventoryMenu->TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				InventoryMenu->AddToViewport();
			}
			
		}
	}
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

