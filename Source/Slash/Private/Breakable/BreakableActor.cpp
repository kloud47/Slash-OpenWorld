#include "Breakable/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Treasure.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	SetRootComponent(GeometryCollection);
	GeometryCollection->SetGenerateOverlapEvents(true);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(GeometryCollection);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Broken) return;
	Broken = true;
	UWorld* world = GetWorld();
	if (world && TreasureClasses.Num() > 0)
	{
		FVector Location = GetActorLocation();
		Location.Z += 75.f;

		const int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);

		world->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
	}
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}
