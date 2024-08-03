// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ChessFunctionLibrary.generated.h"

class AChessPiece;
/**
 * 
 */
UCLASS()
class CHESSGAME_API UChessFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FGameplayTag EvaluatePieceTeam(const AChessPiece* Piece,const FGameplayTag& TeamTag);
	
};
