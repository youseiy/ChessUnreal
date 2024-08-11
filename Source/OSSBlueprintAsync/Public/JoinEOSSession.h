// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h"
#include "JoinEOSSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFail);

UCLASS()
class OSSBLUEPRINTASYNC_API UJoinEOSSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	UPROPERTY()
	class UEOSLimiarSubsystem* EOSSubsystem=nullptr;

	UPROPERTY()
	UObject* ContextObject;
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(BlueprintAssignable)
	FOnSuccess OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnFail OnFail;

	void OnJoinComplete(FName Name, EOnJoinSessionCompleteResult::Type Result);
protected:
	virtual void Activate() override;
public:
	UPROPERTY(VisibleAnywhere)
	 FBlueprintSessionResult GameSession;

	bool bTravelSeamless=false;

	UFUNCTION(BlueprintCallable,Category=EOSBlueprintAsync,meta=(BlueprintInternalUseOnly = "true",HidePin = "WorldContextObject",DefaultToSelf="WorldContextObject"))
	static UJoinEOSSession* JoinEOSSession(UObject* WorldContextObject, APlayerController* Controller,bool bSeamless, FBlueprintSessionResult Session);
	


};
