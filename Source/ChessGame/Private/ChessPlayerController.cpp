// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPlayerController.h"

#include "ChessTile.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "ChessGame/ChessGame.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"

AChessPlayerController::AChessPlayerController()
{
	bReplicates=true;
}



void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor=true;
	
	if (IsLocalController())
	{
		UE_LOGFMT(LogChessGame,Warning,"PC name is {a}",GetName());
		UKismetSystemLibrary::PrintString(this,FString::Printf(TEXT("Begin player %s Controller!"),*GetName()));
	}
	//todo: make this safer
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TileTime,[this]()
		{
			if (bIsAnyTileSelected) return;
			
			
			FHitResult HitResult;

			
			
			if (const bool bClientHit=GetHitResultUnderCursorByChannel(TraceTypeQuery1,false,HitResult))
			{
				for (const auto& Tile : CurrentValidMoves)
				{
					Tile->DisableShader();
				}

				
				HoveredTile=Cast<AChessTile>(HitResult.GetActor());
				
				if (auto* Interface=Cast<IChessPieceInterface>(HoveredTile->GetChessPiece()))
				{
					CurrentValidMoves=Interface->GetValidMoves();
					
					for (const auto& Tile : Interface->GetValidMoves())
					{
						
						Tile->ShowShader();
					}
					
				}
			};
		},1.f/60.f,true);
	}
}





void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
			EnhancedInputComponent->BindAction(ToggleLookAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleLook);
			EnhancedInputComponent->BindAction( SelectAction,ETriggerEvent::Started, this, &ThisClass::Select);
			EnhancedInputComponent->BindAction(MoveBoardAction, ETriggerEvent::Triggered, this, &ThisClass::MoveBoard);
			EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ThisClass::Cancel);
		}
		
	}

	
}

void AChessPlayerController::Cancel(const FInputActionValue& Value)
{
	bIsAnyTileSelected=false;
}

void AChessPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	
	AddPitchInput(LookAxis.Y);
	AddYawInput(LookAxis.X);
}

void AChessPlayerController::ToggleLook(const FInputActionValue& Value)
{
	//this doesnt do anything, its need for chorded action look on Mouse
}
void AChessPlayerController::MoveBoard(const FInputActionValue& Value)
{
	//specific for gamepad
	//todo: imp
}
void AChessPlayerController::Select(const FInputActionValue& Value)
{
	if (!GetCurrentHoveredTile()) return;


	auto* CurrentTile=GetCurrentHoveredTile();
	auto* ChessPieceOnTile=CurrentTile->GetChessPiece();

	bIsAnyTileSelected=true;
	FHitResult HitResult;
	
	auto* Interface=Cast<IChessPieceInterface>(ChessPieceOnTile);

	auto Moves=Interface->GetValidMoves();
	
	if (GetHitResultUnderCursorByChannel(TraceTypeQuery1,false,HitResult))
	{
		if (Moves.Contains(HitResult.GetActor()))
		{
			Interface->MoveTo(Cast<AChessTile>(HitResult.GetActor()));
			bIsAnyTileSelected=false;
		}
	}
	
	
	
}
void AChessPlayerController::Client_TileHit_Implementation(const FHitResult& TileHitResult)
{
	UKismetSystemLibrary::PrintString(this,TEXT("Hit Tile!"));
}
void AChessPlayerController::Server_RequestTile_Implementation()
{
}

