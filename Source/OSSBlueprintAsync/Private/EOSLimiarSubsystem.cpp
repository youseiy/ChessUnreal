// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSLimiarSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Logging/StructuredLog.h"
#include "Online/OnlineSessionNames.h"

void UEOSLimiarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SubsystemPtr = IOnlineSubsystem::Get();
	IdentityPtr = SubsystemPtr->GetIdentityInterface();
    SessionPtr=SubsystemPtr->GetSessionInterface();

}
void UEOSLimiarSubsystem::Deinitialize()
{
	//SubsystemPtr->Shutdown();
	//IdentityPtr->Logout(0);
	
}

void UEOSLimiarSubsystem::LoginEOS()
{
	if (!SubsystemPtr || !IdentityPtr) return;

	
	FString AuthType; 
    FParse::Value(FCommandLine::Get(), TEXT("AUTH_TYPE="), AuthType);

	IdentityPtr->OnLoginCompleteDelegates->AddWeakLambda(this,[this](int32 LocalUserNum,bool bWasSuccessful,const FUniqueNetId& UserID,const FString& Error)
	{
		if (SubsystemPtr && IdentityPtr)
		{
		        
			GEngine->AddOnScreenDebugMessage(1,5.f,FColor::Blue,FString::Printf(TEXT("Welcome To HeavenChess! %s"),*IdentityPtr->GetPlayerNickname(0)));
			IdentityPtr->ClearOnLoginCompleteDelegates(0,this);
		}
	});


	if (!IdentityPtr->AutoLogin(0))
	{
		 /* 
        Fallback if the CLI parameters are empty.Useful for PIE.
        The type here could be developer if using the DevAuthTool, ExchangeCode if the game is launched via the Epic Games Launcher, etc...
        */
        FOnlineAccountCredentials Credentials("accountportal","", "");
 
        UE_LOG(LogTemp, Log, TEXT("Logging into EOS...")); // Log to the UE logs that we are trying to log in. 
        
        if (!IdentityPtr->Login(0, Credentials))
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to login... ")); // Log to the UE logs that we are trying to log in.
			// Clear our handle and reset the delegate. 
            IdentityPtr->ClearOnLoginCompleteDelegate_Handle(0, LoginDelegateHandle);
            LoginDelegateHandle.Reset();			
        }        
	}
    
    
	
}

void UEOSLimiarSubsystem::CreateEOSSession()
{
	if (IdentityPtr->GetLoginStatus(0)!=ELoginStatus::LoggedIn )
	{
		UE_LOGFMT(LogCore, Warning, "Cant Create Session!, Not LoggedIn!");
		return;
	}

    if (SubsystemPtr)
	{
		 if (SessionPtr)
		 {
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

            SessionPtr->OnCreateSessionCompleteDelegates.AddWeakLambda(this,[this](FName SessionName, bool bWasSuccessful)
            {
	            if (SubsystemPtr)
	            {
		            if (SessionPtr)
		            {
                        UE_LOG(LogTemp,Warning,TEXT("Createsessionnnnnnn"));
			            SessionPtr->ClearOnCreateSessionCompleteDelegates(this);
		            }
	            }
            });

            SessionPtr->CreateSession(0,FName("CubeRage"),SessionSettings);
			
		 }
        
	}
}

void UEOSLimiarSubsystem::ShowInviteUI()
{
	if (IsPlayerLogged() && GetOnlineSubsystemPtr())
	{
		GetOnlineSubsystemPtr()->GetExternalUIInterface()->ShowInviteUI(0,FName("CubeRage"));
	}
}

bool UEOSLimiarSubsystem::IsPlayerLogged()
{
	return IdentityPtr->GetLoginStatus(0)==ELoginStatus::LoggedIn;
}



