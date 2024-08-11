// Fill out your copyright notice in the Description page of Project Settings.


#include "EOSLimiarSubsystem.h"


#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"


void UEOSLimiarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	SubsystemPtr = IOnlineSubsystem::Get();
	IdentityPtr = SubsystemPtr->GetIdentityInterface();
    SessionPtr=SubsystemPtr->GetSessionInterface();

	LoginEOS();
}
void UEOSLimiarSubsystem::Deinitialize()
{
	
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

bool UEOSLimiarSubsystem::IsPlayerLogged()
{
	return IdentityPtr->GetLoginStatus(0)==ELoginStatus::LoggedIn;
}



