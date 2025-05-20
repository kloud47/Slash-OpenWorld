// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "items/Item.h"
#include "items/ItemTypes.h"
#include "Item2.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem2 : public AItem
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ItemSKMesh;
	
public:	
	AItem2();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
};
