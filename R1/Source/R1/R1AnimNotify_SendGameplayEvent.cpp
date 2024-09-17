// Fill out your copyright notice in the Description page of Project Settings.


#include "R1AnimNotify_SendGameplayEvent.h"
#include "Character/R1Character.h"

UR1AnimNotify_SendGameplayEvent::UR1AnimNotify_SendGameplayEvent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UR1AnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AR1Character* LocalCharacter = Cast<AR1Character>(MeshComp->GetOwner());
	if (LocalCharacter)
	{
		LocalCharacter->HandleGameplayEvent(EventTag);
	}
}
