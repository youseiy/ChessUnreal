// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ChessPiecesSet.generated.h"

class AChessPiece;
/**
 * 
 */
UCLASS()
class CHESSGAME_API UChessPiecesSet : public UDataAsset
{
	GENERATED_BODY()



	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> King;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> Queen;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> Knight;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> Rook;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> Pawn;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AChessPiece> Bishop;





	

	
};
