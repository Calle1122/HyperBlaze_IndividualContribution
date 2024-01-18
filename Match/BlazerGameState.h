// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ProjectBallBlazers/Stats/ActionProperties/GoalProperties.h"
#include "ProjectBallBlazers/Stats/ActionProperties/KillProperties.h"
#include "ProjectBallBlazers/Stats/ActionProperties/PassProperties.h"
#include "BlazerGameState.generated.h"

UENUM(BlueprintType)
enum class EChargeScoreThreshold : uint8
{
	Cool,
	Hot,
	Searing,
	Blazing
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewKillInfo, FKillProperties, KillInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChargeScoreThreshold, int, ChargeScore, EChargeScoreThreshold, ChargeScoreThreshold);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPassInfo, FPassProperties, PassInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewGoalInfo, FGoalProperties, GoalInfo);

UCLASS()
class PROJECTBALLBLAZERS_API ABlazerGameState : public AGameState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Replicated)
	int Team0Score;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int Team1Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball Variables")
	int CurrentBallScoreValue = 50;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball Variables")
	int CurrentChargeScore = 0;

	
	float CurrentMultiDecayTimer;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Setup Variables")
	float SecondsBetweenEachMultiplierDecay;

	UPROPERTY(Replicated)
	bool bIsTimerOn;
	UPROPERTY(Replicated)
	float GameTimer;

	// Replicated game trackers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Game Trackers")
	FPassProperties LastPassInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Game Trackers")
	FGoalProperties LastGoalInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Game Trackers")
	FKillProperties LastKillInfo;

	// Game tracker delegates
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnNewKillInfo"), Category = "Game Tracker Events")
	FOnNewKillInfo OnNewKillInfo;
	// Game tracker delegates
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnNewPassInfo"), Category = "Game Tracker Events")
	FOnNewPassInfo OnNewPassInfo;
	// Game tracker delegates
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnNewGoalInfo"), Category = "Game Tracker Events")
	FOnNewGoalInfo OnNewGoalInfo;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "OnChangeChargeScoreThreshold"), Category = "Game Tracker Events")
	FOnChargeScoreThreshold OnChangeChargeScoreThreshold;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	/** External Functions */

	ABlazerGameState();
	UFUNCTION(BlueprintCallable, Category = "Blazer Game State Functions")
	void AddScore(int Team, int Amount);
	
	UFUNCTION(BlueprintCallable, Category = "Ball Functions")
	void SetBallScoreBaseValue(int NewBaseScoreValue);
	UFUNCTION(BlueprintCallable, Category = "Ball Functions")
	void ResetCurrentBallScoreAndMultiplier();
	UFUNCTION(BlueprintCallable, Category = "Ball Functions")
	int GetCurrentBallValue();
	
	void SetChargeScore(int Value);
	UFUNCTION(BlueprintCallable)
	static EChargeScoreThreshold ConvertChargeScoreThreshold(int ChargeScore);

	//Game Tracker Update Events
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void CreateNewPassInfo(ABlazerPlayerState* Passer, ABlazerPlayerState* Receiver, float Distance);
	UFUNCTION(BlueprintCallable)
	void CreateNewGoalInfo(ABlazerPlayerState* ScoringPlayer, ABlazerPlayerState* AssistingPlayer, EGoalTypes GoalType,
		int ScoringTeam, float GoalDistance, int BounceCount, int PreviousTeamInLead, int NewTeamInLead, bool bLongGoal);
	UFUNCTION(BlueprintCallable)
	void CreateNewKillInfo(ABlazerPlayerState* Killer, ABlazerPlayerState* Victim, ABlazerPlayerState* VictimLastTarget,
	TArray<ABlazerPlayerState*> AssistingPlayers, float KillDistance, bool bHeadshotKill, bool bDefendingKill, bool bSavingKill,
	bool bRevengeKill, bool bComebackKill, bool bDoubleKill);

	UFUNCTION(BlueprintCallable, Category = "Game Timer Functions")
	void SetTimer(int Minutes, int Seconds);
	UFUNCTION(BlueprintCallable, Category = "Game Timer Functions")
	void StartTimer();
	UFUNCTION(BlueprintCallable, Category = "Game Timer Functions")
	void StopTimer();
	UFUNCTION(BlueprintCallable, Category = "Game Timer Functions")
	void IncrementTimer(float DeltaTime);
	UFUNCTION(BlueprintCallable, Category = "Game Timer Functions")
	float GetTimeLeft();
};
