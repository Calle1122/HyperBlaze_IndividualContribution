#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMatchPassTypes : uint8
{
	PassCount,
	GoalAssistCount,
	StealCount,
	LongestPassRecord,
	StealValueRecord,
};

UENUM(BlueprintType)
enum class EMatchMiscTypes : uint8
{
	BallControlTime,
	BallHoldTime,
	TotalPlayerScore,
};

UENUM(BlueprintType)
enum class EMatchKillTypes : uint8
{
	KillCount,
	DoubleKillCount,
	DefendingKillCount,
	SavingKillCount,
	LongestKillRecord,
};

UENUM(BlueprintType)
enum class EMatchGoalTypes : uint8
{
	GoalCount,
	ThrownGoalCount,
	DunkGoalCount,
	LongestGoalRecord,
	HighestBounceGoalRecord,
	GoalValueRecord,
};

UENUM(BlueprintType)
enum class EMatchDeathTypes : uint8
{
	DeathCount,
};

UENUM(BlueprintType)
enum class EMatchAssistTypes : uint8
{
	AssistCount,
	DoubleAssistCount,
};