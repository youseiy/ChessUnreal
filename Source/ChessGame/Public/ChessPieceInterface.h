﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChessPieceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UChessPieceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHESSGAME_API IChessPieceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual TArray<class AChessTile*> GetValidMoves() const=0;
	virtual void MoveTo(AChessTile* NewPosition)=0;
	virtual void Capture(class AChessPiece* TargetPiece)=0;
	virtual bool IsMoveValid(const FVector& NewPosition) const=0;
};
