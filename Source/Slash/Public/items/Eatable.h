// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemsData.h"
#include "items/Item.h"
#include "Eatable.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API AEatable : public AItem
{
	GENERATED_BODY()
public:
	AEatable();
	
	UPROPERTY(EditAnywhere, Category = "Data", BlueprintReadWrite)
	FSlotData SlotData;
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
private:
	UPROPERTY(EditAnywhere, Category = Sound)
	USoundBase* PickUpSound;
};
