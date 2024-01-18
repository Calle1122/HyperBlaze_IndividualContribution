#pragma once

#include "CoreMinimal.h"
#include "PassMatchStats.generated.h"

USTRUCT(BlueprintType)
struct FPassMatchStats
{
	GENERATED_BODY()

	FPassMatchStats();
	~FPassMatchStats();

	/** COUNTERS */
	
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Pass Counters")
	int MatchPassCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Pass Counters")
	int MatchGoalAssistCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Pass Counters")
	int MatchStealCount;

	/** RECORDS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Pass Records")
	float LongestPassRecord;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Pass Records")
	int StealValueRecord;
};

