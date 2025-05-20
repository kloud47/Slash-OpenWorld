// Fill out your copyright notice in the Description page of Project Settings.


#include "item2/Item2.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"


AItem2::AItem2()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemSKMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ItemSkeletalMesh");
	RootComponent = ItemSKMesh;
	Sphere->SetupAttachment(ItemSKMesh);
	EmbersEffect->SetupAttachment(ItemSKMesh);

	ItemMesh = nullptr;
}

void AItem2::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItem2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

