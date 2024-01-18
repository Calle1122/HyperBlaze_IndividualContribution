#pragma once

#include "CoreMinimal.h"
#include "MiscMatchStats.generated.h"

class ABlazerCharacter;

USTRUCT(BlueprintType)
struct FMiscMatchStats
{
	GENERATED_BODY()

	FMiscMatchStats();
	~FMiscMatchStats();

	/** COUNTERS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Misc Counters")
	float BallControlTime;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Misc Counters")
	float BallHoldTime;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Misc Counters")
	int TotalPlayerScore;
};

