#include "Components/AttributesComponent.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributesComponent::RecieveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributesComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool UAttributesComponent::IsAlive()
{
	return Health > 0.f;
}


void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

