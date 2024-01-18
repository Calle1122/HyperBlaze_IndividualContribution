#pragma once

#include "CoreMinimal.h"
#include "Categories/AssistMatchStats.h"
#include "Categories/DeathMatchStats.h"
#include "Categories/GoalMatchStats.h"
#include "Categories/KillMatchStats.h"
#include "Categories/MiscMatchStats.h"
#include "Categories/PassMatchStats.h"
#include "PlayerMatchStats.generated.h"

USTRUCT(BlueprintType)
struct FPlayerMatchStats
{
	GENERATED_BODY()

	FPlayerMatchStats();
	~FPlayerMatchStats();

	//Goal Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FGoalMatchStats GoalMatchStats;

	//Pass Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FPassMatchStats PassMatchStats;

	//Kill Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FKillMatchStats KillMatchStats;

	//Death Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FDeathMatchStats DeathMatchStats;

	//Assist Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FAssistMatchStats AssistMatchStats;

	//Misc Stats
	UPROPERTY(BlueprintReadWrite, Category = "Player MatchStats Category")
	FMiscMatchStats MiscMatchStats;
};

