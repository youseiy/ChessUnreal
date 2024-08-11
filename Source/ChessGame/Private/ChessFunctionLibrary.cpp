// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessFunctionLibrary.h"

#include "ChessGameplayTags.h"
#include "ChessPiece.h"

FGameplayTag UChessFunctionLibrary::EvaluatePieceTeam(const AChessPiece* Piece,const FGameplayTag& TeamTag)
{
	//we  check if the x from id has 0 || 1 for white and 6 || 7 to black
	ensure(Piece);
	
	const auto DefaultBoardID=Piece->GetDefaultBoardID();
	
	return DefaultBoardID.X==0 || DefaultBoardID.X==1? ChessGameplayTags::TAG_Team_White:ChessGameplayTags::TAG_Team_Black;
}
