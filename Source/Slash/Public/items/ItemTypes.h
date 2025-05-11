#pragma once

//UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_NotHovering UMETA(DisplayName = "NotHovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};