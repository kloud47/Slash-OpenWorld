#include "items/Treasure.h"

#include "Characters/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
