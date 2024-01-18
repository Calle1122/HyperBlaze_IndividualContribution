#pragma once

#include "CoreMinimal.h"
#include "PlayerTracker.generated.h"

class ABlazerPlayerState;

USTRUCT(BlueprintType)
struct FPlayerTracker
{
	GENERATED_BODY()

	FPlayerTracker();
	~FPlayerTracker();

	/** ACTOR INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Actors")
	ABlazerPlayerState* LastDamagedEnemy;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Actors")
	ABlazerPlayerState* LastKiller;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Actors")
	ABlazerPlayerState* LastDamager;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Actors")
	TArray<ABlazerPlayerState*> RecentDamagers;

	/** OTHER INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Info")
	float LastDealtDamageAmount;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Info")
	int ConsecutiveDeaths;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Info")
	float TimeSinceLastKill;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Info")
	float TimeSinceLastAssist;
	UPROPERTY(BlueprintReadWrite, Category = "Player Tracker Info")
	float TimeSinceTakingDamage;
};

