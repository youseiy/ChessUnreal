// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FindSessionsCallbackProxy.h"
#include "FindEOSSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComplete,const TArray<FBlueprintSessionResult>&, Results);

class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class OSSBLUEPRINTASYNC_API UFindEOSSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	class UEOSLimiarSubsystem* EOSSubsystem=nullptr;
	UPROPERTY()
	UObject* ContextObject;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;


	void OnFindSessionsComplete(bool bWasSuccessful);
protected:

	virtual void Activate() override;
public:

	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY(BlueprintAssignable)
	FOnComplete OnComplete;

	UFUNCTION(BlueprintCallable,Category=EOSBlueprintAsync,meta=(BlueprintInternalUseOnly = "true",HidePin = "WorldContextObject",DefaultToSelf="WorldContextObject"))
	static UFindEOSSession* FindEOSSession(UObject* WorldContextObject, APlayerController* Controller);

	
};
