// Fill out your copyright notice in the Description page of Project Settings.


#include "items/Eatable.h"

#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AEatable::AEatable()
{
	ItemState = EItemState::EIS_NotHovering;
	
}

void AEatable::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* Chara = Cast<AMainCharacter>(OtherActor);
	if (Chara)// check if slash character is overlapping:
	{
		if (PickUpSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickUpSound,
				GetActorLocation()
			);
		}
		Destroy();
	}
}
