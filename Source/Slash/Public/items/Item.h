// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemTypes.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()// helps in initializinig c++project with UE5 engine :)
	
public:	
	// Sets default values for this actor's properties
	// Called every frame
	AItem();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sine params")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine params")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

	UFUNCTION(BlueprintPure)
	float TransformedCos();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmbersEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;



};
