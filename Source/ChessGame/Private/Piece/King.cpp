// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/King.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "ChessGameState.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


AKing::AKing()
{
	PrimaryActorTick.bCanEverTick = false;
}
void AKing::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AKing::Server_InCheck() const 
{
	auto* ChessGS = GetWorld()->GetGameState<AChessGameState>();
	ensure(ChessGS);

	// Get all opponent pieces
	TArray<AChessPiece*> OpponentPieces =ChessGS->Server_GetAllOpponentPieces(GetIsWhite());

	// Iterate over each opponent piece to check if any can move to the King's current position
	for (AChessPiece* OpponentPiece : OpponentPieces)
	{
		if (!IsValid(OpponentPiece))
		{
			continue;
		}

		// Update valid moves of the opponent piece
		OpponentPiece->UpdateValidMoves();

		// Check if the King's current position is in the opponent piece's valid moves
		for (AChessTile* ValidMove : OpponentPiece->GetValidMoves())
		{
			if (ValidMove->GetTileID() == CurrentBoardID)
			{
				return true; // King is in check
			}
		}
	}

	return false; // King is not in check*/
}

void AKing::UpdateValidMoves()
{
	Super::UpdateValidMoves();
	// This is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));

	ensure(ChessGameMode && ChessGameMode->ChessBoard);

	ValidMoves.Empty();

	// Directions the king can move: all adjacent squares
	TArray<FVector2D> Directions{
		{1, 0},    // Right
		{-1, 0},   // Left
		{0, 1},    // Up
		{0, -1},   // Down
		{1, 1},    // Top-right
		{-1, 1},   // Top-left
		{1, -1},   // Bottom-right
		{-1, -1}   // Bottom-left
	};

	// Iterate over each direction
	for (const FVector2D& Direction : Directions)
	{
		FVector2D CurrentPosition = CurrentBoardID + Direction;

		// Check if the new position is within board limits
		if (!ChessGameMode->ChessBoard->IsWithinBoardLimits(CurrentPosition))
		{
			continue;
		}

		AChessTile* Tile = ChessGameMode->ChessBoard->GetTileAt(CurrentPosition);
		if (!Tile)
		{
			continue;
		}

		DrawDebugString(GetWorld(), Tile->GetActorLocation(), FString::Printf(TEXT("Board ID %0.0f : %0.0f"), CurrentPosition.X, CurrentPosition.Y));

		// Check if the tile is occupied
		if (Tile->IsOccupied())
		{
			if (Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
			{
				ValidMoves.Add(Tile); // Capture move
			}
		}
		else
		{
			ValidMoves.Add(Tile); // Valid move
		}
	}
}


