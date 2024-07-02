// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessTile.h"
#include "GameFramework/Actor.h"
#include "ChessBoard.generated.h"

class AChessTile;
class UChessPiecesSet;





UCLASS()
class CHESSGAME_API AChessBoard : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UChessPiecesSet> BlackPiecesSet;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UChessPiecesSet> WhitePiecesSet;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessTile> BlackTile;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessTile> WhiteTile;
	
	UPROPERTY()
	TArray<AChessTile*> Tiles;

	UPROPERTY()
	TArray<AChessTile*> WhiteTiles;

	UPROPERTY()
	TArray<AChessTile*> BlackTiles;
	
	UPROPERTY()
	TArray<AChessPiece*> Pieces;
	
	int32 X{8};
	int32 Y{8};
	FVector2D BoardSize{8,8};

	
	TSubclassOf<class AChessPiece> GetPieceClass(int32 Index, bool bIsWhite) const;
	

	void BuildBoard();
	TSubclassOf<AChessPiece> EvaluatePieceClass(int32 OuterIndex, int32 InnerIndex);
	AChessPiece* InitChessPiece(const TSubclassOf<AChessPiece>& PieceClass,AChessTile& OwnerTile,const FVector& Location,const  FVector2D& BoardID);
	AChessTile* InitChessTile(TSubclassOf<AChessTile> TileClass,const FTransform& Location);
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
public:
	AChessBoard();
	const TArray<AChessTile*>& GetTiles()const {return Tiles;}
	bool IsWithinBoardLimits(const FVector2D& Position) const;
	AChessTile* GetTileAt(const FVector2D& Position) const;
	UFUNCTION(Server,Reliable)
	void Server_RequestChessPieceMove(AChessPiece* Piece,AChessTile* Tile);
};