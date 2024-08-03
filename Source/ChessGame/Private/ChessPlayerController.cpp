// Fill out your copyright notice in the Description page of Project Settings.
#include "ChessPlayerController.h"

#include "ChessBoard.h"
#include "ChessCollisionTypes.h"
#include "ChessFunctionLibrary.h"
#include "ChessGameState.h"
#include "ChessPlayerState.h"
#include "ChessTile.h"
#include "EnhancedInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "ChessGame/ChessGame.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Logging/StructuredLog.h"
#include "Net/UnrealNetwork.h"

AChessPlayerController::AChessPlayerController()
{
	bReplicates=true;
	
	bShowMouseCursor=true;

	
}

void AChessPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChessPlayerController,TeamTag)
}
void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	ensure(Subsystem);
	
	Subsystem->AddMappingContext(IMC, 0);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(ToggleLookAction, ETriggerEvent::Triggered, this, &ThisClass::ToggleLook);
	EnhancedInputComponent->BindAction( SelectAction,ETriggerEvent::Started, this, &ThisClass::SelectChessPiece);
	EnhancedInputComponent->BindAction(MoveBoardAction, ETriggerEvent::Started, this, &ThisClass::MoveBoard);
	EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &ThisClass::Cancel);
	
}

void AChessPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		OnRep_PlayerState();
	}
}

void AChessPlayerController::Server_SetTeam(FGameplayTag Tag)
{
	TeamTag=Tag;
	OnRep_TeamTag();
}



void AChessPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	

	if (HasAuthority())
	{
		GetPlayerState<AChessPlayerState>()->OnPlayerReady.ExecuteIfBound();
	}
	GetPlayerState<AChessPlayerState>()->OnPlayerReady.ExecuteIfBound();
}

void AChessPlayerController::Cancel(const FInputActionValue& Value)
{
	if (IsValid(GetWorld())) return;

	
	if (GetWorld()->GetTimerManager().IsTimerPaused(TileTimer))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(TileTimer);
		bIsAnyTileSelected=false;
	}
	
}

void AChessPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	
	AddPitchInput(LookAxis.Y);
	AddYawInput(LookAxis.X);
}

void AChessPlayerController::ToggleLook(const FInputActionValue& Value)
{
	//this doesnt do anything, its need for chorded action look on Mouse
}
void AChessPlayerController::MoveBoard(const FInputActionValue& Value)
{
	GetWorld()->GetTimerManager().PauseTimer(TileTimer);
	
	// Specific for gamepad input
	FVector2D MovementInput = Value.Get<FVector2D>();
    
	// Ensure we have a valid tile and game state
	if (IsValid(GetCurrentHoveredTile()))
	{
		auto* CurrentTile = GetCurrentHoveredTile();
		auto* GS = GetWorld()->GetGameState<AChessGameState>();
		if (IsValid(GS) && IsValid(GS->GetChessBoard()))
		{
			DrawDebugBox(GetWorld(),CurrentTile->GetActorLocation(),FVector{200},FColor::Red,true,10.f);
					
			FVector2D CurrentTileID = CurrentTile ->GetTileID();
			FVector2D NewTileID = CurrentTileID + MovementInput.RoundToVector(); // Adjust based on input

			// Ensure the new tile is within board limits
			if (GS->GetChessBoard()->IsWithinBoardLimits(NewTileID))
			{
				AChessTile* NewTile = GS->GetChessBoard()->GetTileAt(NewTileID);
				if (IsValid(NewTile))
				{
					DrawDebugBox(GetWorld(),NewTile->GetActorLocation(),FVector{200},FColor::Red,true,10.f);
					
					HoveredTile->DisableShader();
					HoveredTile =  NewTile;
					if (HoveredTile->IsOccupied())
					{
						if (auto* ChessPiece = HoveredTile->GetChessPiece())
						{
							CurrentValidMoves = ChessPiece->GetValidMoves();
					
							for (auto& Tile : CurrentValidMoves)
							{

								if (Tile->IsOccupied())
								{
									Tile->ShowShader();
									continue;
								}
						
								Tile->ShowShader();

						
							}
						}
					}
				}
			}
		}
	}
}

