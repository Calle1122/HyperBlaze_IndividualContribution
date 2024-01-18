// Fill out your copyright notice in the Description page of Project Settings.

#include "BlazeBall.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBallBlazers/BlazerPlayer/BlazerCharacter.h"
#include "ProjectBallBlazers/BlazerPlayer/BlazerMovementComponent.h"
#include "ProjectBallBlazers/Components/HealthComponent.h"
#include "ProjectBallBlazers/Match/BlazerGameState.h"
#include "ProjectBallBlazers/Match/BlazerPlayerState.h"
#include "ProjectBallBlazers/Utility/CustomMath.h"

#define FLOOR_Z .75f
#define FLOOR_SLOPE_Z .99f
#define FLOOR_DIST_UPPER 5.0f
#define FLOOR_DIST 2.0f

ABlazeBall::ABlazeBall()
{
	MaxVelocity = 25000.f;
	AirDrag = 100.f;
	GroundedDrag = 300.f;
	SlopeGravityScale = 1.5f;
	BallVelocityOnDrop = 500.f;
	MinBounceVelocity = 50.f;
	GravityScaleMultiplierPerBounce = 1.5f;
	VelocityReductionMultiplierPerBounce = .9f;
	MinVelocityBeforeFreeze = 0.1f;
	MaxPickupVelocity = 2000.f;

	MinHitVelocity = 2000.f;
	MinDeathVelocity = 3000.f;
	HitKnockback = 600.f;
	HitVerticalKnockback = 200.f;
	HitDamage = 20.f;
	HitBounceReductionMultiplier = .75f;
	
	HoldStartDuration = 1.f;
	DecayStartDuration = .5f;
	ChargePerSeconds = 5.f;
	ChargeDecayPerSeconds = 3.3f;
	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->InitSphereRadius(50.f);
	SphereCollider->SetSimulatePhysics(false);

	SetRootComponent(SphereCollider);
	
	OverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapCollider"));
	OverlapCollider->InitSphereRadius(80.f);
	OverlapCollider->SetSimulatePhysics(false);
	OverlapCollider->SetupAttachment(SphereCollider);

	// Create ball mesh
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	BallMesh->SetupAttachment(SphereCollider);
	BallMesh->SetSimulatePhysics(false);
}

//Replicates variables
void ABlazeBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, CurrentBallState, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, bIsInteractable, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, CurrentVelocity, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, CurrentAirTime, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, LastTeamThatHeldBall, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, PreviousBallHolderPS, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, CurrentBallHolderPS, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, LastThrownPos, SharedParams)
	DOREPLIFETIME_WITH_PARAMS_FAST(ABlazeBall, LastPickedUpPos, SharedParams)
}

void ABlazeBall::BeginPlay()
{
	Super::BeginPlay();

	if(GetLocalRole() == ROLE_Authority) ServerBeginPlay();
}

void ABlazeBall::ServerBeginPlay_Implementation()
{
	ChangeBallState(EBallState::Airborne);

	OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &ABlazeBall::OnOverlap);
	
	CurrentAirTime = 0;
	bIsInteractable = true;
	ChangeChargeScore(0);
	ResetGravity();
}

void ABlazeBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentBallState == EBallState::DisabledPhysics)
	{
		if(GetLocalRole() == ROLE_Authority)
		{
			if(HoldCooldown <= 0)
			{
				ChangeChargeScore(CurrentChargeScore + ChargePerSeconds*DeltaTime);
			}
			else
			{
				HoldCooldown -= DeltaTime;
			}
		}
		return;
	}
		
	//Rotation Local
	if(!CurrentVelocity.IsZero())
	{
		const float RotationalSpeed = CurrentVelocity.Length()/SphereCollider->GetScaledSphereRadius();
		const FVector RotationDirection = FVector::CrossProduct(CurrentVelocity.GetSafeNormal(), FVector::UpVector);

		AddActorWorldRotation(FCustomMath::VectorToRotator(RotationalSpeed * DeltaTime * RotationDirection));
	}

	//Local Move for interpolation
	if(GetLocalRole() != ROLE_Authority)
	{
		AddActorWorldOffset(CurrentVelocity*DeltaTime, true);
		return; //Rest should only run on server
	}

	PhysicsTimer += DeltaTime;
	
	if(PhysicsTimer > 1.f/60.f)
	{
		ServerBallPhysics(PhysicsTimer);
		PhysicsTimer = 0;		
	}

	if(CurrentBallState == EBallState::Frozen)
	{
		if(DecayCooldown <= 0)
		{
			ChangeChargeScore(CurrentChargeScore - ChargeDecayPerSeconds*DeltaTime);
		}
		else
		{
			DecayCooldown -= DeltaTime;
		}
	}
}

