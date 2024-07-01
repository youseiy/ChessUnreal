// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPlayerController.h"

#include "ChessBoard.h"
#include "ChessCollisionTypes.h"
#include "ChessGameState.h"
#include "ChessPlayerState.h"
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

void AChessPlayerController::Server_MovePiece_Implementation(AChessPiece* Piece, AChessTile* Tile)
{
	GetWorld()->GetGameState<AChessGameState>()->GetChessBoard()->Server_RequestChessPieceMove(Piece,Tile);
}


void AChessPlayerController::Server_RequestPieceOwnership_Implementation(AChessPiece* ChessPiece, APlayerController* PC)
{
	ChessPiece->SetOwner(PC);
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
		GetWorld()->GetTimerManager().SetTimer(TileTimer,[this]()
		{
			//todo: make a func
			
			FHitResult HitResult;
			if (const bool bClientHit=GetHitResultUnderCursorByChannel(ChessTileTraceChannel,false,HitResult))
			{
				for (const auto& Tile : CurrentValidMoves)
				{
					Tile->DisableShader();
				}
				HoveredTile=Cast<AChessTile>(HitResult.GetActor());
				
				if (HoveredTile->IsOccupied())
				{
					auto& ValidMoves=HoveredTile->GetChessPiece()->GetValidMoves();
					
					CurrentValidMoves=ValidMoves;
					
					for (auto& Tile : ValidMoves)
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
	if (IsValid(GetWorld())) return;

	if (GetWorld()->GetTimerManager().IsTimerPaused(TileTimer))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TileTimer);
		bIsAnyTileSelected=false;
	}
	
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
	if (!IsValid(GetCurrentHoveredTile()) || !IsValid(GetWorld())) return;

	UE_LOGFMT(LogChessGame,Warning,"{a} Chessboard valid!",GetWorld()->GetGameState<AChessGameState>()->GetChessBoard()->GetName());

	auto* CurrentTile=GetCurrentHoveredTile();
	auto* ChessPieceOnTile=CurrentTile->GetChessPiece();

	if (CurrentTile->IsOccupied())
	{
		
		GetWorld()->GetTimerManager().PauseTimer(TileTimer);
		bIsAnyTileSelected=true;
		//temp
		auto& Moves=ChessPieceOnTile->GetValidMoves();

		FHitResult HitResult;
		//find out the context of the tile
		if (GetHitResultUnderCursorByChannel(ChessTileTraceChannel,false,HitResult))
		{
			if (Moves.Contains(HitResult.GetActor()) && HitResult.GetActor()!=CurrentTile)
			{
				//Server_RequestPieceOwnership( ChessPieceOnTile,this);
				
				//ChessPieceOnTile->Server_TryMoveTo(Cast<AChessTile>(HitResult.GetActor()));
				
					Server_MovePiece(ChessPieceOnTile,Cast<AChessTile>(HitResult.GetActor()));
				
			
				
				GetWorld()->GetTimerManager().UnPauseTimer(TileTimer);
				bIsAnyTileSelected=false;
			}
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

