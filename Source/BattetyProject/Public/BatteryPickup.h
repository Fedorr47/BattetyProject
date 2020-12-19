// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 *
 */
UCLASS()
class BATTETYPROJECT_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

	ABatteryPickup();

	/** Override the WasCollected function*/
	void WasCollected_Implementation() override;

public:
	float GetPower();

	UFUNCTION(BlueprintNativeEvent)
	void LightDestroy();
	void LightDestroy_Implementation();

	void CustomDestroy() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float batteryPower = 0.0f;
};
