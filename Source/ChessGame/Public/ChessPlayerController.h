// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

class AChessPiece;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class CHESSGAME_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> ToggleLookAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> SelectAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> MoveBoardAction;
	UPROPERTY(EditAnywhere,Category="Input",meta=(AllowPrivateAccess))
	TObjectPtr<class UInputAction> CancelAction;


	FTimerHandle TileTimer;
	UPROPERTY()
	class AChessTile* HoveredTile;
	UPROPERTY()
	class AChessTile* SelectedTile;

	
	UPROPERTY()
	TArray<AChessTile*> CurrentValidMoves;
	
	bool bIsAnyTileSelected=false;
	
	void Cancel(const  FInputActionValue& Value);
	void Look(const  FInputActionValue& Value);
	void ToggleLook(const  FInputActionValue& Value);
	void SelectChessPiece(const FInputActionValue& Value);
	void MoveBoard(const FInputActionValue& Value);
	void SelectTile();

	UFUNCTION(Server,Reliable)
	void Server_RequestTile();
	UFUNCTION(Client,Reliable)
	void Client_TileHit(const FHitResult& TileHitResult);
	UFUNCTION(Client,Reliable)
	void Server_RequestPieceOwnership(AChessPiece* ChessPiece, APlayerController* PC);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	AChessPlayerController();
	AChessTile* GetCurrentHoveredTile()const{return HoveredTile;}
	UFUNCTION(Server,Reliable)
	void Server_MovePiece(AChessPiece* Piece, AChessTile* CurrentTile, AChessTile* TileToMove);
	UFUNCTION(Server,Reliable)
	void Server_CapturePiece(AChessPiece* Piece, AChessPiece* CapturedPiece);
	
};
