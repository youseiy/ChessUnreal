// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CreateEOSSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreateFail);

UCLASS()
class OSSBLUEPRINTASYNC_API UCreateEOSSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
protected:

	virtual void Activate() override;

public:
	UPROPERTY()
	class UEOSLimiarSubsystem* EOSSubsystem=nullptr;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UObject* ContextObject;

	UPROPERTY(EditAnywhere)
	int32 NumPlayers;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LevelToTravel;

	UPROPERTY(BlueprintAssignable)
	FOnCreateSuccess OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnCreateFail OnFail;

	UFUNCTION(BlueprintCallable,Category=EOSBlueprintAsync,meta=(BlueprintInternalUseOnly = "true",HidePin = "WorldContextObject",DefaultToSelf="WorldContextObject"))
	static UCreateEOSSession* CreateEOSSession(UObject* WorldContextObject,APlayerController* Controller, int32 Players, TSoftObjectPtr<UWorld> Level);

	void OnCreateSession(FName SessionName, bool bWasSuccessful);
};
