// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/Inventory.h"

FReply UInventory::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == FKey(FName("I")))
	{
		RemoveFromParent();
		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->SetShowMouseCursor(false);
			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
		}
		return FReply::Handled();
	}

	return FReply::Unhandled();
}
