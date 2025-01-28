#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Attributes Category")
	float Health;// Current Health:
		
	UPROPERTY(EditAnywhere, Category = "Attributes Category")
	float MaxHealth;
public:
	void RecieveDamage(float Damage);
	float GetHealthPercent();
	bool IsAlive();
};
