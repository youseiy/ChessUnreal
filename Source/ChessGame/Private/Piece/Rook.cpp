// Fill out your copyright notice in the Description page of Project Settings.


#include "Piece/Rook.h"

#include "ChessBoard.h"
#include "ChessGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARook::ARook()
{
}

// Called when the game starts or when spawned
void ARook::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARook::UpdateValidMoves()
{
	Super::UpdateValidMoves();
	// This is only on the server
	auto* ChessGameMode = Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ChessGameMode) || !IsValid(ChessGameMode->ChessBoard))
	{
		return;
	}

	ValidMoves.Empty();

	// Determine the direction multipliers based on piece color
	float VerticalMultiplier = GetIsWhite() ? 1 : -1; // Reverse for black pieces
	float HorizontalMultiplier = 1; // Horizontal direction remains the same for both colors

	// Directions the rook can move: right, left, up, down
	TArray<FVector2D> Directions = {
		{HorizontalMultiplier, 0},   // Right
		{-HorizontalMultiplier, 0},  // Left
		{0, -VerticalMultiplier},     // Up (white) / Down (black)
		{0, VerticalMultiplier}     // Down (white) / Up (black)
	};

	// Iterate over each direction
	for (const FVector2D& Direction : Directions)
	{
		FVector2D CurrentPosition = CurrentBoardID;

		// Continue moving in the current direction until we hit an obstacle or go out of bounds
		for (;;)
		{
			DrawDebugString(GetWorld(),GetActorLocation(),FString::Printf(TEXT("Piece Board ID %0.0f : %0.0f"),CurrentPosition.X,CurrentPosition.Y));		
			CurrentPosition -= Direction;

			
			// Check if the new position is within board limits
			if (!ChessGameMode->ChessBoard->IsWithinBoardLimits(CurrentPosition))
			{
				break;
			}

			AChessTile* Tile = ChessGameMode->ChessBoard->GetTileAt(CurrentPosition);
			if (!Tile)
			{
				break;
			}

			DrawDebugString(GetWorld(),Tile->GetActorLocation(),FString::Printf(TEXT("Board ID %0.0f : %0.0f"),CurrentPosition.X,CurrentPosition.Y));			
			
			// Check if the tile is occupied
			if (Tile->IsOccupied())
			{
				if (Tile->GetChessPiece()->GetIsWhite() != GetIsWhite())
				{
					ValidMoves.Add(Tile); // Capture move
				}
				break; // Stop moving in this direction
			}

			ValidMoves.Add(Tile); // Valid move
		}
	}
}