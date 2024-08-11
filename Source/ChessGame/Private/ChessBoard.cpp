// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGame/Public/ChessBoard.h"

#include "ChessGameMode.h"
#include "ChessGameState.h"
#include "ChessPiecesSet.h"
#include "Piece/King.h"
#include "ChessGame/ChessGame.h"
#include "ChessGame/Public/ChessPiece.h"
#include "ChessGame/Public/ChessTile.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/StructuredLog.h"



AChessBoard::AChessBoard()
{
	PrimaryActorTick.bCanEverTick = false;


	bReplicates=true;
	bAlwaysRelevant=true;
}
void AChessBoard::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Cast<AChessGameMode>(UGameplayStatics::GetGameMode(this))->ChessBoard=this;
		BuildBoard();
		GetWorld()->GetGameState<AChessGameState>()->Server_SetChessBoard(this);
	}

	
}
void AChessBoard::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}

TSubclassOf<AChessPiece> AChessBoard::GetPieceClass(int32 Index, bool bIsWhite) const
{
	switch (Index)
	{
	case 0: case 7: return bIsWhite ? WhitePiecesSet->Rook: BlackPiecesSet->Rook;
	case 1: case 6: return bIsWhite ? WhitePiecesSet->Knight : BlackPiecesSet->Knight;
	case 2: case 5: return bIsWhite ?  WhitePiecesSet->Bishop :  BlackPiecesSet->Bishop;
	case 3: return bIsWhite ? WhitePiecesSet->Queen : BlackPiecesSet->Queen;
	case 4: return bIsWhite ? WhitePiecesSet->King : BlackPiecesSet->King;
	default: return nullptr;
	}
}
bool AChessBoard::IsWithinBoardLimits(const FVector2D& Position) const
{
	return Position.X >= 0 && Position.X < X &&
		   Position.Y >= 0 && Position.Y < Y;
}
AChessTile* AChessBoard::GetTileAt(const FVector2D& Position) const
{
	// Check if the position is within board limits
	if (!IsWithinBoardLimits(Position))
	{
		return nullptr;
	}

	return *Tiles.FindByPredicate([ Position]( const AChessTile* Piece)
	{
		return Piece->GetTileID()==Position;
	});
}

TArray<AChessPiece*> AChessBoard::GetBlackPieces() const
{
	TArray<AChessPiece*> BlackPieces;

	for (const auto& Piece : Pieces)
	{
		if (!Piece->GetIsWhite())
		{
			BlackPieces.Emplace(Piece);
		}
	}
	
	return BlackPieces;
}

TArray<AChessPiece*> AChessBoard::GetWhitePieces() const
{
	TArray<AChessPiece*> WhitePieces;

	for (const auto& Piece : Pieces)
	{
		if (Piece->GetIsWhite())
		{
			 WhitePieces.Emplace(Piece);
		}
	}
	
	return WhitePieces;
}

void AChessBoard::BuildBoard()
{
	if (!ensure(BlackTile ) || !ensure(BlackTile)) return;
	
	for (int32 OuterIndex{0}; OuterIndex <  X; OuterIndex++)
	{
		for (int32 InnerIndex{0}; InnerIndex < Y; InnerIndex++)
		{
			FVector2D BoardID{static_cast<float>(OuterIndex),static_cast<float>(InnerIndex)};
			
			FVector SpawnLoc = FVector{
				static_cast<float>(InnerIndex - X / 2),
				static_cast<float>(OuterIndex - Y / 2),
				0.f
			} * 100 + GetActorLocation();

			FTransform SpawnTrans;
			SpawnTrans.SetLocation(SpawnLoc);

			
			// Alternate between black and white tiles
			TSubclassOf<AChessTile> TileClass = ((OuterIndex + InnerIndex) % 2 == 0) ? WhiteTile : BlackTile;

			auto* Tile = InitChessTile(TileClass, SpawnTrans);
			Tile->SetBoardID(BoardID);
			Tiles.Emplace(Tile);

			
			
			if ((OuterIndex + InnerIndex) % 2 == 0)
			{
				WhiteTiles.Emplace(Tile);
			}
			else
			{
				BlackTiles.Emplace(Tile);
			}
			
			// Determine chess piece
			TSubclassOf<AChessPiece> PieceClass =EvaluatePieceClass(OuterIndex,InnerIndex);;
			ensure(PieceClass);
			
			auto* SpawnedPiece=InitChessPiece(PieceClass,*Tile,SpawnLoc,BoardID);

			BlackKing=SpawnedPiece->IsA<AKing>()?Cast<AKing>(SpawnedPiece):nullptr;
			WhiteKing=SpawnedPiece->IsA<AKing>()?Cast<AKing>(SpawnedPiece):nullptr;
			
			ensure(SpawnedPiece);
			
			Pieces.Add(SpawnedPiece);
			Tile->SetChessPiece(SpawnedPiece);


			UE_LOGFMT(LogChessGame,Warning,"Tile: {X}:{Y} has Chess piece {a}",OuterIndex,InnerIndex,PieceClass?PieceClass->GetName():"none");
		
			
			
		}
	}

	//Initialize valid moves for all pieces
	for (const auto& Piece : Pieces)
	{
		Piece->UpdateValidMoves();
	}
	
	UE_LOGFMT(LogChessGame,Warning,"Chess board has {a} Tiles, {b} Black and {c} White",Tiles.Num(),WhiteTiles.Num(),BlackTiles.Num());
}

TSubclassOf<AChessPiece> AChessBoard::EvaluatePieceClass(int32 OuterIndex, int32 InnerIndex)
{
	if (OuterIndex == 1) // Pawns
	{
		return WhitePiecesSet->Pawn;
	}
	if (OuterIndex == 6) // Pawns
	{
		return BlackPiecesSet->Pawn;
	}
	if (OuterIndex == 0 || OuterIndex == 7) // Major pieces
	{
		bool bIsWhite = (OuterIndex == 0);
		return GetPieceClass(InnerIndex, bIsWhite);
	}
	return nullptr;
}

AChessPiece* AChessBoard::InitChessPiece(const TSubclassOf<AChessPiece>& PieceClass, AChessTile& OwnerTile,
                                         const FVector& Location, const FVector2D& BoardID)
{
	FTransform Transform{Location};
	AChessPiece* Piece = GetWorld()->SpawnActorDeferred<AChessPiece>(PieceClass,Transform,this,nullptr);
	Piece->SetInitBoardID(BoardID);
	Piece->FinishSpawning(Transform);
	Piece->AttachToActor(&OwnerTile, FAttachmentTransformRules::SnapToTargetIncludingScale);
	return Piece;
	
}

AChessTile* AChessBoard::InitChessTile(TSubclassOf<AChessTile> TileClass,const FTransform& Location)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner=this;
	auto* Tile=GetWorld()->SpawnActor<AChessTile>(TileClass,Location,SpawnParameters);
	Tile->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	return Tile;
}
void AChessBoard::Server_RequestChessPieceMove_Implementation(AChessPiece* Piece, AChessTile* Tile)
{
	if (!IsValid(Piece) || !IsValid(Tile)) return;
	
	Piece->Server_TryMoveTo(Tile);

	for (auto& ChessPiece : Pieces)
	{
		if (!IsValid(ChessPiece)) continue;
		
		ChessPiece->UpdateValidMoves();
	}
}