void ABlazeBall::PickupBall(ABlazerCharacter* NewBallHandler)
{
	if(!bIsInteractable) return;
	if(GetLocalRole() != ROLE_Authority) return;
	if(CurrentBallState == EBallState::DisabledPhysics) return;
	if(CurrentBallHandler != nullptr) return;

	//Set ball holder reference to player that picked it up
	CurrentBallHandler = NewBallHandler;
	CurrentBallHandler->SetHasBall(this);

	HoldCooldown = HoldStartDuration;

	LastPickedUpPos = CurrentBallHandler->GetActorLocation();
	
	if(Cast<ABlazerPlayerState>(CurrentBallHandler->GetPlayerState()))
	{
		CurrentBallHolderPS = Cast<ABlazerPlayerState>(CurrentBallHandler->GetPlayerState());
	}

	if(const ABlazerPlayerState* BallHandlerPlayerState = Cast<ABlazerPlayerState>(CurrentBallHandler->GetPlayerState()))
	{
		LastTeamThatHeldBall = BallHandlerPlayerState->Team;
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Team %d has the ball"), LastTeamThatHeldBall));
	}

	if(IsValid(PreviousBallHolderPS))
	{
		if(PreviousBallHolderPS != CurrentBallHolderPS && CurrentVelocity.Length() >= 850)
		{
			//TODO: calculate distance
			Cast<ABlazerGameState>(GetWorld()->GetGameState())->CreateNewPassInfo(PreviousBallHolderPS, CurrentBallHolderPS, FVector::Distance(LastPickedUpPos, LastThrownPos));
		}
	}
	
	//Disable ball (visibility & collision)
	//BallMesh->SetVisibility(false);
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToComponent(CurrentBallHandler->GetMesh(), AttachmentRules, CurrentBallHandler->BallGripSocketName);
	ChangeBallState(EBallState::DisabledPhysics);
	CurrentVelocity = FVector::ZeroVector;
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CurrentBallHandler->MulticastSetHasBall(true);
	CurrentBallHandler->MulticastPickupBall();
	
	MulticastPickupBall(NewBallHandler);
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Pickup is state %i"), CurrentBallState));
}

void ABlazeBall::ServerPickupBall_Implementation(ABlazerCharacter* NewBallHandler)
{
	PickupBall(NewBallHandler);
}

void ABlazeBall::MulticastPickupBall_Implementation(ABlazerCharacter* NewBallHandler)
{
	if(NewBallHandler->IsLocallyControlled())
	{
		BallMesh->SetVisibility(false);
	}
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	AttachToComponent(NewBallHandler->GetMesh(), AttachmentRules, NewBallHandler->BallGripSocketName);
	SetActorRelativeLocation(FVector::ZeroVector);
	CurrentVelocity = FVector::ZeroVector;
	OnPickupBall(NewBallHandler->IsLocallyControlled());
}

