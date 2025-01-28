#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

private:

	UPROPERTY(EditAnywhere)
	//UClass* TreasureClass;
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	// safegaurd to only call it one time:
	bool Broken = false;
};
