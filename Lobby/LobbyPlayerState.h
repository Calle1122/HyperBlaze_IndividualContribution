// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBALLBLAZERS_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Replicated)
	int32 Team;

	UFUNCTION(Blueprintable)
	int32 GetTeam(){ return Team;}

	UFUNCTION(Blueprintable, Server, Reliable)
	void SetTeam(int32 NewTeamNumber);

	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