void ABlazeBall::ThrowBall(const FVector& HandlerOrigin, const FVector& PlaceLocation, const FVector& ThrowVelocity)
{
	if(GetLocalRole() != ROLE_Authority) return;
	//Enable the ball (visibility & collision)
	BallMesh->SetVisibility(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bIsThrown = true;
	LastThrownTimestamp = GetWorld()->TimeSeconds;	

	//Set initial ball location
	SetActorRotation(FQuat::Identity);
	SetActorLocation(CorrectBallPlacement(HandlerOrigin, PlaceLocation));
	
	//Reset current gravity scale and velocity
	ResetGravity();
	CurrentAirTime = 0;
	CurrentVelocity = ThrowVelocity;
	ChangeBallState(EBallState::Airborne);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LastThrownPos = HandlerOrigin;
	
	PreviousBallHolderPS = CurrentBallHolderPS;
	CurrentBallHolderPS = nullptr;

	MulticastThrowBall();
	
	//Clear reference to ball holder
	CurrentBallHandler->SetHasBall(nullptr);
	CurrentBallHandler->MulticastSetHasBall(false);
	CurrentBallHandler->MulticastThrowBall();
	
	CurrentBallHandler = nullptr;
}

void ABlazeBall::ServerThrowBall_Implementation(const FVector HandlerOrigin, const FVector PlaceLocation, const FVector ThrowVelocity)
{
	ThrowBall(HandlerOrigin, PlaceLocation, ThrowVelocity);
}

void ABlazeBall::MulticastThrowBall_Implementation()
{
	BallMesh->SetVisibility(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnThrowBall();
}

void ABlazeBall::DropBall(const FVector& HandlerOrigin, const FVector& PlaceLocation)
{
	if(GetLocalRole() != ROLE_Authority) return;
	//Enable the ball (visibility & collision)
	BallMesh->SetVisibility(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	LastThrownTimestamp = GetWorld()->TimeSeconds;

	//Set initial ball location
	SetActorRotation(FQuat::Identity);
	SetActorLocation(CorrectBallPlacement(HandlerOrigin, PlaceLocation));
	
	//Set velocity to "BallVelocityOnDrop"
	ResetGravity();
	CurrentAirTime = 0;
	CurrentVelocity = FVector::DownVector * BallVelocityOnDrop;
	ChangeBallState(EBallState::Airborne);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	LastThrownPos = HandlerOrigin;
	
	PreviousBallHolderPS = CurrentBallHolderPS;
	CurrentBallHolderPS = nullptr;
	
	MulticastDropBall();
	
	//Clear reference to ball holder
	CurrentBallHandler->SetHasBall(nullptr);
	CurrentBallHandler->MulticastSetHasBall(false);
	CurrentBallHandler->MulticastDropBall();
	
	CurrentBallHandler = nullptr;
}

void ABlazeBall::ServerDropBall_Implementation(const FVector HandlerOrigin, const FVector PlaceLocation)
{
	DropBall(HandlerOrigin, PlaceLocation);
}

void ABlazeBall::MulticastDropBall_Implementation()
{
	BallMesh->SetVisibility(true);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OnDropBall();
}

void ABlazeBall::MulticastBounceBall_Implementation(const float BounceMagnitude)
{
	OnBounce(BounceMagnitude);
}

void ABlazeBall::AddImpulse(const FVector Force)
{
	if(CurrentBallState == EBallState::DisabledPhysics) return;
	ForceToAdd += Force;
}

void ABlazeBall::OverrideForce(const FVector Force)
{
	if(CurrentBallState == EBallState::DisabledPhysics) return;
	bOverrideForce = true;
	PendingOverrideForce = Force;
}

void ABlazeBall::ServerAddImpulse_Implementation(const FVector Force)
{
	AddImpulse(Force);
}

void ABlazeBall::ServerOverrideForce_Implementation(const FVector Force)
{
	OverrideForce(Force);
}

void ABlazeBall::ServerBallPhysics_Implementation(float DeltaTime)
{
	if(CurrentBallState == EBallState::DisabledPhysics) return;
	
	switch (CurrentBallState)
	{
		case EBallState::Grounded:
			PhysGrounded(DeltaTime);
		break;
		case EBallState::Airborne:
			PhysAirborne(DeltaTime);
		break;
		case EBallState::Frozen:
			PhysFrozen(DeltaTime);
		break;
	}
}

void ABlazeBall::PhysGrounded(float DeltaTime)
{
	UpdateVelocity(DeltaTime);

	//Apply gravity to current direction
	const FVector Gravity = (FVector::DownVector * GetGravityScale(0)*SlopeGravityScale);

	CurrentVelocity = FVector::VectorPlaneProject( CurrentVelocity + Gravity*DeltaTime, LastFloorNormal);

	PerformMove(CurrentVelocity*DeltaTime, DeltaTime);

	if(CurrentBallState == EBallState::DisabledPhysics) return;

	GroundCheck(CurrentVelocity.Z <= MinVelocityBeforeFreeze);
	
	ApplyDrag(GroundedDrag, DeltaTime);
}

void ABlazeBall::PhysAirborne(float DeltaTime)
{
	CurrentAirTime += DeltaTime;
	
	UpdateVelocity(DeltaTime);

	//Apply gravity to current direction
	const FVector Gravity = (FVector::DownVector * GetGravityScale(CurrentAirTime));

	CurrentVelocity += Gravity * DeltaTime;

	PerformMove(CurrentVelocity*DeltaTime, DeltaTime);

	if(CurrentBallState == EBallState::DisabledPhysics) return;

	GroundCheck(CurrentVelocity.Z <= MinVelocityBeforeFreeze && CurrentVelocity.Z > -(MinBounceVelocity + GetGravityScale(CurrentAirTime)*DeltaTime));

	ApplyDrag(AirDrag, DeltaTime);
}

void ABlazeBall::PhysFrozen(float DeltaTime)
{
	if(!FloorSweep())
	{
		ChangeBallState(EBallState::Airborne);
	}
	else if(LastFloorNormal.Z <= FLOOR_SLOPE_Z)
	{
		ChangeBallState(EBallState::Grounded);
	}
	UpdateVelocity(DeltaTime);
}

void ABlazeBall::UpdateVelocity(float DeltaTime)
{
	if(!ForceToAdd.IsZero())
	{
		CurrentVelocity += ForceToAdd;
		ForceToAdd = FVector::ZeroVector;
		ChangeBallState(EBallState::Airborne);
	}
	if(bOverrideForce)
	{
		bOverrideForce = false;
		CurrentVelocity = PendingOverrideForce;
		PendingOverrideForce = FVector::ZeroVector;
		ChangeBallState(EBallState::Airborne);
	}
}

void ABlazeBall::PerformMove(FVector MoveDelta, float DeltaTime)
{
	FVector Velocity = CurrentVelocity;

	FHitResult Sweep(1.f);
	//FCollisionQueryParams QueryParams(FName(TEXT("BallSweep")), false,  this);
	//FCollisionResponseParams ResponseParams;

	//SphereCollider->InitSweepCollisionParams(QueryParams, ResponseParams);
	
	//Move ball in current direction based on current velocity
	AddActorWorldOffset(MoveDelta, true, &Sweep);

	if(CurrentBallState == EBallState::DisabledPhysics) return;

	if(Sweep.bBlockingHit)
	{
		float Time = Sweep.Time;
		
		if(Sweep.bStartPenetrating)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Penetrating %s"), *(Sweep.Normal * (Sweep.PenetrationDepth+.1f)).ToString());
			AddActorWorldOffset(Sweep.Normal * (Sweep.PenetrationDepth+.1f));			
		}
		
		const FVector Normal = Sweep.ImpactNormal;
		
		FVector SlideVector = FVector::VectorPlaneProject(MoveDelta, Normal) * (1.f - Time);
		if (FVector::DotProduct(SlideVector, MoveDelta) > 0.f)
		{
			FHitResult SecondSweep(1.f);
			//UE_LOG(LogTemp, Warning, TEXT("Sliding: %s"), *SlideVector.ToString());
			AddActorWorldOffset(SlideVector, true, &SecondSweep);

			if(SecondSweep.bBlockingHit)
			{
				Sweep = SecondSweep;
			}
		}		

		//Only bouncy if not penetrating and going fast enough
		if(FVector::DotProduct(-(Velocity), Normal) > MinBounceVelocity + GetGravityScale(CurrentAirTime)*DeltaTime)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Bounce: %f, Min: %f"), FVector::DotProduct(-Velocity, Normal), MinBounceVelocity + GetGravityScale(CurrentAirTime)*DeltaTime);
			Velocity = FMath::GetReflectionVector(Velocity, Normal);

			//Reduces current velocity by "VelocityReductionMultiplierPerBounce" & increases current gravity scale by "GravityScaleMultiplierPerBounce"
			Velocity *= VelocityReductionMultiplierPerBounce;
			CurrentGravityMultiplier *= GravityScaleMultiplierPerBounce;
			bIsThrown = false;

			MulticastBounceBall_Implementation(FVector::DotProduct(Velocity, Normal));
		}
	}

	CurrentVelocity = Velocity;
}

void ABlazeBall::GroundCheck(const bool bAdditionalCondition)
{
	if(FloorSweep() && bAdditionalCondition)
	{
		ChangeBallState(EBallState::Grounded);
		ResetGravity();
		OnGrounded();
	}
	else
	{
		ChangeBallState(EBallState::Airborne);
	}
}

bool ABlazeBall::FloorSweep()
{	
	FHitResult FloorHit(1.f);
	FCollisionQueryParams QueryParams(FName(TEXT("BallSweep")), false,  this);
	FCollisionResponseParams ResponseParams;
	FCollisionShape CollisionShape = SphereCollider->GetCollisionShape();

	SphereCollider->InitSweepCollisionParams(QueryParams, ResponseParams);

	const FVector StartLoc = GetActorLocation() + FVector::UpVector*FLOOR_DIST_UPPER;
	const FVector EndLoc = GetActorLocation() + FVector::DownVector*FLOOR_DIST;
	
	if(GetWorld()->SweepSingleByChannel(FloorHit,  StartLoc, EndLoc,
		FQuat::Identity, ECC_PhysicsBody, CollisionShape, QueryParams, ResponseParams) && FloorHit.ImpactNormal.Z > FLOOR_Z)
	{		
		LastFloorNormal = FloorHit.Normal;

		return true;
	}
	else
	{
		LastFloorNormal = FVector::ZeroVector;		
		return false;
	}
}

void ABlazeBall::ApplyDrag(const float DragForce, float DeltaTime)
{
	FVector Velocity = CurrentVelocity;
	
	float VelocityLength = Velocity.Length();

	//Reduce current velocity by "VelocityReductionPerTick"
	VelocityLength = FMath::Max(0,VelocityLength - DragForce*DeltaTime);

	Velocity = Velocity.GetSafeNormal() * FMath::Min(VelocityLength, MaxVelocity);
	
	CurrentVelocity = Velocity;
	
	//Freeze the ball if it is moving slower than "MinVelocityBeforeFreeze"
	if(CurrentBallState == EBallState::Grounded && LastFloorNormal.Z > FLOOR_SLOPE_Z && VelocityLength < MinVelocityBeforeFreeze)
	{
		//Set current velocity to 0 and reset current direction
		CurrentVelocity = FVector::ZeroVector;
		CurrentAirTime = 0;

		//Reset current gravity scale
		ResetGravity();
		
		ChangeBallState(EBallState::Frozen);
	}
}

void ABlazeBall::ChangeBallState(const EBallState NewState)
{
	if(NewState != CurrentBallState)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("ChangeState %i"), NewState));
	}
	if(bIsThrown && NewState != EBallState::Airborne)
	{
		bIsThrown = false;
	}
	if(NewState == EBallState::Frozen)
	{
		DecayCooldown = DecayStartDuration;
	}
	CurrentBallState = NewState;
}

