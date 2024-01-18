// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "BlazeBallSpawner.generated.h"

UCLASS()
class PROJECTBALLBLAZERS_API ABlazeBallSpawner : public AActor
{
	GENERATED_BODY()

public:
	ABlazeBallSpawner();

	//Mesh Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> SpawnerPlatformMesh;
	//Arrow Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Position")
	TObjectPtr<UArrowComponent> BallSpawnPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Spawner Setup Variables")
	TSubclassOf<AActor> BallActorBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner Setup Variables")
	float SpawnDelay;

public:
	/** External Spawner Functions **/
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Ball Spawner Functions")
	void ServerSpawnBall();

protected:
	UFUNCTION()
	void SpawnBall() const;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
