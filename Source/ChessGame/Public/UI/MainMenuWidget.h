﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class CHESSGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


private:
	virtual void NativeConstruct() override;

	

private:
	UFUNCTION()
	void OnCreateGameButtonClicked();
	UFUNCTION()
	void OnJoinGameButtoClicked();
	UFUNCTION()
	void OnQuitButtonButtonClicked();
	
	
	
	
	
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget),meta=(AllowPrivateAccess))
	TObjectPtr<UButton> CreateGameButton;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget),meta=(AllowPrivateAccess))
	TObjectPtr<UButton> JoinGameButton;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget),meta=(AllowPrivateAccess))
	TObjectPtr<UButton> QuitButton;

	
};