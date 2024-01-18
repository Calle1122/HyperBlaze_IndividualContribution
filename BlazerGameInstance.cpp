// Fill out your copyright notice in the Description page of Project Settings.


#include "BlazerGameInstance.h"


UBlazerGameInstance::UBlazerGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TeamMap = TMap<FString, int32>();
	SkinMap = TMap<FString, int32>();
}

int32 UBlazerGameInstance::GetTeam(FString PlayerName)
{
	if(TeamMap.Contains(PlayerName))
	{
		return TeamMap[PlayerName];
	}

	return 999;
}

int32 UBlazerGameInstance::GetSkin(FString PlayerName)
{
	if(SkinMap.Contains(PlayerName))
	{
		return SkinMap[PlayerName];
	}

	return 999;
}

void UBlazerGameInstance::SetTeam(FString PlayerName, int32 NewTeamNumber)
{
	if(TeamMap.Contains(PlayerName))
	{
		TeamMap[PlayerName] = NewTeamNumber;
	}
	else
	{
		TeamMap.Add(PlayerName, NewTeamNumber);
		FString x;
	}
}

void UBlazerGameInstance::SetSkin(FString PlayerName, int32 NewSkinIndex)
{
	if(SkinMap.Contains(PlayerName))
	{
		SkinMap[PlayerName] = NewSkinIndex;
	}
	else
	{
		SkinMap.Add(PlayerName, NewSkinIndex);
		FString x;
	}
}

void UBlazerGameInstance::PrintTeams()
{
	for (auto Element : TeamMap)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Player: %s Team: %d"), *Element.Key, Element.Value));
	}
}

bool UBlazerGameInstance::CheckTeamMapForPlayer(FString PlayerName)
{
	return TeamMap.Contains(PlayerName);
}
