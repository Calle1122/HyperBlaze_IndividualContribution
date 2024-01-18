// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectBallBlazers/BlazerPlayer/BlazerPlayerController.h"
#include "BlazerGameMode.generated.h"

//Forward Declarations
class ARespawnPoint;


/**
 * 
 */
UCLASS()
class PROJECTBALLBLAZERS_API ABlazerGameMode : public AGameMode
{
	GENERATED_BODY()

	ABlazerGameMode();

	virtual void BeginPlay() override;

	TMap<int32,TArray<ARespawnPoint*>> RespawnPoints;
public:
	virtual void GenericPlayerInitialization(AController* C) override;

	void SetTeamForPlayer(ABlazerPlayerController* PlayerController);

	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION(BlueprintCallable)
	ARespawnPoint* GetRespawnPoint(ABlazerPlayerController* PlayerController);

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

};
