// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattetyProjectGameMode.generated.h"


// enum to store the current state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EPause,
	EUnknown
};

UCLASS(minimalapi)
class ABattetyProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABattetyProjectGameMode();

	virtual void Tick(float DeltaTime) override;

	/** Returns power needed to win - needed for the HUD */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const { return PowerToWin; };

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentPlayState() { return CurrentState; };

	UFUNCTION(BlueprintCallable, Category = "Power")
	void SetCurrentPlayState(EBatteryPlayState NewState);

	UFUNCTION(BlueprintCallable, Category = "BattryGameMode")
	void PauseGame();

	UFUNCTION(BlueprintCallable, Category = "BattryGameMode")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "BattryGameMode")
	void AddCoridorTile();

	UFUNCTION(BlueprintCallable, Category = "BattryGameMode")
	ABattetyProjectCharacter* GetCurrentPlayer();

	void SpawnNextCoridorTile();
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOverlapDelegate, AActor*, OverlappedObject, AActor*, OverlappingObject);
	UPROPERTY(BlueprintAssignable)
	FOnOverlapDelegate OnOverlapEvent;

protected:
	/** The rate at which the character loses power */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProrected = "true"))
	float DecayRate = 0.0f;

	/** The power amount need to win the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProrected = "true"))
	float PowerToWin = 0.0f;

	/** The widget class to use for our HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProrected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BatteryGameMode", Meta = (BlueprintProrected = "true"))
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BatteryGameMode", Meta = (BlueprintProrected = "true"))
	TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	/** The instance  of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

	UPROPERTY()
	UUserWidget* PauseWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CorridorTile", Meta = (BlueprintProrected = "true"))
	int32 NumberOfCorridorTiles = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CorridorTile", Meta = (BlueprintProrected = "true"))
	TSubclassOf<class ACorridorTile> CorridorBlueprint;

	UPROPERTY()
	TArray<class ACorridorTile*> TCorridorTile;

private:

	/** Keeps track of the current playing state */
	EBatteryPlayState CurrentState;

	class ABattetyProjectCharacter* currentPlayer;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	UPROPERTY()
	ACorridorTile* FirstCorridorTile = nullptr;
	UPROPERTY()
	ACorridorTile* LastCorridorTile = nullptr;
	UPROPERTY()
	ACorridorTile* MiddleCorridorTile = nullptr;

	int32 MiddleOfTiles = 0;


	/** Handle any function calls that rely upon changing the playing state of a game */
	void HandleNewState(EBatteryPlayState NewState);
	
	UFUNCTION()
	void OverlapDispatcher(AActor* OverlappedObject, AActor* OverlappingObject);
};