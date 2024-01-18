// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheelPageWidget.h"
#include "Blueprint/UserWidget.h"
#include "PingWheelWidget.generated.h"

UCLASS()
class PROJECTBALLBLAZERS_API UPingWheelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//
	/** CHILD WIDGETS **/
	//

	//Wheel Page 1
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWheelPageWidget* WheelPage_1;
	
	//Wheel Page 2
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UWheelPageWidget* WheelPage_2;
};
