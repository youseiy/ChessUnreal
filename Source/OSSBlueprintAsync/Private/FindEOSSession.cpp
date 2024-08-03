// Fill out your copyright notice in the Description page of Project Settings.


#include "FindEOSSession.h"

#include "EOSLimiarSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Logging/StructuredLog.h"
#include "Online/OnlineSessionNames.h"


void UFindEOSSession::Activate()
{
	RegisterWithGameInstance(this);
	EOSSubsystem=PlayerController->GetWorld()->GetGameInstance()->GetSubsystem<UEOSLimiarSubsystem>();
	if (!EOSSubsystem->IsPlayerLogged()) return;

	SessionSearch= MakeShareable(new  FOnlineSessionSearch());
	SessionSearch->MaxSearchResults=10;
	SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS,FString("CubeRage"),EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES,true,EOnlineComparisonOp::Equals);


	EOSSubsystem->GetSessionPtr()->OnFindSessionsCompleteDelegates.AddUObject(this,&ThisClass::OnFindSessionsComplete);
	EOSSubsystem->GetSessionPtr()->FindSessions(0,SessionSearch.ToSharedRef());


	
}
UFindEOSSession* UFindEOSSession::FindEOSSession(UObject* WorldContextObject,APlayerController* Controller)
{
	UFindEOSSession* Action=NewObject<UFindEOSSession>();
	Action->PlayerController=Controller;
	return Action;
}
void UFindEOSSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	UE_LOGFMT(LogCore, Warning, "Found Sessions? {Bool}",bWasSuccessful );

	if (bWasSuccessful)
	{
		UE_LOGFMT(LogCore, Warning, "Found {Many} Sessions ",SessionSearch->SearchResults.Num() );

		EOSSubsystem->GetSessionPtr()->ClearOnFindSessionsCompleteDelegates(this);
		TArray<FBlueprintSessionResult> BPWrapper;

		for (const auto& Wrapper : SessionSearch->SearchResults)
		{	
			FBlueprintSessionResult BP;
			BP.OnlineResult=Wrapper;
			BPWrapper.Add(BP);
		}	

		OnComplete.Broadcast(BPWrapper);
		SetReadyToDestroy();
	}
}