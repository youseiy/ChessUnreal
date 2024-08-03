// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "EOSLimiarSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class OSSBLUEPRINTASYNC_API UEOSLimiarSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
	//Delegate to bind callback event for login. 
	FDelegateHandle LoginDelegateHandle;



	IOnlineSubsystem* SubsystemPtr;
	IOnlineIdentityPtr IdentityPtr;
	IOnlineSessionPtr SessionPtr;

	
	UFUNCTION(BlueprintCallable)
	void LoginEOS();
	UFUNCTION(BlueprintCallable)
	void CreateEOSSession();
	UFUNCTION(BlueprintCallable)
	void ShowInviteUI();

	

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	UFUNCTION(BlueprintCallable)
	bool IsPlayerLogged();
	IOnlineSessionPtr GetSessionPtr() const { return SessionPtr;}
	IOnlineIdentityPtr GetIdentityPtr() const {return IdentityPtr;} 
	IOnlineSubsystem* GetOnlineSubsystemPtr() const{ return SubsystemPtr;}
	
	
};
