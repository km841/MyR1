// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "R1AIController.generated.h"

/**
 * 
 */
UCLASS()
class R1_API AR1AIController : public AAIController
{
	GENERATED_BODY()

public:
	AR1AIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
