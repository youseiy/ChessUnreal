// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessPlayerController.generated.h"

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


	FTimerHandle TileTime;
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
	void Select(const FInputActionValue& Value);
	void MoveBoard(const FInputActionValue& Value);

	UFUNCTION(Server,Reliable)
	void Server_RequestTile();
	UFUNCTION(Client,Reliable)
	void Client_TileHit(const FHitResult& TileHitResult);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	AChessPlayerController();
	
	AChessTile* GetCurrentHoveredTile()const{return HoveredTile;}
};
