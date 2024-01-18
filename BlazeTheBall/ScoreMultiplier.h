// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScoreMultiplier.generated.h"

UCLASS()
class PROJECTBALLBLAZERS_API AScoreMultiplier : public AActor
{
	GENERATED_BODY()

public:
	AScoreMultiplier();

	//Collider Component (Also Root)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Collider")
	TObjectPtr<UStaticMeshComponent> Collider;
	//Ring Mesh
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Ring")
	TObjectPtr<UStaticMeshComponent> Ring;

	//Multiplier Setup Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Setup Variables")
	TObjectPtr<UMaterial> ActiveRingMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Setup Variables")
	TObjectPtr<UMaterial> InActiveRingMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Setup Variables")
	TObjectPtr<UMaterial> ActiveInsideMaterial;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Multiplier Setup Variables")
	TObjectPtr<UMaterial> InActiveInsideMaterial;
	
	//Multiplier External Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Multiplier Variables")
	bool IsOpen;

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpen();
	UFUNCTION(BlueprintImplementableEvent)
	void OnClose();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	//Function needed to replicate variables
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Multiplier External Functions
	UFUNCTION(BlueprintCallable, Category = "Multiplier Functions")
	void SetOpenState(bool NewIsOpen);

	/** Networked Open State Functions **/
	/*UFUNCTION(Server, Reliable)
	void ServerSetOpenState(bool NewIsOpen);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetOpenState(bool NewIsOpen);*/

	//Collision Functions
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* Primitive, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
