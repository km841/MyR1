// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)
enum class ECreatureState : uint8
{
	None,
	Moving,
	Skill,
	Dead,
};

#define D(x) if(GEngine) { GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, x);}