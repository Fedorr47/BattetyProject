// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTETYPROJECT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void CustomDestroy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** True when the pickup can be used, and false when pickup is deactivated*/
	bool bIsActive = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Return the mesh for the pickup */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	/** Return wheter or not the pickup is active */
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	/** Allows other classes to safely change whether or not pickup is active */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool PickupState);

	/** Function to call when the pickup is collected */
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

private:
	/** Static mesh to represent the pickup in the level */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

};
