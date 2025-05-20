// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/Inventory.h"


void UInventory::CloseInventory()
{
	RemoveFromParent();
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(false);
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
}
