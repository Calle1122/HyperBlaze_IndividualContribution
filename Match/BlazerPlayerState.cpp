// Fill out your copyright notice in the Description page of Project Settings.


#include "BlazerPlayerState.h"
#include "Net/UnrealNetwork.h"

void ABlazerPlayerState::SetTeam_Implementation(int32 NewTeamNumber)
{
	Team = NewTeamNumber;
}

void ABlazerPlayerState::SetSkin_Implementation(int32 NewSkinNumber)
{
	Skin = NewSkinNumber;
}

void ABlazerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlazerPlayerState, Team)
	DOREPLIFETIME(ABlazerPlayerState, Skin)
	DOREPLIFETIME(ABlazerPlayerState, MatchStats)
	DOREPLIFETIME(ABlazerPlayerState, PlayerTracker)
	DOREPLIFETIME(ABlazerPlayerState, CharacterRef)
}

void ABlazerPlayerState::ClearRecentDamagers_Implementation()
{
	PlayerTracker.RecentDamagers.Empty();
}

void ABlazerPlayerState::AddUniqueRecentDamager_Implementation(ABlazerPlayerState* NewDamager)
{
	PlayerTracker.RecentDamagers.AddUnique(NewDamager);
}

void ABlazerPlayerState::ResetTimeSinceTakingDamage_Implementation()
{
	PlayerTracker.TimeSinceTakingDamage = 0;
}

void ABlazerPlayerState::IncrementTimeSinceTakingDamage_Implementation(float DeltaTime)
{
	PlayerTracker.TimeSinceTakingDamage += DeltaTime;
}

void ABlazerPlayerState::ResetTimeSinceLastAssist_Implementation()
{
	PlayerTracker.TimeSinceLastAssist = 0;
}

void ABlazerPlayerState::IncrementTimeSinceLastAssist_Implementation(float DeltaTime)
{
	PlayerTracker.TimeSinceLastAssist += DeltaTime;
}

void ABlazerPlayerState::ResetTimeSinceLastKill_Implementation()
{
	PlayerTracker.TimeSinceLastKill = 0;
}

void ABlazerPlayerState::IncrementTimeSinceLastKill_Implementation(float DeltaTime)
{
	PlayerTracker.TimeSinceLastKill += DeltaTime;
}

void ABlazerPlayerState::ResetConsecutiveDeaths_Implementation()
{
	PlayerTracker.ConsecutiveDeaths = 0;
}

void ABlazerPlayerState::AddConsecutiveDeath_Implementation()
{
	PlayerTracker.ConsecutiveDeaths ++;
}

void ABlazerPlayerState::SetLastDamager_Implementation(ABlazerPlayerState* LastDamager)
{
	PlayerTracker.LastDamager = LastDamager;
}

void ABlazerPlayerState::SetLastKiller_Implementation(ABlazerPlayerState* LastKiller)
{
	PlayerTracker.LastKiller = LastKiller;
}

void ABlazerPlayerState::SetLastDealtDamageAmount_Implementation(float Amount)
{
	PlayerTracker.LastDealtDamageAmount = Amount;
}

void ABlazerPlayerState::SetLastDamagedEnemy_Implementation(ABlazerPlayerState* DamagedEnemy)
{
	PlayerTracker.LastDamagedEnemy = DamagedEnemy;
}

void ABlazerPlayerState::AddOrUpdateAssistStat_Implementation(EMatchAssistTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchAssistTypes::AssistCount:
		MatchStats.AssistMatchStats.MatchAssistCount += Amount;
		break;
	case EMatchAssistTypes::DoubleAssistCount:
		MatchStats.AssistMatchStats.DoubleAssistCount += Amount;
		break;
	default: ;
	}
}

void ABlazerPlayerState::AddOrUpdateKillStat_Implementation(EMatchKillTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchKillTypes::KillCount:
		MatchStats.KillMatchStats.MatchKillCount += Amount;
		break;
	case EMatchKillTypes::DoubleKillCount:
		MatchStats.KillMatchStats.DoubleKillCount += Amount;
		break;
	case EMatchKillTypes::DefendingKillCount:
		MatchStats.KillMatchStats.DefendingKillCount += Amount;
		break;
	case EMatchKillTypes::SavingKillCount:
		MatchStats.KillMatchStats.SavingKillCount += Amount;
		break;
	case EMatchKillTypes::LongestKillRecord:
		MatchStats.KillMatchStats.LongestKillRecord = Amount;
		break;
	default: ;
	}
}

void ABlazerPlayerState::AddOrUpdatePassStat_Implementation(EMatchPassTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchPassTypes::PassCount:
		MatchStats.PassMatchStats.MatchPassCount += Amount;
		break;
	case EMatchPassTypes::GoalAssistCount:
		MatchStats.PassMatchStats.MatchGoalAssistCount += Amount;
		break;
	case EMatchPassTypes::StealCount:
		MatchStats.PassMatchStats.MatchStealCount += Amount;
		break;
	case EMatchPassTypes::LongestPassRecord:
		MatchStats.PassMatchStats.LongestPassRecord = Amount;
		break;
	case EMatchPassTypes::StealValueRecord:
		MatchStats.PassMatchStats.StealValueRecord = Amount;
		break;
	default: ;
	}
}

void ABlazerPlayerState::AddOrUpdateDeathStat_Implementation(EMatchDeathTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchDeathTypes::DeathCount:
		MatchStats.DeathMatchStats.MatchDeathCount += Amount;
		break;
	default: ;
	}
}

void ABlazerPlayerState::AddOrUpdateMiscStat_Implementation(EMatchMiscTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchMiscTypes::BallControlTime:
		MatchStats.MiscMatchStats.BallControlTime += Amount;
		break;
	case EMatchMiscTypes::BallHoldTime:
		MatchStats.MiscMatchStats.BallHoldTime += Amount;
		break;
	case EMatchMiscTypes::TotalPlayerScore:
		MatchStats.MiscMatchStats.TotalPlayerScore += Amount;
		break;
	default: ;
	}
}

void ABlazerPlayerState::AddOrUpdateGoalStat_Implementation(EMatchGoalTypes StatType, float Amount)
{
	switch (StatType) {
	case EMatchGoalTypes::GoalCount:
		MatchStats.GoalMatchStats.MatchGoalCount += Amount;
		break;
	case EMatchGoalTypes::ThrownGoalCount:
		MatchStats.GoalMatchStats.MatchThrowGoalCount += Amount;
		break;
	case EMatchGoalTypes::DunkGoalCount:
		MatchStats.GoalMatchStats.MatchDunkGoalCount += Amount;
		break;
	case EMatchGoalTypes::LongestGoalRecord:
		MatchStats.GoalMatchStats.LongestGoalRecord = Amount;
		break;
	case EMatchGoalTypes::HighestBounceGoalRecord:
		MatchStats.GoalMatchStats.HighestBounceGoalRecord = Amount;
		break;
	case EMatchGoalTypes::GoalValueRecord:
		MatchStats.GoalMatchStats.GoalValueRecord = Amount;
		break;
	default: ;
	}
}
