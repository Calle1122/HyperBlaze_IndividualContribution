// Fill out your copyright notice in the Description page of Project Settings.

#include "BlazerGameState.h"

#include "BlazerPlayerState.h"
#include "Net/UnrealNetwork.h"

void ABlazerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
	DOREPLIFETIME(ABlazerGameState, Team0Score)
	DOREPLIFETIME(ABlazerGameState, Team1Score)

	DOREPLIFETIME(ABlazerGameState, bIsTimerOn)
	DOREPLIFETIME(ABlazerGameState, GameTimer)

	DOREPLIFETIME(ABlazerGameState, LastPassInfo)
	DOREPLIFETIME(ABlazerGameState, LastGoalInfo)
	
	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazerGameState, CurrentBallScoreValue, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazerGameState, CurrentChargeScore, SharedParams)
}

void ABlazerGameState::BeginPlay()
{
	Super::BeginPlay();
	//Setting 2 teams for now (might make it dynamic later)
	// TeamScores.Add(0, 0);
	// TeamScores.Add(1, 0);
	ResetCurrentBallScoreAndMultiplier();
}


ABlazerGameState::ABlazerGameState()
{
	SecondsBetweenEachMultiplierDecay = 1.f;
}

void ABlazerGameState::AddScore(int Team, int Amount)
{
	switch (Team)
	{
		case 0:
			Team0Score = FMath::Min(1000, Team0Score + Amount);
			break;
		case 1:
			Team1Score = FMath::Min(1000, Team1Score + Amount);
			break;
		default:
			break;
	}

	if(Team0Score == 1000 || Team1Score == 1000)
	{
		SetTimer(0, 0);
	}
}

void ABlazerGameState::SetBallScoreBaseValue(const int NewBaseScoreValue)
{
	//Sets the balls base score value to "NewBaseScoreValue"
	CurrentBallScoreValue = NewBaseScoreValue;
}

void ABlazerGameState::ResetCurrentBallScoreAndMultiplier()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Resetting Ball Score")));
	CurrentChargeScore = 0;
	CurrentBallScoreValue = 50;
	OnChangeChargeScoreThreshold.Broadcast(0, ConvertChargeScoreThreshold(0));
}

void ABlazerGameState::CreateNewPassInfo_Implementation(ABlazerPlayerState* Passer, ABlazerPlayerState* Receiver, float Distance)
{
	LastPassInfo.PassingPlayer = Passer;
	LastPassInfo.ReceivingPlayer = Receiver;
	LastPassInfo.BallValue = GetCurrentBallValue();
	LastPassInfo.PassDistance = Distance;
	LastPassInfo.DistanceToGoal = 0;
	LastPassInfo.bIsSteal = false;
	LastPassInfo.bDefendingSteal = false;
	LastPassInfo.bLongPass = false;

	OnNewPassInfo.Broadcast(LastPassInfo);
}

void ABlazerGameState::CreateNewGoalInfo(ABlazerPlayerState* ScoringPlayer, ABlazerPlayerState* AssistingPlayer, EGoalTypes GoalType,
		int ScoringTeam, float GoalDistance, int BounceCount, int PreviousTeamInLead, int NewTeamInLead, bool bLongGoal)
{
	LastGoalInfo.ScoringPlayer = ScoringPlayer;
	LastGoalInfo.GoalAssistPlayer = AssistingPlayer;
	LastGoalInfo.GoalType = GoalType;
	LastGoalInfo.ScoringTeam = ScoringTeam;
	LastGoalInfo.DistanceTravelledToGoal = GoalDistance;
	LastGoalInfo.BallValue = GetCurrentBallValue();
	LastGoalInfo.BounceCount = BounceCount;
	LastGoalInfo.PreviousTeamInLead = PreviousTeamInLead;
	LastGoalInfo.NewTeamInLead = NewTeamInLead;
	LastGoalInfo.bLongGoal = bLongGoal;
	LastGoalInfo.bHattrickGoal = false;
	LastGoalInfo.GoalCharge = " ";

	OnNewGoalInfo.Broadcast(LastGoalInfo);
}

void ABlazerGameState::CreateNewKillInfo(ABlazerPlayerState* Killer, ABlazerPlayerState* Victim,
	ABlazerPlayerState* VictimLastTarget, TArray<ABlazerPlayerState*> AssistingPlayers, float KillDistance, bool bHeadshotKill,
	bool bDefendingKill, bool bSavingKill, bool bRevengeKill, bool bComebackKill, bool bDoubleKill)
{
	//Update Last Kill Info
	LastKillInfo.AssistingPlayers = AssistingPlayers;
	LastKillInfo.KillingPlayer = Killer;
	LastKillInfo.DeadPlayer = Victim;
	LastKillInfo.DeadPlayerLastTarget = VictimLastTarget;
	LastKillInfo.DistanceToTarget = KillDistance;
	LastKillInfo.bHeadshotKill = bHeadshotKill;
	LastKillInfo.bDefendingKill = bDefendingKill;
	LastKillInfo.bSavingKill = bSavingKill;
	LastKillInfo.bRevengeKill = bRevengeKill;
	LastKillInfo.bComebackKill = bComebackKill;
	LastKillInfo.bDoubleKill = bDoubleKill;
	
	//Broadcast OnKillInfo Event
	OnNewKillInfo.Broadcast(LastKillInfo);
}

void ABlazerGameState::SetTimer(int Minutes, int Seconds)
{
	GameTimer = Seconds + (Minutes * 60);
}

void ABlazerGameState::StartTimer()
{
	bIsTimerOn = true;
}

void ABlazerGameState::StopTimer()
{
	bIsTimerOn = false;
}

void ABlazerGameState::IncrementTimer(float DeltaTime)
{
	if(bIsTimerOn)
	{
		GameTimer = FMath::Max(0, GameTimer - DeltaTime);
	}
}

float ABlazerGameState::GetTimeLeft()
{
	return GameTimer;
}

int ABlazerGameState::GetCurrentBallValue()
{
	int FinalChargeScore = CurrentChargeScore;

	if(CurrentChargeScore >= 100)
	{
		FinalChargeScore *= 4;
	}
	else if(CurrentChargeScore > 70)
	{
		FinalChargeScore *= 3;
	}
	else if(CurrentChargeScore > 50)
	{
		FinalChargeScore *= 2;
	}
	
	return CurrentBallScoreValue + FinalChargeScore;
}

void ABlazerGameState::SetChargeScore(const int Value)
{
	if(ConvertChargeScoreThreshold(Value) != ConvertChargeScoreThreshold(CurrentChargeScore))
	{
		OnChangeChargeScoreThreshold.Broadcast(Value, ConvertChargeScoreThreshold(Value));
	}
	CurrentChargeScore = Value;
}

EChargeScoreThreshold ABlazerGameState::ConvertChargeScoreThreshold(const int ChargeScore)
{
	if(ChargeScore >= 100) return EChargeScoreThreshold::Blazing;
	else if(ChargeScore > 70) return EChargeScoreThreshold::Searing;
	else if(ChargeScore > 50) return EChargeScoreThreshold::Hot;
	else return EChargeScoreThreshold::Cool;
}
