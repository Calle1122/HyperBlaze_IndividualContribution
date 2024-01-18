// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreMultiplier.h"
#include "BlazeBall.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBallBlazers/Match/BlazerGameState.h"

AScoreMultiplier::AScoreMultiplier()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Create collider
	Collider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collider"));

	// Create ring mesh
	//Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	//Ring->SetupAttachment(Collider);
}

void AScoreMultiplier::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &AScoreMultiplier::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this, &AScoreMultiplier::OnOverlapExit);
	Collider->SetVisibility(false);
	IsOpen = true;
}

void AScoreMultiplier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Replicates variables
void AScoreMultiplier::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AScoreMultiplier, IsOpen)
}

void AScoreMultiplier::SetOpenState(bool NewIsOpen)
{
	IsOpen = NewIsOpen;
	
	if(IsOpen)
	{
		//Set materials
		//Collider->SetMaterial(0, ActiveInsideMaterial);
		OnOpen();
		//Ring->SetMaterial(0, ActiveRingMaterial);
	}
	else
	{
		//Set materials
		//Collider->SetMaterial(0, InActiveInsideMaterial);
		OnClose();
		//Ring->SetMaterial(0, InActiveRingMaterial);
	}
}

/*
void AScoreMultiplier::ServerSetOpenState_Implementation(bool NewIsOpen)
{
	MulticastSetOpenState(NewIsOpen);
}

void AScoreMultiplier::MulticastSetOpenState_Implementation(bool NewIsOpen)
{
	IsOpen = NewIsOpen;
	
	if(IsOpen)
	{
		Collider->SetCollisionProfileName(FName("OverlapAll"));

		//Set materials
		Collider->SetMaterial(0, ActiveInsideMaterial);
		Ring->SetMaterial(0, ActiveRingMaterial);
	}
	else
	{
		Collider->SetCollisionProfileName(FName("BlockAll"));

		//Set materials
		Collider->SetMaterial(0, InActiveInsideMaterial);
		Ring->SetMaterial(0, InActiveRingMaterial);
	}
}
*/

void AScoreMultiplier::OnOverlapBegin(UPrimitiveComponent* Primitive, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsOpen)
	{
		return;
	}

	/* Deprecated
	
	if(ABlazeBall* Ball = Cast<ABlazeBall>(OtherActor))
	{
		ABlazerGameState* gameState = GetWorld()->GetGameState<ABlazerGameState>();
		if(gameState->CurrentBallScoreMultiplier == 1)
		{
			gameState->IncreaseBallMultiplier(1);
		}
		else
		{
			gameState->IncreaseBallMultiplier(2);
		}
	} */
}

void AScoreMultiplier::OnOverlapExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(ABlazeBall* Ball = Cast<ABlazeBall>(OtherActor))
	{
		SetOpenState(false);
		
	}
}
