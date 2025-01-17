// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/R1Character.h"
#include "R1Player.generated.h"

DECLARE_DELEGATE(FRookissDelegate);

/**
 * 
 */
UCLASS()
class R1_API AR1Player : public AR1Character
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AR1Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TestFunc();

	virtual void HandleGameplayEvent(FGameplayTag EventTag) override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

	FRookissDelegate RookissDelegate;
	
};
