// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
//#include "ChessGameplayTags.generated.h"


namespace ChessGameplayTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Team_White);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Team_Black);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Widget_WaitingforPlayers);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Widget_Pause);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Widget_GameHUD);
}
