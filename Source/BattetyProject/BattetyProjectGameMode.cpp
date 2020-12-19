// Copyright Epic Games, Inc. All Rights Reserved.

#include "BattetyProjectGameMode.h"
#include "BattetyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "CorridorTile.h"


ABattetyProjectGameMode::ABattetyProjectGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	// base decay rate
	DecayRate = 0.01f;
}

void ABattetyProjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ABattetyProjectCharacter* character = Cast<ABattetyProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (character)
	{
		float characterPower = character->GetCurrentPower();
		if (characterPower >= PowerToWin)
		{
			SetCurrentPlayState(EBatteryPlayState::EWon);
		}
		else if (characterPower > 0.0f && CurrentState != EBatteryPlayState::EPause)
		{
			character->UpdatePower(-DeltaTime * DecayRate * (character->GetInitialPower()));
		}
		else if(characterPower <= 0.0f)
		{
			SetCurrentPlayState(EBatteryPlayState::EGameOver);
		}
	}
}

void ABattetyProjectGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all spawn volume actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentPlayState(EBatteryPlayState::EPlaying);

	currentPlayer = Cast<ABattetyProjectCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (currentPlayer)
	{
		PowerToWin = (currentPlayer->GetInitialPower()) * 1.25f;
	}
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
	if (PauseMenuWidgetClass)
	{
		PauseWidget = CreateWidget(GetWorld(), PauseMenuWidgetClass);
	}

	for (int i = 0; i <= NumberOfCorridorTiles; ++i)
	{
		SpawnNextCoridorTile();
	}
}

void ABattetyProjectGameMode::PauseGame()
{
	CurrentState = EBatteryPlayState::EPause;
	HandleNewState(CurrentState);
}

void ABattetyProjectGameMode::ResumeGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	UGameplayStatics::SetGamePaused(this, false);
	PauseWidget->RemoveFromViewport();
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	PlayerController->bShowMouseCursor = false;

	CurrentState = EBatteryPlayState::EPlaying;
}

void ABattetyProjectGameMode::SpawnNextCoridorTile()
{
	if (TCorridorTile.Num() > 0)
	{
		ACorridorTile* lastCorridorTile = TCorridorTile.Last();
		lastCorridorTile->SpawnCorridorTile(GetWorld());
	}
	else
	{
		ACorridorTile* InitialCorridorTile = NewObject<ACorridorTile>(CorridorBlueprint);
		//InitialCorridorTile->SetActorLocation(FVector(-80, 0, 0));
		InitialCorridorTile->CorridorBlueprint = CorridorBlueprint;
		InitialCorridorTile->SpawnCorridorTile(GetWorld());
	}
}

void ABattetyProjectGameMode::AddCoridorTile(ACorridorTile* previousCoridorTile)
{
	if (TCorridorTile.Num() > NumberOfCorridorTiles)
	{
		TCorridorTile.RemoveAt(0);
		TCorridorTile.Shrink();
	}
	TCorridorTile.AddUnique(previousCoridorTile);
}

ABattetyProjectCharacter* ABattetyProjectGameMode::GetCurrentPlayer()
{
	if (currentPlayer)
	{
		return currentPlayer;
	}
	return nullptr;
}

void ABattetyProjectGameMode::SetCurrentPlayState(EBatteryPlayState NewState)
{
	CurrentState = NewState; 
	HandleNewState(CurrentState);
}

void ABattetyProjectGameMode::HandleNewState(EBatteryPlayState NewState)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	switch (NewState)
	{
		case EBatteryPlayState::EPlaying:
		{
			for (ASpawnVolume* SpawnVolumeActor : SpawnVolumeActors)
			{
				SpawnVolumeActor->SetSpawningActive(true);
			}
		}
		break;
		case EBatteryPlayState::EWon:
		{
			for (ASpawnVolume* SpawnVolumeActor : SpawnVolumeActors)
			{
				SpawnVolumeActor->SetSpawningActive(false);
				SpawnVolumeActor->DestroyAllSpawnObjects();
			}
			if (PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, false);
			}
			if (currentPlayer)
			{
				currentPlayer->GetMovementComponent()->MovementState.bCanJump = false;
			}
		}
		break;
		case EBatteryPlayState::EGameOver:
		{
			for (ASpawnVolume* SpawnVolumeActor : SpawnVolumeActors)
			{
				SpawnVolumeActor->SetSpawningActive(false);
				SpawnVolumeActor->DestroyAllSpawnObjects();
			}
			
			if (PlayerController)
			{
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}	
			if (currentPlayer)
			{
				currentPlayer->GetMesh()->SetSimulatePhysics(true);
				currentPlayer->GetMovementComponent()->MovementState.bCanJump = false;
			}
		}
		break;
		case EBatteryPlayState::EPause:
		{
			if (PauseWidget != nullptr)
			{
				UGameplayStatics::SetGamePaused(this, true);
				PauseWidget->AddToViewport();
				UWidgetBlueprintLibrary::SetInputMode_UIOnly(PlayerController, PauseWidget, false);
				PlayerController->bShowMouseCursor = true;
			}
		}
		break;
		case EBatteryPlayState::EUnknown:
		{

		}
		break;
		default:
		break;
	}
}