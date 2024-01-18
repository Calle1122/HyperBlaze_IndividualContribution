// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"

#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::SetTeam_Implementation(int32 NewTeamNumber)
{
	Team = NewTeamNumber;
}


void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, Team)
}