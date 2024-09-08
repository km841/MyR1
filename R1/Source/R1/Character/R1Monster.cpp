// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/R1Monster.h"

AR1Monster::AR1Monster()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, -0.f));
}

void AR1Monster::BeginPlay()
{
	Super::BeginPlay();
	
}

void AR1Monster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
