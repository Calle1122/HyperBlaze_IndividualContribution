#pragma once

#include "CoreMinimal.h"
#include "AssistMatchStats.generated.h"

USTRUCT(BlueprintType)
struct FAssistMatchStats
{
	GENERATED_BODY()

	FAssistMatchStats();
	~FAssistMatchStats();

	/** COUNTERS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Assist Counters")
	int MatchAssistCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Assist Counters")
	int DoubleAssistCount;
};

