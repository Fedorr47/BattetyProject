#include "BatteryPickup.h"

//Set default values
ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);
	batteryPower = 150.f;
}

void ABatteryPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
	Destroy();
}

float ABatteryPickup::GetPower()
{
	return batteryPower;
}

void ABatteryPickup::LightDestroy_Implementation()
{
	Destroy();
}

void ABatteryPickup::CustomDestroy()
{
	LightDestroy();
}