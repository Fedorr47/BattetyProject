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
	const FTransform& GetAttachTransform();
	
	UFUNCTION(BlueprintCallable, Category = "CorridorTile")
	void SpawnCorridorTile(const FTransform& inTransform);

	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* firstComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetWorldToSpawn(UWorld* InWorldToSpawn) { WorldToSpawn = InWorldToSpawn; }

private:

	class ABattetyProjectGameMode* BattetyProjectGameMode;

	USceneComponent* RootSceneComponent;

	UWorld* WorldToSpawn = nullptr;
};
