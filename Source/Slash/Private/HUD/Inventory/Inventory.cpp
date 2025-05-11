// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/Inventory.h"

void UInventory::CloseInventory(FName KeyName)
{
	if (KeyName == FKey(FName("I")))
	{
		RemoveFromParent();
		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->SetShowMouseCursor(false);
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
	}
}
