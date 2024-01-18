#pragma once

#include "CoreMinimal.h"
#include "PassProperties.generated.h"

class ABlazerPlayerState;

USTRUCT(BlueprintType)
struct FPassProperties
{
	GENERATED_BODY()

	FPassProperties();
	~FPassProperties();

	/** ACTORS */

	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Actors")
	ABlazerPlayerState* PassingPlayer;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Actors")
	ABlazerPlayerState* ReceivingPlayer;

	/** PASS INFO */

	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	float PassDistance;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	float DistanceToGoal;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	float BallValue;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	bool bIsSteal;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	bool bDefendingSteal;
	UPROPERTY(BlueprintReadWrite, Category = "Pass Properties Info")
	bool bLongPass;
};

