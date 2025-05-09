// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemsData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SLASH_API FMoneyData : public FTableRowBase
{
	GENERATED_BODY()
	
	FMoneyData();
	FMoneyData(FName Name, int Amount, UStaticMesh* Mesh)
		: Name(Name), Amount(Amount), MoneyMesh(Mesh) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> MoneyMesh;
};
