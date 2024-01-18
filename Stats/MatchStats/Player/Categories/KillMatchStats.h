#pragma once

#include "CoreMinimal.h"
#include "KillMatchStats.generated.h"

USTRUCT(BlueprintType)
struct FKillMatchStats
{
	GENERATED_BODY()

	FKillMatchStats();
	~FKillMatchStats();

	/** COUNTERS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Kill Counters")
	int MatchKillCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Kill Counters")
	int DoubleKillCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Kill Counters")
	int DefendingKillCount;
	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Kill Counters")
	int SavingKillCount;

	/** RECORDS */

	UPROPERTY(BlueprintReadWrite, Category = "MatchStats Kill Records")
	float LongestKillRecord;
};

