// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrosshairWidget.generated.h"

UCLASS(Abstract)
class PROJECTBALLBLAZERS_API UCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Crosshair Events")
	void OnShowCrosshair();
	UFUNCTION(BlueprintImplementableEvent, Category = "Crosshair Events")
	void OnShowDot();
};
