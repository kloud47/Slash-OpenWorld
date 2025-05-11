// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Money.h"

// Sets default values
AMoney::AMoney()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentRotation = FRotator::ZeroRotator;

	MoneyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MoneyMesh");
	MoneyMesh->SetupAttachment(GetRootComponent());

	MoneyMesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
}


// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make Item Rotate:
	float RotationAmount = RotationSpeed * DeltaTime;
	CurrentRotation.Yaw += RotationAmount;
	SetActorRotation(CurrentRotation);
}
