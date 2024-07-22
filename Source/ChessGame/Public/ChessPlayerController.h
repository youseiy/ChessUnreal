// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UPROPERTY(ReplicatedUsing=OnRep_TeamTag)
	FGameplayTag TeamTag;

	FGameplayTag CurrentTurn;
	

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
	
	UFUNCTION()
	void OnRep_TeamTag();

protected:
	virtual void OnRep_Pawn() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PostInitializeComponents() override;
	virtual void OnRep_PlayerState() override;
public:
	AChessPlayerController();
	
	void Server_SetTeam(FGameplayTag Tag);
	
	FGameplayTag GetTeam()const {return TeamTag;}
	
	UFUNCTION(Client,Reliable)
	void Client_StartTurn();
	UFUNCTION(Client,Reliable)
	void Client_EndTurn();
	AChessTile* GetCurrentHoveredTile()const{return HoveredTile;}
	UFUNCTION(Server,Reliable)
	void Server_MovePiece(AChessPiece* Piece, AChessTile* CurrentTile, AChessTile* TileToMove);
	UFUNCTION(Server,Reliable)
	void Server_CapturePiece(AChessPiece* Piece, AChessPiece* CapturedPiece);
	
};
