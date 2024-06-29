// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> LookAction;

	
	void Look(const struct FInputActionValue& Value);
protected:
	virtual void SetupInputComponent() override;


public:
	AChessPlayerController();
	
};
