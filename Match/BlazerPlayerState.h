// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectBallBlazers/Stats/MatchStats/Player/PlayerMatchStats.h"
#include "ProjectBallBlazers/Stats/MatchStats/Player/Categories/CategoryTypes/StatCategoryTypes.h"
#include "ProjectBallBlazers/Stats/Trackers/PlayerTracker.h"
#include "ProjectBallBlazers/Widgets/PlayerHUD/NetworkedBlazerHUDWidget.h"
#include "BlazerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBALLBLAZERS_API ABlazerPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	//Variables
	
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 Team;
	UPROPERTY(BlueprintReadWrite, Replicated)
	int32 Skin;

	UPROPERTY(BlueprintReadWrite, Replicated)
	ABlazerCharacter* CharacterRef;

	UPROPERTY(BlueprintReadWrite, Replicated)
	FPlayerMatchStats MatchStats;
	UPROPERTY(BlueprintReadWrite, Replicated)
	FPlayerTracker PlayerTracker;
	
	UPROPERTY(BlueprintReadWrite)
	UNetworkedBlazerHUDWidget* NetworkedHUDRef;

	//Functions
	
	UFUNCTION(BlueprintCallable)
	int32 GetTeam(){ return Team;}
	UFUNCTION(BlueprintCallable)
	int32 GetSkin(){ return Skin;}

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetTeam(int32 NewTeamNumber);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void SetSkin(int32 NewSkinNumber);

	//Replicated Variables
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**/
	//** EXTERNAL STAT SETTERS **/
	/**/

	//Assist
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdateAssistStat(EMatchAssistTypes StatType, float Amount);

	//Kill
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdateKillStat(EMatchKillTypes StatType, float Amount);

	//Pass
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdatePassStat(EMatchPassTypes StatType, float Amount);
	
	//Death
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdateDeathStat(EMatchDeathTypes StatType, float Amount);
	
	//Misc
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdateMiscStat(EMatchMiscTypes StatType, float Amount);
	
	//Goal
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Stat Functions")
	void AddOrUpdateGoalStat(EMatchGoalTypes StatType, float Amount);
	
	/**/
	/**/
	/**/

	/**/
	//** EXTERNAL TRACKER SETTERS **/
	/**/

	//Last Damaged Enemy
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void SetLastDamagedEnemy(ABlazerPlayerState* DamagedEnemy);

	//Last Killer
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void SetLastKiller(ABlazerPlayerState* LastKiller);

	//Last Damager
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void SetLastDamager(ABlazerPlayerState* LastDamager);

	//Add Unique Recent Damager
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void AddUniqueRecentDamager(ABlazerPlayerState* NewDamager);

	//Clear Recent Damagers
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void ClearRecentDamagers();
	
	//Last Dealt Damage Amount
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void SetLastDealtDamageAmount(float Amount);

	//Increment Time Since Last Kill
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void IncrementTimeSinceLastKill(float DeltaTime);

	//Reset Time Since Last Kill
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void ResetTimeSinceLastKill();

	//Increment Time Since Last Assist
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void IncrementTimeSinceLastAssist(float DeltaTime);

	//Reset Time Since Last Assist
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void ResetTimeSinceLastAssist();

	//Increment Time Since Taking Damage
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void IncrementTimeSinceTakingDamage(float DeltaTime);

	//Reset Time Since Taking Damage
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void ResetTimeSinceTakingDamage();

	//Add Consecutive Deaths
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void AddConsecutiveDeath();

	//Reset Consecutive Deaths
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Player Tracker Functions")
	void ResetConsecutiveDeaths();
	
	/**/
	/**/
	/**/
};
