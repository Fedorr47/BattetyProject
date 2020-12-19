// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuBase.h"
#include "UObject/UObjectGlobals.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Kismet/GameplayStatics.h"


void UMainMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (BtStartGame)
	{
		BtStartGame->OnPressed.AddDynamic(this, &UMainMenuBase::FnStartGame);
	}
	if (BtExit)
	{
		BtExit->OnPressed.AddDynamic(this, &UMainMenuBase::FnExitGame);
	}
	if (BtOptions)
	{
		BtOptions->OnPressed.AddDynamic(this, &UMainMenuBase::FnOptions);
	}
}

UMainMenuBase::UMainMenuBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMainMenuBase::FnStartGame()
{
	UGameplayStatics::OpenLevel(this, "BatteryArena");
	RemoveFromParent();
}

void UMainMenuBase::FnExitGame()
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this, playerController, EQuitPreference::Quit, true);
}

void UMainMenuBase::FnOptions()
{

}

//TSharedRef<SWidget> UMainMenuBase::RebuildWidget()
//{
//    if (WidgetTree)
//    {
//        RootPanelWidget = Cast<UPanelWidget>(GetRootWidget());
//        if (RootPanelWidget == nullptr)
//        {
//            RootPanelWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), FName("RootPanelWidget"));
//
//            UCanvasPanelSlot* RootSlot = Cast<UCanvasPanelSlot>(RootPanelWidget->Slot);
//            if (RootSlot)
//            {
//                RootSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
//                RootSlot->SetOffsets(FMargin(0.f, 0.f));
//            }
//
//            WidgetTree->RootWidget = RootPanelWidget;
//        }
//    }
//
//    TSharedRef<SWidget> Widget = Super::RebuildWidget();
//
//    UTextBlock* BtStartGameText = NewObject<UTextBlock>();
//    BtStartGameText->SetText(FText::FromString("Start Game"));
//
//    
//    BtStartGame = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName("ButtonStartGame"));
//    BtStartGame->SetContent(BtStartGameText);
//    RootPanelWidget->AddChild(BtStartGame);
//    UCanvasPanelSlot* BtStartGameSlot = Cast<UCanvasPanelSlot>(BtStartGame->Slot);
//    BtStartGameSlot->SetAnchors(FAnchors(.5f, .5f, .5f, .5f));
//    BtStartGameSlot->SetSize(FVector2D(300.f, 180.f));
//    BtStartGameSlot->SetAlignment(FVector2D(.5f, .5f));
//    BtStartGame->OnClicked.AddDynamic(this, &UMainMenuBase::FnStartGame);
//
//    return Widget;
//}