void ABlazeBall::ChangeChargeScore(const float NewValue)
{
	CurrentChargeScore = FMath::Clamp(NewValue, 0, 100);
	if(ABlazerGameState* GameState = GetWorld()->GetGameState<ABlazerGameState>())
	{
		GameState->SetChargeScore(CurrentChargeScore);	
	}
}

void ABlazeBall::OnGrounded()
{
	CurrentAirTime = 0;
}

void ABlazeBall::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bIsInteractable) return;
	if(GetLocalRole() != ROLE_Authority) return;
	if(CurrentBallState == EBallState::DisabledPhysics) return;
	if(CurrentBallHandler != nullptr) return;

	ABlazerCharacter* OverlapBlazer = Cast<ABlazerCharacter>(OtherActor);

	if(!OverlapBlazer || OverlapBlazer->GetHealthComponent()->IsDead()) return;

	const ABlazerPlayerState* OverlapBlazerState = Cast<ABlazerPlayerState>(OverlapBlazer->GetPlayerState());

	if(!OverlapBlazerState) return;

	const float TimeSinceThrow = GetWorld()->TimeSeconds - LastThrownTimestamp;
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Overlap %i"), (TimeSinceHeld < .1f)));
	if(TimeSinceThrow < .5f && PreviousBallHolderPS == OverlapBlazerState) return;
	if(TimeSinceThrow < .01f || OverlapBlazer->HasBall()) return;

	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Overlap")));

	const float VelocityMagnitude = CurrentVelocity.Length();
	if(bIsThrown && LastTeamThatHeldBall == OverlapBlazerState->Team)
	{
		PickupBall(OverlapBlazer);
	}
	else if(!bIsThrown && VelocityMagnitude < MaxPickupVelocity)
	{
		PickupBall(OverlapBlazer);
	}
	else if(VelocityMagnitude > MinHitVelocity)
	{
		LastThrownTimestamp = GetWorld()->TimeSeconds;
		//todo: freeze player for duration
		const float ForceMultiplier = VelocityMagnitude/MinHitVelocity;
		if(VelocityMagnitude > MinDeathVelocity)
		{
			OverlapBlazer->GetHealthComponent()->ServerTakeDamage(9999, this);
			MulticastHitPlayer(true, VelocityMagnitude);
		}
		else
		{
			OverlapBlazer->GetHealthComponent()->ServerTakeDamage(HitDamage*ForceMultiplier, this);
			MulticastHitPlayer(false, VelocityMagnitude);
		}
		OverlapBlazer->GetBlazerMovementComponent()->AddImpulse(CurrentVelocity.GetSafeNormal() * (HitKnockback*ForceMultiplier) + FVector::UpVector*(HitVerticalKnockback*ForceMultiplier), true);
		const FVector ThrowerDirection = ((LastThrownPos + FVector::UpVector*50.f) - GetActorLocation()).GetSafeNormal();
		OverrideForce(ThrowerDirection * (VelocityMagnitude*HitBounceReductionMultiplier));
	}
}

