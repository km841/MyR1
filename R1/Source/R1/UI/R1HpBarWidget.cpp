// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/R1HpBarWidget.h"
#include "Components/ProgressBar.h"

UR1HpBarWidget::UR1HpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UR1HpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UR1HpBarWidget::SetHpRatio(float Ratio)
{
	if (HpBar != nullptr)
	{
		HpBar->SetPercent(Ratio);
	}
}
