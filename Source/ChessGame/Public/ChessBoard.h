// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	
	 int32 NumSquares{64};

	virtual void OnConstruction(const FTransform& Transform) override;

	TSubclassOf<class AChessPiece> GetPieceClass(int32 Index, bool bIsWhite) const;

	void BuildBoard();
public:
	// Sets default values for this actor's properties
	AChessBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
};