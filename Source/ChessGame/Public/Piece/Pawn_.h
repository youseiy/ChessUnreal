// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Pawn_.generated.h"

class AChessTile;

UCLASS()
class CHESSGAME_API APawn_ : public AChessPiece
{
	GENERATED_BODY()

	bool bAlreadyMoved=false;
	virtual void UpdateValidMoves()  final override;
	virtual void Server_TryMoveTo_Implementation(AChessTile* NewPosition) override;
public:
	APawn_();
protected:
	virtual void BeginPlay() override;

};
