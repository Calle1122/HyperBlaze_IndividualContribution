// Fill out your copyright notice in the Description page of Project Settings.


#include "BlazerGameMode.h"

#include "AdvancedSessionsLibrary.h"
#include "BlazerPlayerState.h"
#include "EngineUtils.h"
#include "Engine/PlayerStartPIE.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectBallBlazers/BlazerGameInstance.h"
#include "ProjectBallBlazers/PlayerHandling/RespawnPoint.h"

// bool ABlazerGameMode::ReadyToStartMatch_Implementation()
// {
// 	Super::ReadyToStartMatch_Implementation();
//
// 	return GetNumPlayers() == 4;
// }

void ABlazerGameMode::GenericPlayerInitialization(AController* C)
{
	SetTeamForPlayer(Cast<ABlazerPlayerController>(C));
	Super::GenericPlayerInitialization(C);
}

// void ABlazerGameMode::OnPostLogin(AController* NewPlayer)
void ABlazerGameMode::SetTeamForPlayer(ABlazerPlayerController* PlayerController)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SetTeamForPlayer"));
	// Super::OnPostLogin(NewPlayer);
	ABlazerPlayerState* PlayerState = Cast<ABlazerPlayerState>(PlayerController->PlayerState);
	UBlazerGameInstance* GameInstance = Cast<UBlazerGameInstance>(GetGameInstance());
	if (PlayerState && GameInstance )
	{
		if(GameInstance->TeamMap.Contains(PlayerState->GetUniqueId().GetUniqueNetId()->ToString()))
		{
			PlayerState->SetTeam(GameInstance->GetTeam(PlayerState->GetUniqueId().GetUniqueNetId()->ToString()));
		}
		else
		{
			// GameInstance->SetTeam(PlayerState->GetPlayerName(), 0);
			GameInstance->SetTeam(PlayerState->GetUniqueId().GetUniqueNetId()->ToString(), 0);
		}

		//Skin setup
		if(GameInstance->SkinMap.Contains(PlayerState->GetUniqueId().GetUniqueNetId()->ToString()))
		{
			PlayerState->SetSkin(GameInstance->GetSkin(PlayerState->GetUniqueId().GetUniqueNetId()->ToString()));
		}
		else
		{
			GameInstance->SetSkin(PlayerState->GetUniqueId().GetUniqueNetId()->ToString(), 0);
			PlayerState->SetSkin(GameInstance->GetSkin(PlayerState->GetUniqueId().GetUniqueNetId()->ToString()));
		}
	}
}

AActor* ABlazerGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Find Player StartTeam %d"), Cast<ABlazerPlayerState>(Player->PlayerState)->GetTeam()));
	// return Super::FindPlayerStart_Implementation(Player, IncomingName);
	
	// Choose a player start
	APlayerStart* FoundPlayerStart = nullptr;
	UClass* PawnClass = GetDefaultPawnClassForController(Player);
	APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	TArray<APlayerStart*> UnOccupiedStartPointsTaggedForTeam;
	TArray<APlayerStart*> OccupiedStartPointsTaggedForTeam;
	TArray<APlayerStart*> UnOccupiedStartPointsUntagged;
	TArray<APlayerStart*> OccupiedStartPointsUntagged;
	TArray<APlayerStart*> UnOccupiedStartPointsTaggedForOtherTeam;
	TArray<APlayerStart*> OccupiedStartPointsTaggedForOtherTeam;
	UWorld* World = GetWorld();
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (PlayerStart->IsA<APlayerStartPIE>())
		{
			// Always prefer the first "Play from Here" PlayerStart, if we find one while in PIE mode
			FoundPlayerStart = PlayerStart;
			break;
		}
		if (PlayerStart && PlayerStart->PlayerStartTag == FName(FString::FromInt(Cast<ABlazerPlayerState>(Player->PlayerState)->GetTeam())))
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPointsTaggedForTeam.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPointsTaggedForTeam.Add(PlayerStart);
			}
		}
		else if (PlayerStart->PlayerStartTag == "")
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPointsUntagged.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPointsUntagged.Add(PlayerStart);
			}
		}
		else
		{
			FVector ActorLocation = PlayerStart->GetActorLocation();
			const FRotator ActorRotation = PlayerStart->GetActorRotation();
			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
			{
				UnOccupiedStartPointsTaggedForOtherTeam.Add(PlayerStart);
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				OccupiedStartPointsTaggedForOtherTeam.Add(PlayerStart);
			}
		}
	}
	if (FoundPlayerStart == nullptr)
	{
		if (UnOccupiedStartPointsTaggedForTeam.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPointsTaggedForTeam[FMath::RandRange(0, UnOccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else if (OccupiedStartPointsTaggedForTeam.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPointsTaggedForTeam[FMath::RandRange(0, OccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else if (UnOccupiedStartPointsUntagged.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPointsUntagged[FMath::RandRange(0, UnOccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else if (OccupiedStartPointsUntagged.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPointsUntagged[FMath::RandRange(0, OccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else if (UnOccupiedStartPointsTaggedForOtherTeam.Num() > 0)
		{
			FoundPlayerStart = UnOccupiedStartPointsTaggedForOtherTeam[FMath::RandRange(0, UnOccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else if (OccupiedStartPointsTaggedForOtherTeam.Num() > 0)
		{
			FoundPlayerStart = OccupiedStartPointsTaggedForOtherTeam[FMath::RandRange(0, OccupiedStartPointsTaggedForTeam.Num() - 1)];
		}
		else
		{
			FoundPlayerStart = Cast<APlayerStart>(World->GetWorldSettings());
		}
	}
	return FoundPlayerStart;
}

AActor* ABlazerGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("ChoosePlayerStart BLazer Controller %s"), *Player->GetName()));
	return Super::ChoosePlayerStart_Implementation(Player);
}

ARespawnPoint* ABlazerGameMode::GetRespawnPoint(ABlazerPlayerController* PlayerController)
{
	ABlazerPlayerState* PlayerState = Cast<ABlazerPlayerState>(PlayerController->PlayerState);
	if(RespawnPoints.Contains(PlayerState->GetTeam()))
	{
		TArray<ARespawnPoint*> RespawnPointsForTeam = RespawnPoints[PlayerState->GetTeam()];
		if(RespawnPointsForTeam.Num() > 0)
		{
			return RespawnPointsForTeam[FMath::RandRange(0, RespawnPointsForTeam.Num() - 1)];
		}
	}
	return nullptr;
}

void ABlazerGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	// TODO Uncomment this when packaging for reals
	// //?Name=Jin who dis?SplitscreenCount=1
	// TArray<FString> ParsedOptions;
	// Options.ParseIntoArray(ParsedOptions, TEXT("?"), true);
	// // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, ParsedOptions[0].RightChop(5));
	// GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, ParsedOptions[0].RightChop(5) + " " + UniqueId.ToString());
	// if(!Cast<UBlazerGameInstance>(GetGameInstance())->CheckTeamMapForPlayer(ParsedOptions[0].RightChop(5)))
	// {
	// 	ErrorMessage = "Match Ongoing";
	// }
}

ABlazerGameMode::ABlazerGameMode()
{
	bUseSeamlessTravel = true;
}

void ABlazerGameMode::BeginPlay()
{
	for (TActorIterator<ARespawnPoint> It(GetWorld()); It; ++It)
	{
		if(!RespawnPoints.Contains(It->Team))
		{
			RespawnPoints.Add(It->Team,TArray<ARespawnPoint*>());
		}
		
		RespawnPoints[It->Team].Add(*It);
	}
}
