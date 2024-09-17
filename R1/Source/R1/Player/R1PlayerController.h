// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "R1Define.h"
#include "GameplayTagContainer.h"
#include "R1PlayerController.generated.h"

struct FInputActionValue;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class R1_API AR1PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AR1PlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

public:
	virtual void HandleGameplayEvent(FGameplayTag EventTag);
	
private:
	void TickCursorTrace();
	void ChaseTargetAndAttack();

protected:
	//UPROPERTY(EditAnywhere, Category=Input)
	//TObjectPtr<class UInputMappingContext> InputMappingContext;

	//UPROPERTY(EditAnywhere, Category = Input)
	//TObjectPtr<class UInputAction> TestAction;

	//UPROPERTY(EditAnywhere, Category = Input)
	//TObjectPtr<class UInputAction> MoveAction;

	//UPROPERTY(EditAnywhere, Category = Input)
	//TObjectPtr<class UInputAction> TurnAction;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<class UAnimMontage> AttackMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UNiagaraSystem> FXCursor;

private:
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	ECreatureState GetCreatureState();
	void SetCreatureState(ECreatureState InState);

private:
	FVector CachedDestination;
	float FollowTime;
	bool bMousePressed = false;


protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AR1Character> TargetActor;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AR1Character> HighlightActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AR1Player> R1Player;
	
};
