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
		SpawnCorridorTile(GetWorld());
	}
}

// Called when the game starts or when spawned
void ACorridorTile::BeginPlay()
{
	Super::BeginPlay();

	BattetyProjectGameMode = Cast<ABattetyProjectGameMode>(UGameplayStatics::GetGameMode(this));
	if (BattetyProjectGameMode)
	{
		BattetyProjectGameMode->AddCoridorTile(this);
	}
}

// Called every frame
void ACorridorTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FTransform ACorridorTile::GetAttachTransform()
{
	if (ptrAttachPoint)
	{
		return ptrAttachPoint->GetComponentTransform(); 
	}
	return FTransform();
}

void ACorridorTile::SpawnCorridorTile(UWorld* world)
{
	if (world && CorridorBlueprint) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(this);
		CorridorTileCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACorridorTile::OnOverlapBegin);
		ACorridorTile* nextTile = world->SpawnActor<ACorridorTile>(CorridorBlueprint, GetAttachTransform(), SpawnParams);
	}
}

