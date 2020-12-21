// Fill out your copyright notice in the Description page of Project Settings.


#include "CorridorTile.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "../BattetyProjectGameMode.h"
#include "../BattetyProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACorridorTile::ACorridorTile() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootSceneComponent;

	
	ptrAttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachPoint"));
	ptrAttachPoint->SetupAttachment(RootComponent);


	FVector RootRelativeLocation = RootComponent->GetRelativeLocation();

	RootRelativeLocation.X += 1000;
	ptrAttachPoint->SetRelativeLocation(RootRelativeLocation);
	FLinearColor attachColor(255, 0, 0, 0);
	ptrAttachPoint->SetBoundsScale(5.0f);
	ptrAttachPoint->SetArrowColor(attachColor);

	CorridorTileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CorridorTileCollision"));
	CorridorTileCollision->SetupAttachment(RootComponent);
	CorridorTileCollision->OnComponentBeginOverlap.AddDynamic(this, &ACorridorTile::OnOverlapBegin);
	CorridorTileCollision->SetRelativeLocation(RootRelativeLocation);
}

void ACorridorTile::OnOverlapBegin(UPrimitiveComponent* firstComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (BattetyProjectGameMode && BattetyProjectGameMode->GetCurrentPlayer() == Cast<ABattetyProjectCharacter>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, TEXT("overlap !"));
		CorridorTileCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACorridorTile::OnOverlapBegin);
		// change to a delegate to avoid strong dependency (also delete ABattetyProjectGameMode object)
		Cast<ABattetyProjectGameMode>(UGameplayStatics::GetGameMode(this))->AddCoridorTile();
	}
}

// Called when the game starts or when spawned
void ACorridorTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACorridorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const FTransform& ACorridorTile::GetAttachTransform()
{
	return ptrAttachPoint->GetComponentTransform(); 
}

void ACorridorTile::SpawnCorridorTile(const FTransform& inTransform)
{
	if (WorldToSpawn && CorridorBlueprint) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(this);
		WorldToSpawn->SpawnActor<ACorridorTile>(CorridorBlueprint, inTransform, SpawnParams);
	}
}




