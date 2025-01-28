#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
//#include "Slash/DebugMacros.h"
#include "InputActionValue.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction; 
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Input)
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
};
