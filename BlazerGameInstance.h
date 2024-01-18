// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "BlazerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
// class PROJECTBALLBLAZERS_API UBlazerGameInstance : public UGameInstance//: public UAdvancedFriendsGameInstance
class PROJECTBALLBLAZERS_API UBlazerGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
public:

	UBlazerGameInstance(const FObjectInitializer& ObjectInitializer);
	
	TMap<FString,int32> TeamMap;
	TMap<FString, int32> SkinMap;

	UFUNCTION(BlueprintCallable)
	int32 GetTeam(FString PlayerName);
	UFUNCTION(BlueprintCallable)
	int32 GetSkin(FString PlayerName);

	UFUNCTION(BlueprintCallable)
	void SetTeam(FString PlayerName, int32 NewTeamNumber);
	UFUNCTION(BlueprintCallable)
	void SetSkin(FString PlayerName, int32 NewSkinIndex);

	UFUNCTION(BlueprintCallable)
	void PrintTeams();

	UFUNCTION(BlueprintCallable)
	bool CheckTeamMapForPlayer(FString PlayerName);
	
};
