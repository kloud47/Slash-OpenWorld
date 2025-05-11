// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Money.generated.h"

UCLASS()
class SLASH_API AMoney : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MoneyMesh;
	
public:	
	// Sets default values for this actor's properties
	AMoney();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	FDataTableRowHandle MoneyData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 45.0f; // Degrees per second

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
	FRotator CurrentRotation;
};
