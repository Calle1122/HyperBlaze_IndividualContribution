// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectBallBlazers/Match/BlazerPlayerState.h"
#include "BlazeBall.generated.h"

class USphereComponent;
class ABlazerCharacter;

UENUM(BlueprintType)
enum class EBallState : uint8
{
	Grounded,
	Airborne,
	Frozen,
	DisabledPhysics,
};

UCLASS()
class PROJECTBALLBLAZERS_API ABlazeBall : public AActor
{
	GENERATED_BODY()

public:
	ABlazeBall();

	//Collider Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> OverlapCollider;

	//Mesh Component
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball Mesh")
	TObjectPtr<UStaticMeshComponent> BallMesh;

	//Ball Setup Variables
	/** Magnitude limit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0"))
	float MaxVelocity;
	/** Min magnitude for a bounce to occur */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0"))
	float MinBounceVelocity;
	/** Force we use to slow ball in air*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float AirDrag;
	/** Force we use to slow ball on ground */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float GroundedDrag;
	/** Scale for the slope force */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0"))
	float SlopeGravityScale;
	/** GravityScale overtime */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables")
	UCurveFloat* GravityCurve;
	/** Initial downwards force applied when dropped */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float BallVelocityOnDrop;
	/** Multiplier applied on gravity scale each bounce */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0"))
	float GravityScaleMultiplierPerBounce;
	/** Multiplier applied to velocity each bounce */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0"))
	float VelocityReductionMultiplierPerBounce;
	/** Magnitude we freeze ball at */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float MinVelocityBeforeFreeze;
	/** Magnitude we can pickup ball */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Physics Variables", meta=(ClampMin="0", ForceUnits="cm/s"))
	float MaxPickupVelocity;

	/** Magnitude we will trigger a hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float MinHitVelocity;
	/** Magnitude we will one hit kill player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float MinDeathVelocity;
	/** Impulse force (ignoring mass) we will add to hit player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float HitKnockback;
	/** Impulse force (ignoring mass) we will add to hit player vertically */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float HitVerticalKnockback;
	/** Damage we will apply to hit player */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float HitDamage;
	/** Multiplier applied to velocity each bounce */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Damage Variables", meta=(ClampMin="0"))
	float HitBounceReductionMultiplier;

	/** Time player has to hold ball before ball charge increases */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Charge Variables", meta=(ClampMin="0"))
	float HoldStartDuration;
	/** Time ball has to be frozen before decaying score */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Charge Variables", meta=(ClampMin="0"))
	float DecayStartDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Charge Variables", meta=(ClampMin="0"))
	float ChargePerSeconds;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ball|Charge Variables", meta=(ClampMin="0"))
	float ChargeDecayPerSeconds;
	
	//Ball External Variables (Replicated)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	bool bIsInteractable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	EBallState CurrentBallState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	FVector CurrentVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	float CurrentAirTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	int LastTeamThatHeldBall;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	ABlazerPlayerState* PreviousBallHolderPS;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	ABlazerPlayerState* CurrentBallHolderPS;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	FVector LastThrownPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	FVector LastPickedUpPos;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Ball|ReplicatedDebug")
	float CurrentChargeScore;

protected:
	TObjectPtr<ABlazerCharacter> CurrentBallHandler;
	
	//Ball Internal Variables
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="Ball|Debug")
	float CurrentGravityMultiplier;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	FVector ForceToAdd;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	bool bOverrideForce;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	FVector PendingOverrideForce;
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category="Ball|Debug")
	FVector LastFloorNormal;
	UPROPERTY(Transient)
	float PhysicsTimer;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	float LastThrownTimestamp;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	bool bIsThrown;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	float HoldCooldown;
	UPROPERTY(Transient, VisibleAnywhere, Category="Ball|Debug")
	float DecayCooldown;
	
public:
	//for Server Functions
	UFUNCTION()
	void PickupBall(ABlazerCharacter* NewBallHandler);
	UFUNCTION()
	void ThrowBall(const FVector& HandlerOrigin, const FVector& PlaceLocation, const FVector& ThrowVelocity);
	UFUNCTION()
	void DropBall(const FVector& HandlerOrigin, const FVector& PlaceLocation);
	UFUNCTION()
	void AddImpulse(FVector Force);
	UFUNCTION()
	void OverrideForce(FVector Force);

	UFUNCTION(BlueprintCallable)
	float GetNormalizedChargeScore() const;
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerFreeze();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetInteractable(bool bState);
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Ball Server Functions")
	void ServerPickupBall(ABlazerCharacter* NewBallHandler);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerThrowBall(FVector HandlerOrigin, FVector PlaceLocation, FVector ThrowVelocity);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerDropBall(FVector HandlerOrigin, FVector PlaceLocation);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerAddImpulse(FVector Force);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerOverrideForce(FVector Force);

protected:
	UFUNCTION(NetMulticast, Reliable, Category = "Ball Server Functions")
	void MulticastPickupBall(ABlazerCharacter* NewBallHandler);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastThrowBall();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDropBall();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBounceBall(float BounceMagnitude);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHitPlayer(bool bInstantKill, float HitMagnitude);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPickupBall(bool bIsLocal);
	UFUNCTION(BlueprintImplementableEvent)
	void OnThrowBall();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDropBall();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBounce(float BounceMagnitude);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitPlayer(bool bInstantKill, float HitMagnitude);
	
	UFUNCTION()
	FVector CorrectBallPlacement(FVector Origin, FVector Target) const;
	UFUNCTION()
	float GetGravityScale(float Time) const;
	
	UFUNCTION(Server, Reliable)
	void ServerBeginPlay();
	UFUNCTION(Server, Reliable)
	void ServerBallPhysics(float DeltaTime);

	UFUNCTION()
	void PhysGrounded(float DeltaTime);
	UFUNCTION()
	void PhysAirborne(float DeltaTime);
	UFUNCTION()
	void PhysFrozen(float DeltaTime);

	UFUNCTION()
	void UpdateVelocity(float DeltaTime);
	UFUNCTION()
	void PerformMove(FVector MoveDelta, float DeltaTime);
	UFUNCTION()
	void GroundCheck(bool bAdditionalCondition);
	UFUNCTION()
	bool FloorSweep();
	UFUNCTION()
	void ApplyDrag(float DragForce, float DeltaTime);
	UFUNCTION()
	void ChangeBallState(EBallState NewState);
	UFUNCTION()
	void ChangeChargeScore(float NewValue);

	UFUNCTION()
	void OnGrounded();
	UFUNCTION()
	void ResetGravity();
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//Function needed to replicate variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};