#pragma once

#include "CoreMinimal.h"
#include "ActionTypes/GoalTypes/GoalTypes.h"
#include "GoalProperties.generated.h"

class ABlazerPlayerState;

USTRUCT(BlueprintType)
struct FGoalProperties
{
	GENERATED_BODY()

	FGoalProperties();
	~FGoalProperties();

	/** ACTORS */

	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Actors")
	ABlazerPlayerState* ScoringPlayer;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Actors")
	ABlazerPlayerState* GoalAssistPlayer;

	/** GOAL TYPE */

	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Type")
	EGoalTypes GoalType;

	/** GOAL INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	int ScoringTeam;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	float DistanceTravelledToGoal;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	int BallValue;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	int BounceCount;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	int PreviousTeamInLead;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	int NewTeamInLead;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	bool bLongGoal;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	bool bHattrickGoal;
	UPROPERTY(BlueprintReadWrite, Category = "Goal Properties Info")
	FString GoalCharge;
};

