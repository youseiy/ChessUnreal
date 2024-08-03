// Fill out your copyright notice in the Description page of Project Settings.

#include "JoinEOSSession.h"
#include "EOSLimiarSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"



void UJoinEOSSession::Activate()
{
	RegisterWithGameInstance(this);
	EOSSubsystem=PlayerController->GetWorld()->GetGameInstance()->GetSubsystem<UEOSLimiarSubsystem>();

	EOSSubsystem->GetSessionPtr()->OnJoinSessionCompleteDelegates.AddUObject(this,&ThisClass::OnJoinComplete);
	EOSSubsystem->GetSessionPtr()->JoinSession(0,FName("ChessHeaven"),GameSession.OnlineResult);

}

UJoinEOSSession* UJoinEOSSession::JoinEOSSession(UObject* WorldContextObject, APlayerController* Controller,bool bSeamless, FBlueprintSessionResult Session)
{
	UJoinEOSSession* Action=NewObject<UJoinEOSSession>();
	Action->GameSession=Session;
	Action->PlayerController=Controller;
	Action->bTravelSeamless=bSeamless;
	return Action;
}

void UJoinEOSSession::OnJoinComplete(FName Name, EOnJoinSessionCompleteResult::Type Result)
{
	
	if (Result==EOnJoinSessionCompleteResult::Success)
	{
		OnSuccess.Broadcast();
		if (EOSSubsystem)
		{
		FString ConnectionInfo{};
		EOSSubsystem->GetSessionPtr()->GetResolvedConnectString(Name,ConnectionInfo);
			if (!ConnectionInfo.IsEmpty())
			{
			EOSSubsystem->GetSessionPtr()->ClearOnJoinSessionCompleteDelegates(this);
			if (PlayerController)
			{
				PlayerController->ClientTravel(ConnectionInfo,TRAVEL_Absolute,bTravelSeamless);
				SetReadyToDestroy();
			}
			}
		}
	}
	else
	{
		OnFail.Broadcast();
		SetReadyToDestroy();
	}
	
}