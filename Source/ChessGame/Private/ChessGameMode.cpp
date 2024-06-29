// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessGameMode.h"

#include "ChessPlayerController.h"
#include "ChessPlayerState.h"

AChessGameMode::AChessGameMode()
{
	PlayerStateClass=AChessPlayerState::StaticClass();
	PlayerControllerClass=AChessPlayerController::StaticClass();
}
