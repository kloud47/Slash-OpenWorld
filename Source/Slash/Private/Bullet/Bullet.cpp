// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet/Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->InitialSpeed = 2500.0f;
	ProjectileMovement->ProjectileGravityScale = 0.02f;
	ProjectileMovement->Bounciness = 0.2f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(GetRootComponent());
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}
