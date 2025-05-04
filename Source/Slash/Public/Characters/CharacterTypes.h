#pragma once

UENUM(BlueprintType)
enum class ECharacterGate : uint8
{
	ECG_Walking UMETA(DisplayName = "Walking"),
	ECG_Jogging UMETA(DisplayName = "Jogging"),
	ECG_Crouch UMETA(DisplayName = "Crouch"),
};

UENUM(BlueprintType)
enum class ECharacterStance : uint8
{
	ECS_Unarmed UMETA(DisplayName = "Unarmed"),
	ECS_Pistol UMETA(DisplayName = "Pistol"),
	ECS_Rifle UMETA(DisplayName = "Rifle"),
};

UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	ELD_Forward UMETA(DisplayName = "Forward"),
	ELD_Backward UMETA(DisplayName = "Backward"),
	ELD_Left UMETA(DisplayName = "Left"),
	ELD_Right UMETA(DisplayName = "Right"),
};


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
	ECS_Rolling UMETA(DisplayName = "Rolling")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),

	EDP_Max UMETA(DisplayName = "DefaultMax")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),

	EES_NoState UMETA(DisplayName = "NoState")
};