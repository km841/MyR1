// Fill out your copyright notice in the Description page of Project Settings.


#include "R1GameInstance.h"
#include "R1AssetManager.h"

UR1GameInstance::UR1GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UR1GameInstance::Init()
{
	Super::Init();

	UR1AssetManager::Initialize();
}

void UR1GameInstance::Shutdown()
{
	Super::Shutdown();
}
