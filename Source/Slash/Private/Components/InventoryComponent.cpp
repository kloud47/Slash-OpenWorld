// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Characters/MainCharacter.h"
#include "HUD/Inventory/Inventory.h"
#include "Kismet/KismetSystemLibrary.h"

UInventoryComponent::UInventoryComponent()
{
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
			Input->BindAction(GrabAction, ETriggerEvent::Started, this, &UInventoryComponent::GrabItem);
		}	
	}

	if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetNetOwningPlayer()->PlayerController))
	{
		GrabWidget = CreateWidget<UUserWidget>(PC, GrabWidgetClass);
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

void UInventoryComponent::GrabItem(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Grab Item"));
}


FHitResult UInventoryComponent::TraceToPickUp()
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
	return HitResult;
}


void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

