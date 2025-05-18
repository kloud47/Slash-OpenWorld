// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Characters/MainCharacter.h"
#include "HUD/Inventory/Inventory.h"
#include "items/Eatable.h"
#include "Kismet/KismetSystemLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoneyAmount = 0;
	InventoryAction = nullptr;
	// CurrentItem = FSlotData();
}


void UInventoryComponent::InitializeInput()
{
	if (AActor* Owner = Cast<AMainCharacter>(GetOwner()))
	{
		if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(Owner->InputComponent))
		{
			Input->BindAction(InventoryAction, ETriggerEvent::Started, this, &UInventoryComponent::OpenInventory);
			Input->BindAction(GrabAction, ETriggerEvent::Started, this, &UInventoryComponent::GrabItem);
		}	
	}
}

void UInventoryComponent::CreateWidgets()
{
	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetNetOwningPlayer()->PlayerController))
	{
		GrabWidget = CreateWidget<UUserWidget>(PC, GrabWidgetClass);
		InventoryMenu = CreateWidget<UInventory>(PC, InventoryClass);
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
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
		} 
	}
}

void UInventoryComponent::GrabItem(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Grab Item"));

	bool bFoundExistingStack = false;
	switch (CurrentItem.ItemType)
	{
		case EItemType::EIT_Mele :
			{
				UE_LOG(LogTemp, Display, TEXT("Mele Item"));
				if (MeleItemSize < MaxMeleItemSize)
				{
					AddItem(CurrentItem, bFoundExistingStack, AllItem.MeleItems, MeleItemSize, MaxMeleItemSize);
				}
				break;
			}
		case EItemType::EIT_Ranged :
			{
				UE_LOG(LogTemp, Display, TEXT("Ranged Item"));
				if (RangedItemSize < MaxRangedItemSize)
				{
					AddItem(CurrentItem, bFoundExistingStack, AllItem.RangedItems, RangedItemSize, MaxRangedItemSize);
				}
				break;
			}
		case EItemType::EIT_Eatable :
			{
				UE_LOG(LogTemp, Display, TEXT("Eatable Item"));
				if (EatableSize < MaxEatableSize)
				{
					AddItem(CurrentItem, bFoundExistingStack, AllItem.EatableItems, EatableSize, MaxEatableSize);
				}
				break;
			}
		default:
			{
				UE_LOG(LogTemp, Display, TEXT("Invalid Item Type"));
			}
	}
}

void UInventoryComponent::AddItem(FSlotData Item, bool &bFoundExistingStack, TArray<FSlotData> &Arr, int32 &ItemIndex, int32 ItemSize)
{
	for (FSlotData& ExistingItem : Arr)
	{
		if (ExistingItem.ItemID.RowName == Item.ItemID.RowName)
		{
			int32 StackSize = Item.ItemID.DataTable->FindRow<FItemsData>(Item.ItemID.RowName, TEXT("Not Found Item"), true)->StackSize;
			if ((ExistingItem.Quantity + Item.Quantity) > StackSize)
			{
				ExistingItem.Quantity = StackSize;
				int32 remainingQty  = (Item.Quantity + ExistingItem.Quantity) - StackSize;
				Item.Quantity = remainingQty;
				Arr[ItemIndex] = Item;
				ItemIndex++;
				bFoundExistingStack = true;
				break;
			} else
			{
				ExistingItem.Quantity += Item.Quantity;
				bFoundExistingStack = true;
				break;
			}
		}
	}

	if (!bFoundExistingStack)
	{
		Arr[ItemIndex] = Item;
		ItemIndex++;
	}
	OtherActor->Destroy();
	UpdateItemsInInventory();
}


void UInventoryComponent::TraceToPickUp()
{
	const AMainCharacter* Owner = Cast<AMainCharacter>(GetOwner());
	FVector start = Owner->Camera->GetComponentLocation();
	FVector end = Owner->Camera->GetForwardVector();
	end = (end * 500.f) + start;

	FHitResult HitResult;
	const TArray<AActor*> ActorToIgnore;
	
	const bool hit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		start,
		end,
		20.f,
		TraceTypeQuery1,
		false,
		ActorToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
		);

	// FSlotData PrevItem = CurrentItem;
	// && PrevItem.ItemID != CurrentItem.ItemID
	AEatable* ItemData = Cast<AEatable>(HitResult.GetActor());
	// bool bCorrectHit = HitResult.GetActor()->IsA<AEatable>();
	
	if (hit && ItemData)
	{
		CurrentItem = ItemData->SlotData;
		OtherActor = ItemData;
		GrabWidget->AddToViewport();
	}
	else  GrabWidget->RemoveFromParent();
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TraceToPickUp();
}

