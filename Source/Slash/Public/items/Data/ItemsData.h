#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemsData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_Mele UMETA(DisplayName = "Mele", Description = "Mele Weapons"),
	EIT_Range UMETA(DisplayName = "Range", Description = "Range Weapons"),
	EIT_Eatable UMETA(DisplayName = "Eatable", Description = "Eatable Items for Recovery and Mana"),
};
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
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> MoneyMesh;
};

USTRUCT(BlueprintType)
struct SLASH_API FItemsData : public FTableRowBase
{
	GENERATED_BODY()

	FItemsData();
	FItemsData(FText Name, FText Desc, TObjectPtr<UTexture2D> Thumbnail, int32 StackSize, float Power, TObjectPtr<UStaticMesh> Mesh)
		: Name(Name), Description(Desc), Thumbnail(Thumbnail), StackSize(StackSize), Power(Power), Mesh(Mesh) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> Mesh;
};

// USTRUCT(BlueprintType)
// struct SLASH_API FSlotData
// {
// 	GENERATED_BODY()
//
// 	FSlotData();
// 	FSlotData(FDataTableRowHandle ItemID, int32 Quantity, EItemType ItemType)
// 		: ItemID(ItemID), Quantity(Quantity), ItemType(ItemType) {};
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FDataTableRowHandle ItemID;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	int32 Quantity;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	EItemType ItemType;
// };