// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChessPiece.h"
#include "Rook.generated.h"

UCLASS()
class CHESSGAME_API ARook : public AChessPiece
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARook();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void UpdateValidMoves() final override;
public:
};
