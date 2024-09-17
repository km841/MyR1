// Fill out your copyright notice in the Description page of Project Settings.


#include "R1PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "System/R1AssetManager.h"
#include "Data/R1InputData.h"
#include "R1GameplayTags.h"
#include "Character/R1Player.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AR1PlayerController::AR1PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}
void AR1PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (const UR1InputData* InputData = UR1AssetManager::GetAssetByName<UR1InputData>("InputData"))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputData->InputMappingContext, 0);
		}
	}

	R1Player = Cast<AR1Player>(GetCharacter());
}

void AR1PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (const UR1InputData* InputData = UR1AssetManager::GetAssetByName<UR1InputData>("InputData"))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		auto Action1 = InputData->FindInputActionByTag(R1GameplayTags::Input_Action_SetDestination);

		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(Action1, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);

	}
}

void AR1PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TickCursorTrace();

	if (R1Player->GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr) == false)
	{
		SetCreatureState(ECreatureState::Moving);
	}

	ChaseTargetAndAttack();
}

void AR1PlayerController::HandleGameplayEvent(FGameplayTag EventTag)
{
	if (EventTag.MatchesTag(R1GameplayTags::Event_Montage_Attack))
	{
		if (TargetActor)
		{
			TargetActor->OnDamaged(R1Player->FinalDamage, R1Player);
		}
	}
}

void AR1PlayerController::TickCursorTrace()
{
	if (bMousePressed)
	{
		return;
	}

	FHitResult OutCursorHit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, OutCursorHit);

	if (bHitSuccessful == false)
	{
		return;
	}

	AR1Character* LocalHighlightActor = Cast<AR1Character>(OutCursorHit.GetActor());

	if (LocalHighlightActor == nullptr)
	{
		if (HighlightActor)
		{
			HighlightActor->UnHighlight();
		}
	}
	else
	{
		if (HighlightActor)
		{
			if (HighlightActor != LocalHighlightActor)
			{
				HighlightActor->UnHighlight();
				LocalHighlightActor->Highlight();
			}
		}
		else
		{
			LocalHighlightActor->Highlight();
		}
	}

	HighlightActor = LocalHighlightActor;

}

void AR1PlayerController::ChaseTargetAndAttack()
{
	if (TargetActor == nullptr)
		return;

	if (GetCreatureState() == ECreatureState::Skill)
	{
		return;
	}

	FVector Direction = TargetActor->GetActorLocation() - R1Player->GetActorLocation();

	if (Direction.Length() < 250.f)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Cyan, TEXT("Attack"));

		if (AttackMontage)
		{
			if (bMousePressed)
			{
				//TargetActor->OnDamaged(R1Player->FinalDamage, R1Player);

				FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(R1Player->GetActorLocation(), TargetActor->GetActorLocation());
				R1Player->SetActorRotation(Rotator);
				
				//if (R1Player->GetMesh()->GetAnimInstance()->Montage_IsPlaying(nullptr) == false)
				R1Player->PlayAnimMontage(AttackMontage);
				SetCreatureState(ECreatureState::Skill);

				TargetActor = HighlightActor;
			}
		}
		else
		{
			TargetActor = nullptr;
		}
	}
	else
	{
		FVector WorldDirection = Direction.GetSafeNormal();
		R1Player->AddMovementInput(WorldDirection, 1.0f, false);
	}
}

void AR1PlayerController::OnInputStarted()
{
	StopMovement();
	bMousePressed = true;
	TargetActor = HighlightActor;
}

void AR1PlayerController::OnSetDestinationTriggered()
{
	if (GetCreatureState() == ECreatureState::Skill)
	{
		return;
	}

	if (TargetActor)
	{
		return;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);

	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}

	APawn* ControlledPawn = GetPawn();
	if (R1Player != nullptr)
	{
		FVector WorldDirection = (CachedDestination - R1Player->GetActorLocation()).GetSafeNormal();
		R1Player->AddMovementInput(WorldDirection, 1.f, false);
	}

}

void AR1PlayerController::OnSetDestinationReleased()
{
	bMousePressed = false;

	if (GetCreatureState() == ECreatureState::Skill)
	{
		return;
	}

	if (FollowTime <= ShortPressThreshold)
	{
		if (TargetActor == nullptr)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
		}

		else
		{

		}

	}

	FollowTime = 0.f;
}

ECreatureState AR1PlayerController::GetCreatureState()
{
	if (R1Player)
	{
		return R1Player->CreatureState;
	}
	return ECreatureState::None;
}

void AR1PlayerController::SetCreatureState(ECreatureState InState)
{
	if (R1Player) 
	{
		R1Player->CreatureState = InState;
	}
}

//void AR1PlayerController::Input_Move(const FInputActionValue& InputValue)
//{
//	FVector2D MovementVector = InputValue.Get<FVector2D>();
//	if (MovementVector.X != 0)
//	{
//		//FVector Direction = FVector::ForwardVector * MovementVector.X;
//		//GetPawn()->AddActorWorldOffset(Direction * 50.f);
//
//		FRotator Rotator = GetControlRotation();
//		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
//		GetPawn()->AddMovementInput(Direction, MovementVector.X);
//	}
//
//	if (MovementVector.Y != 0)
//	{
//		FRotator Rotator = GetControlRotation();
//		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
//		GetPawn()->AddMovementInput(Direction, MovementVector.Y);
//	}
//}
//
//void AR1PlayerController::Input_Turn(const FInputActionValue& InputValue)
//{
//	float Val = InputValue.Get<float>();
//	AddYawInput(Val);
//}
//
//void AR1PlayerController::Input_Jump(const FInputActionValue& InputValue)
//{
//	if (AR1Player* MyPlayer = Cast<AR1Player>(GetPawn()))
//	{
//		MyPlayer->Jump();
//	}
//}
//
//void AR1PlayerController::Input_Attack(const FInputActionValue& InputValue)
//{
//	UE_LOG(LogR1, Log, TEXT("Attack!"));
//
//	if (AttackMontage)
//	{
//		Cast<AR1Character>(GetPawn())->PlayAnimMontage(AttackMontage);
//	}
//
//}
