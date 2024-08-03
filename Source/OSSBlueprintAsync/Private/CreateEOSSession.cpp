// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateEOSSession.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>
#include <Logging/StructuredLog.h>
#include "EOSLimiarSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineSubsystemUtils.h"
#include "Kismet/GameplayStatics.h"


class UEOSLimiarSubsystem;

void UCreateEOSSession::Activate()
{
	EOSSubsystem=ContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UEOSLimiarSubsystem>();
	
	if (EOSSubsystem->GetIdentityPtr()->GetLoginStatus(0)!=ELoginStatus::LoggedIn || !EOSSubsystem )
	{
		UE_LOGFMT(LogCore, Warning, "Cant Create Session!, Not LoggedIn!");
			return;
	}
		
		 
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsDedicated=false;
		SessionSettings.bShouldAdvertise=true;
		SessionSettings.bIsLANMatch=false;
		SessionSettings.NumPublicConnections=5;
		SessionSettings.bAllowJoinInProgress=true;
		SessionSettings.bAllowInvites=true;
		SessionSettings.bAllowJoinViaPresence=true;
		SessionSettings.bUsesPresence=true;
		SessionSettings.bUseLobbiesIfAvailable=true;
		SessionSettings.Set(SEARCH_KEYWORDS,FString("CubeRage"),EOnlineDataAdvertisementType::ViaOnlineService);

	    EOSSubsystem->GetSessionPtr()->OnCreateSessionCompleteDelegates.AddUObject(this,&UCreateEOSSession::OnCreateSession);

	if (!EOSSubsystem->GetSessionPtr()->CreateSession(0,FName("CubeRage"),SessionSettings))
		OnFail.Broadcast();
	
	

}
UCreateEOSSession* UCreateEOSSession::CreateEOSSession(UObject* ContextObject,APlayerController* Controller, int32 Players, TSoftObjectPtr<UWorld> Level)
{
	UCreateEOSSession* Action = NewObject<UCreateEOSSession>();
	Action->ContextObject=ContextObject;
	Action->NumPlayers=Players;
	Action->PlayerController=Controller;
	Action->LevelToTravel=Level;
	return Action;
}

void UCreateEOSSession::OnCreateSession(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOGFMT(LogTemp,Warning,"CreateSession: {sessionname}",SessionName);
		//const FName LevelName = FName(*FPackageName::ObjectPathToPackageName( LevelToTravel.ToString()));
		UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController->GetWorld(),LevelToTravel,true,"listen");
		
		OnSuccess.Broadcast();
	}
	else
	{
		UE_LOGFMT(LogTemp,Warning,"Failed to CreateSession!");
		
	}
	SetReadyToDestroy();
	EOSSubsystem->GetSessionPtr()->ClearOnCreateSessionCompleteDelegates(this);

}
