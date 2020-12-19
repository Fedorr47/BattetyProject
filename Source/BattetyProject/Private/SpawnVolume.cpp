// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Pickup.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent the spawn volume
	SpawnBox= CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	RootComponent = SpawnBox;

	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 4.5f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/** */
FVector ASpawnVolume::GetRandomPointInVolume()
{
	FVector SpawnOrigin = SpawnBox->Bounds.Origin;
	FVector SpawnExtent = SpawnBox->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

/** */
void ASpawnVolume::SpawnPickup()
{
	if (SpawnObject != NULL)
	{
		UWorld* const ptrWorld = GetWorld();
		if (ptrWorld)
		{
			FActorSpawnParameters spawnParameters;
			spawnParameters.Owner = this;
			spawnParameters.Instigator = GetInstigator();

			FVector spawnLocation = GetRandomPointInVolume();

			FRotator spawnRotation;
			spawnRotation.Yaw = FMath::FRand() * 360.0f;
			spawnRotation.Pitch = FMath::FRand() * 360.0f;
			spawnRotation.Roll = FMath::FRand() * 360.0f;

			APickup* const spawnedPickup = ptrWorld->SpawnActor<APickup>(SpawnObject, spawnLocation, spawnRotation, spawnParameters);
			spawnedActors.AddUnique(spawnedPickup);

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
}

void ASpawnVolume::DestroyAllSpawnObjects()
{
	for (APickup* pickup : spawnedActors)
	{
		if (pickup && !pickup->IsPendingKill())
		{
			pickup->CustomDestroy();
		}
	}
	spawnedActors.Empty();
}
