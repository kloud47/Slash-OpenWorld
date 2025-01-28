#include "items/Treasure.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)// check if slash character is overlapping:
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
