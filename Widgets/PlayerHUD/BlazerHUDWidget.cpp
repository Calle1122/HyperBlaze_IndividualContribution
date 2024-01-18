// Fill out your copyright notice in the Description page of Project Settings.

#include "BlazerHUDWidget.h"

void UBlazerHUDWidget::ToggleScoreboardVisibility()
{
	if(bScoreboardIsVisible)
	{
		Scoreboard->SetVisibility(ESlateVisibility::Collapsed);

		MatchScore->SetVisibility(ESlateVisibility::Visible);
		if(bGameplayElementsIsVisible && bGunElementsIsVisible)
		{
			Crosshair->SetVisibility(ESlateVisibility::Visible);	
		}
		
		
		//TODO: bring back commented lines when their UI is implemented
		
		//MiniMap->SetVisibility(ESlateVisibility::Visible);
		//PlayerActionFeed->SetVisibility(ESlateVisibility::Visible);
		//DamageNumbers->SetVisibility(ESlateVisibility::Visible);
		
		bScoreboardIsVisible = false;
	}
	else
	{
		Scoreboard->SetVisibility(ESlateVisibility::Visible);

		MatchScore->SetVisibility(ESlateVisibility::Collapsed);
		Crosshair->SetVisibility(ESlateVisibility::Collapsed);

		//TODO: bring back commented lines when their UI is implemented
		
		//MiniMap->SetVisibility(ESlateVisibility::Collapsed);
		//PlayerActionFeed->SetVisibility(ESlateVisibility::Collapsed);
		//DamageNumbers->SetVisibility(ESlateVisibility::Collapsed);
		
		bScoreboardIsVisible = true;
	}
}

void UBlazerHUDWidget::SetGameplayHUDVisibility(const bool bState)
{
	bGameplayElementsIsVisible = bState;

	if(bState)
	{
		if(bGunElementsIsVisible)
		{
			Crosshair->SetVisibility(ESlateVisibility::Visible);
			AmmoCounter->SetVisibility(ESlateVisibility::Visible);
		}
		HealthBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Crosshair->SetVisibility(ESlateVisibility::Collapsed);
		AmmoCounter->SetVisibility(ESlateVisibility::Collapsed);
		HealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBlazerHUDWidget::SetGunHUDVisibility(const bool bState)
{
	bGunElementsIsVisible = bState;

	if(bState)
	{
		if(bGameplayElementsIsVisible)
		{
			Crosshair->SetVisibility(ESlateVisibility::Visible);
			Crosshair->OnShowCrosshair();
			AmmoCounter->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		Crosshair->SetVisibility(ESlateVisibility::Visible);
		Crosshair->OnShowDot();
		AmmoCounter->SetVisibility(ESlateVisibility::Collapsed);
	}
}
