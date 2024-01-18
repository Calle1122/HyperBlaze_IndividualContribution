#pragma once

#include "CoreMinimal.h"
#include "ProjectBallBlazers/Match/BlazerPlayerState.h"
#include "KillProperties.generated.h"

USTRUCT(BlueprintType)
struct FKillProperties
{
	GENERATED_BODY()

	FKillProperties();
	~FKillProperties();

	/** ACTORS */

	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Actors")
	ABlazerPlayerState* KillingPlayer;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Actors")
	TArray<ABlazerPlayerState*> AssistingPlayers;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Actors")
	ABlazerPlayerState* DeadPlayer;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Actors")
	ABlazerPlayerState* DeadPlayerLastTarget;

	/** KILL INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	float DistanceToTarget;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bHeadshotKill;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bDefendingKill;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bSavingKill;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bRevengeKill;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bComebackKill;
	UPROPERTY(BlueprintReadWrite, Category = "Kill Properties Info")
	bool bDoubleKill;
};

