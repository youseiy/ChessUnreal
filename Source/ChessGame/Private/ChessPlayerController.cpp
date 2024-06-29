// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

AChessPlayerController::AChessPlayerController()
{
	
}



void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(IMC && LookAction);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		}
		
	}

	
}

void AChessPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();

	
	AddPitchInput(LookAxis.Y);
	AddYawInput(LookAxis.X);
}
