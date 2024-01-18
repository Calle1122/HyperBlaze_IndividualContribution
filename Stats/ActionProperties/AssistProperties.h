#pragma once

#include "CoreMinimal.h"
#include "ProjectBallBlazers/Match/BlazerPlayerState.h"
#include "AssistProperties.generated.h"

USTRUCT(BlueprintType)
struct FAssistProperties
{
	GENERATED_BODY()

	FAssistProperties();
	~FAssistProperties();

	/** ACTORS */

	UPROPERTY(BlueprintReadWrite, Category = "Assist Properties Actors")
	ABlazerPlayerState* KillingPlayer;
	UPROPERTY(BlueprintReadWrite, Category = "Assist Properties Actors")
	ABlazerPlayerState* DeadPlayer;

	/** ASSIST INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Assist Properties Info")
	bool bBackBreaker;
	UPROPERTY(BlueprintReadWrite, Category = "Assist Properties Info")
	bool bDoubleAssist;
};

