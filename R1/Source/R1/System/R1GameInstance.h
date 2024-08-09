// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "R1GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class R1_API UR1GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UR1GameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;
	virtual void Shutdown() override;
	
};
