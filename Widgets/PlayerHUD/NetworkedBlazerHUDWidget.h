// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChildWidgets/AnnouncerFeedWidget.h"
#include "ChildWidgets/KillFeedWidget.h"
#include "NetworkedBlazerHUDWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTBALLBLAZERS_API UNetworkedBlazerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Kill Feed
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UKillFeedWidget* KillFeed;
	
	//Announcer Feed
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UAnnouncerFeedWidget* AnnouncerFeed;
};
