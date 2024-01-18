// Fill out your copyright notice in the Description page of Project Settings.

#include "BlazeBallSpawner.h"

#include "BlazeBall.h"
#include "Kismet/KismetSystemLibrary.h"

ABlazeBallSpawner::ABlazeBallSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create platform mesh
	SpawnerPlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	// Create arrow component
	BallSpawnPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Position"));
	BallSpawnPosition->SetupAttachment(SpawnerPlatformMesh);

	SpawnDelay = 5.f;
}

void ABlazeBallSpawner::BeginPlay()
{
	Super::BeginPlay();

	ServerSpawnBall();
}

void ABlazeBallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlazeBallSpawner::ServerSpawnBall_Implementation()
{
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ABlazeBallSpawner::SpawnBall, SpawnDelay, false);
}

void ABlazeBallSpawner::SpawnBall() const
{
	FTransform Transform = BallSpawnPosition->GetComponentTransform();
	Transform.SetRotation(FRotator(0, 0, 0).Quaternion());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABlazeBall* NewBall = GetWorld()->SpawnActor<ABlazeBall>(BallActorBP, Transform, SpawnParams);
	NewBall->ServerFreeze();
}
