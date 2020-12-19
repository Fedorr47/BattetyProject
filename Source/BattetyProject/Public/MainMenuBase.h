// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuBase.generated.h"

/**
 * 
 */
UCLASS()
class BATTETYPROJECT_API UMainMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UMainMenuBase(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* BtStartGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BtOptions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BtExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPanelWidget* RootPanelWidget;

	UFUNCTION()
	void FnStartGame();
	UFUNCTION()
	void FnExitGame();
	UFUNCTION()
	void FnOptions();

	//virtual TSharedRef<SWidget> RebuildWidget();
};
