// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CorridorTile.generated.h"


UCLASS()
class BATTETYPROJECT_API ACorridorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit ACorridorTile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CorridorTile", Meta = (BlueprintProrected = "true"))
	TSubclassOf<class ACorridorTile> CorridorBlueprint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CorridorTile")
	class UArrowComponent* ptrAttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CorridorTile")
	class UBoxComponent* CorridorTileCollision;
	
	UFUNCTION(BlueprintCallable, Category = "CorridorTile")
	AActor* SpawnCorridorTile(FVector InLocation);

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* firstComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	AActor* GetSpawenedCT() { return SpawnedCoridorTile; }

	void SetWorldToSpawn(UWorld* InWorldToSpawn) { WorldToSpawn = InWorldToSpawn; }

	const FVector GetAttachRelativeLocattion();
	const FVector GetAttachComponentLocation();
	void  SetCorridorTileLocation(FVector InLocation);

private:

	class ABattetyProjectGameMode* BattetyProjectGameMode;

	USceneComponent* RootSceneComponent;

	UWorld* WorldToSpawn = nullptr;

	AActor* SpawnedCoridorTile = nullptr;
};
