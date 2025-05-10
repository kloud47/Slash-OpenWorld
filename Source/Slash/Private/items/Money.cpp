// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Money.h"

// Sets default values
AMoney::AMoney()
{
	PrimaryActorTick.bCanEverTick = false;

	MoneyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MoneyMesh");
	MoneyMesh->SetupAttachment(GetRootComponent());

	MoneyMesh->SetCollisionProfileName(FName("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
}

