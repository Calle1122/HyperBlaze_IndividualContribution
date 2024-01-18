#pragma once

#include "CoreMinimal.h"
#include "GoalMatchStats.generated.h"

USTRUCT(BlueprintType)
struct FGoalMatchStats
{
	GENERATED_BODY()

	FGoalMatchStats();
	~FGoalMatchStats();

	/** COUNTERS */
	
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Counters")
	int MatchGoalCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Counters")
	int MatchThrowGoalCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Counters")
	int MatchDunkGoalCount;

	/** RECORDS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Records")
	float LongestGoalRecord;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Records")
	int HighestBounceGoalRecord;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Goal Records")
	int GoalValueRecord;
};

