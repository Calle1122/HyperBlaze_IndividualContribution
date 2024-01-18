#pragma once

#include "CoreMinimal.h"
#include "DeathMatchStats.generated.h"

class ABlazerCharacter;

USTRUCT(BlueprintType)
struct FDeathMatchStats
{
	GENERATED_BODY()

	FDeathMatchStats();
	~FDeathMatchStats();

	/** COUNTERS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Death Counters")
	int MatchDeathCount;
};