void AChessPlayerController::SelectTile()
{
	//if (!CurrentTurn.MatchesTagExact(TeamTag)) return;
	
	
	FHitResult HitResult;
	if (GetHitResultUnderCursorByChannel(TraceTypeQuery2, false, HitResult))
	{
		// Disable shaders for previously valid moves and clear the list
		for (const auto& Tile : CurrentValidMoves)
		{
			Tile->DisableShader();
		}
		CurrentValidMoves.Empty();

		// Update HoveredTile
		if (AChessTile* NewHoveredTile = Cast<AChessTile>(HitResult.GetActor()))
		{
			HoveredTile = NewHoveredTile;
			if (HoveredTile->IsOccupied())
			{
				auto* ChessPiece = HoveredTile->GetChessPiece();
				if (ChessPiece->GetTeam()==TeamTag)
				{
					CurrentValidMoves = ChessPiece->GetValidMoves();
					
					for (auto& Tile : CurrentValidMoves)
					{

						if (Tile->IsOccupied())
						{
							Tile->ShowShader();
							continue;
						}
						
						Tile->ShowShader();

						
					}
				}
			}
		}
	}
}

void AChessPlayerController::SelectChessPiece(const FInputActionValue& Value)
{
	if (!IsValid(GetCurrentHoveredTile()) || !IsValid(GetWorld())) return;


	auto* CurrentTile=GetCurrentHoveredTile();
	auto* ChessPieceOnTile=CurrentTile->GetChessPiece();

	if (CurrentTile->IsOccupied())
	{
		
		GetWorld()->GetTimerManager().PauseTimer(TileTimer);
		bIsAnyTileSelected=true;
		//temp
		auto& Moves=ChessPieceOnTile->GetValidMoves();

		FHitResult HitResult;
		//find out the context of the tile
		if (GetHitResultUnderCursorByChannel(ChessTileTraceChannel,false,HitResult))
		{
			if (Moves.Contains(HitResult.GetActor()) && HitResult.GetActor()!=CurrentTile)
			{
				if (!Cast<AChessTile>(HitResult.GetActor())->IsOccupied())
				{
					Server_MovePiece(ChessPieceOnTile,CurrentTile, Cast<AChessTile>(HitResult.GetActor()));
				}
				else
				{
					Server_CapturePiece(ChessPieceOnTile, Cast<AChessTile>(HitResult.GetActor())->GetChessPiece());
					Server_MovePiece(ChessPieceOnTile,CurrentTile, Cast<AChessTile>(HitResult.GetActor()));
					
				}
				
			
				
			
				
				GetWorld()->GetTimerManager().UnPauseTimer(TileTimer);
				bIsAnyTileSelected=false;
			}
		}
	}
	
}
void AChessPlayerController::Server_CapturePiece_Implementation(AChessPiece* Piece, AChessPiece* CapturedPiece)
{
	CapturedPiece->OnPieceCaptured.ExecuteIfBound();
	
}

void AChessPlayerController::Server_MovePiece_Implementation(AChessPiece* Piece, AChessTile* CurrentTile, AChessTile* TileToMove)
{
	CurrentTile->OnChessPieceMoved.ExecuteIfBound();
	auto* GS{GetWorld()->GetGameState<AChessGameState>()};
	ensure(GS);
	GS->GetChessBoard()->Server_RequestChessPieceMove(Piece,TileToMove);
	GS->Server_EndTurn(TeamTag);
}
void AChessPlayerController::Client_EndTurn_Implementation()
{
	check(GetWorld())
	
	//todo: turn into function
	for (const auto& Tile : CurrentValidMoves)
	{
		Tile->DisableShader();
	}
	
	GetWorld()->GetTimerManager().ClearTimer(TileTimer);
	CurrentTurn=FGameplayTag::EmptyTag;
}

void AChessPlayerController::Client_StartTurn_Implementation()
{
	check(GetWorld());
	
	GetWorld()->GetTimerManager().SetTimer(TileTimer,
	FTimerDelegate::CreateUObject(this,&AChessPlayerController::SelectTile),
	1.f/60.f,true);

	CurrentTurn=TeamTag;
	UE_LOGFMT(LogChessGame,Warning,"ITS MY TURN {A}",TeamTag.GetTagName());

}
void AChessPlayerController::OnRep_TeamTag()
{
	UE_LOGFMT(LogChessGame,Warning,"MY TEAM IS {A}!",TeamTag.GetTagName());
}

void AChessPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
}

void AChessPlayerController::OnPossess(APawn* InPawn)
 {
 	Super::OnPossess(InPawn);
 
 	if (HasAuthority())
 	{
 		OnRep_Pawn();
 	}
 }