void ABlazeBall::MulticastHitPlayer_Implementation(const bool bInstantKill, const float HitMagnitude)
{
	OnHitPlayer(bInstantKill, HitMagnitude);
}

void ABlazeBall::ServerSetInteractable_Implementation(const bool bState)
{
	bIsInteractable = bState;
}

void ABlazeBall::ServerFreeze_Implementation()
{
	//Set current velocity to 0 and reset current direction
	CurrentVelocity = FVector::ZeroVector;
	CurrentAirTime = 0;

	//Reset current gravity scale
	ResetGravity();

	ChangeBallState(EBallState::Frozen);
}

void ABlazeBall::ResetGravity()
{
	CurrentAirTime = 0;
	CurrentGravityMultiplier = 1.f;
}

float ABlazeBall::GetGravityScale(const float Time) const
{
	if(!GravityCurve) return 900.f * CurrentGravityMultiplier;
	return GravityCurve->GetFloatValue(Time) * CurrentGravityMultiplier;
}

float ABlazeBall::GetNormalizedChargeScore() const
{
	return CurrentChargeScore/100.f;
}

FVector ABlazeBall::CorrectBallPlacement(FVector Origin, FVector Target) const
{
	FHitResult HitResult(1.f);
	FCollisionQueryParams QueryParams(FName(TEXT("BallCheckSweep")), true,  this);
	FCollisionResponseParams ResponseParams;

	SphereCollider->InitSweepCollisionParams(QueryParams, ResponseParams);

	if(GetWorld()->SweepSingleByChannel(HitResult, Origin, Target,
		FQuat::Identity, ECC_PhysicsBody, SphereCollider->GetCollisionShape(), QueryParams, ResponseParams))
	{
		return HitResult.Location + (Origin - Target).GetSafeNormal()*.01f;
	}

	return Target;
}
