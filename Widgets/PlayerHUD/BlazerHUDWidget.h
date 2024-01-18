// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChildWidgets/AmmoCounterWidget.h"
#include "ChildWidgets/CrosshairWidget.h"
#include "ChildWidgets/DamageNumberWidget.h"
#include "ChildWidgets/HealthbarWidget.h"
#include "ChildWidgets/MatchTimerWidget.h"
#include "ChildWidgets/MiniMapWidget.h"
#include "ChildWidgets/PlayerActionFeedWidget.h"
#include "ChildWidgets/PlayerStanceWidget.h"
#include "ChildWidgets/MatchScoreWidget/MatchScoreWidget.h"
#include "ProjectBallBlazers/Widgets/Scoreboard/ScoreboardWidget.h"
#include "BlazerHUDWidget.generated.h"

UCLASS(Abstract)
class PROJECTBALLBLAZERS_API UBlazerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	//
	/** CHILD WIDGETS **/
	//
	
	//Crosshair
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UCrosshairWidget* Crosshair;
	
	//Health Bar
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHealthbarWidget* HealthBar;

	//Ammo Counter
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAmmoCounterWidget* AmmoCounter;

	//Match Score
	// - has child widgets in it
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMatchScoreWidget* MatchScore;

	//Match Timer
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMatchTimerWidget* MatchTimer;

	//Damage Numbers
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDamageNumberWidget* DamageNumbers;

	//Player Action Feed
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPlayerActionFeedWidget* PlayerActionFeed;

	//Mini Map
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UMiniMapWidget* MiniMap;

	//Stance Icon
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPlayerStanceWidget* StanceIcon;

	//Scoreboard
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScoreboardWidget* Scoreboard;

	//
	//** HUD FUNCTIONS **//
	//

	//Toggle Scoreboard
	UFUNCTION(BlueprintCallable, Category = "Blazer HUD Functions")
	void ToggleScoreboardVisibility();

	UFUNCTION(BlueprintCallable, Category = "Blazer HUD Functions")
	void SetGameplayHUDVisibility(bool bState);

	UFUNCTION(BlueprintCallable, Category = "Blazer HUD Functions")
	void SetGunHUDVisibility(bool bState);

protected:
	//
	//** INTERNAL VARIABLES **//
	//

	//Current Scoreboard Visibility
	bool bScoreboardIsVisible;
	bool bGameplayElementsIsVisible;
	bool bGunElementsIsVisible;
};
