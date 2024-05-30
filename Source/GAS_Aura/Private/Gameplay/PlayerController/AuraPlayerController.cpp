// Copyright fangh.space

#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Untils/AuraLog.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextWidgetComponent.h"
#include "Untils/AuraGameplayTags.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASComponent()
{
	return AuraAbilitySystemComponent =
		AuraAbilitySystemComponent == nullptr ?
			Cast<UAuraAbilitySystemComponent>(
				UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter())) :
			AuraAbilitySystemComponent;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!AuraInputMappingContext || !IA_Move)
	{
		UE_LOG(Aura, Warning, TEXT("%hc: AuraInputMappingContext or IA_Move is nullptr"), *__FUNCTION__);
		return;
	}
	
	if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(AuraInputMappingContext, 0);
		
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;
		
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	
	TickHandle();
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	if (IsNativeTick_CursorTrace)
	{
		CursorTrace();
	}
	if (IsNativeTick_AutoRun)
	{
		AutoRun();
	}
}

void AAuraPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ClearTickTimerHandle_CursorTrace();
	ClearTickTimerHandle_AutoRun();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	if (AuraInputComponent)
	{
		AuraInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
		AuraInputComponent->BindAction(IA_SHIFT, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
		AuraInputComponent->BindAction(IA_SHIFT, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
		AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const auto InputAxisVector = InputActionValue.Get<FVector2d>();
	const auto ForwardDirection =
		FRotationMatrix(FRotator{0.f, GetControlRotation().Yaw, 0.f}).GetUnitAxis(EAxis::X);
	const auto RightDirection =
		FRotationMatrix(FRotator{0.f, GetControlRotation().Yaw, 0.f}).GetUnitAxis(EAxis::Y);

	if (const auto Controlled = GetCharacter())
	{
		Controlled->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		Controlled->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	/*
	 * Line Trace from Cursor. There are many ways to do this
	 * A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *		- Unhighlight LastActor
	 * D. LastActor is valid && ThisActor is valid && ThisActor != LastActor
	 *		- Unhighlight LastActor And Highlight ThisActor
	 * E. LastActor is valid && ThisActor is valid && ThisActor == LastActor
	 *		- Do nothing
	 */
//region OldCode
	/*if (LastActor == nullptr)
	{
		if (ThisActor!= nullptr)
		{
			// Case B
			ThisActor->HighLightActor();
		}
		else
		{
			// Case A - Do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor== nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else // Both Actor Valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			else
			{
				// Case E - Do nothing
			}
		}
	}*/
//endregion
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (ThisActor) ThisActor->HighLightActor();
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (GetCharacter())
	{
		const auto LocationOnSpline =
			SplineComponent->FindLocationClosestToWorldLocation(GetCharacter()->GetActorLocation(), ESplineCoordinateSpace::World);
		const auto Direction =
			SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		
		GetCharacter()->AddMovementInput(Direction);

		const auto DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
			StopTickTimerHandle_AutoRun();
		}
	}
}

void AAuraPlayerController::Client_ShowDamageNumber_Implementation(
	const float DamageAmount, ACharacter* TargetCharacter, const bool IsBlockedHit, const bool IsCriticalHit)
{
	if (!TargetCharacter) return;
	if (IsValid(DamageTextComponentClass))
	{
		const auto DamageTextComp = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
		if (!DamageTextComp) return;
		
		DamageTextComp->RegisterComponent();
		DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComp->SetDamageText(DamageAmount, IsBlockedHit, IsCriticalHit);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("DamageTextComponentClass is nullptr in %s"), *GetName());
	}
}

// Tick Timer Manager
void AAuraPlayerController::TickHandle()
{
	if (IsTickStart_CursorTrace)
	{
		if (TickTimerRate_CursorTrace == ETICK_RATE::ER_TICK_Ultra)
		{
			IsNativeTick_CursorTrace = true;
		}
		else
		{
			IsNativeTick_CursorTrace = false;
			InitTickTimerHandle_CursorTrace();
		}
	}
	
	if (IsTickStart_AutoRun)
	{
		if (TickTimerRate_AutoRun == ETICK_RATE::ER_TICK_Ultra)
		{
			IsNativeTick_AutoRun = true;
		}
		else
		{
			IsNativeTick_AutoRun = false;
			InitTickTimerHandle_AutoRun();
			StopTickTimerHandle_AutoRun();
		}
	}
}

// Tick Timer => CursorTrace
void AAuraPlayerController::InitTickTimerHandle_CursorTrace()
{
	GetWorld()->GetTimerManager().SetTimer(
		TickTimerHandle_CursorTrace, this, &AAuraPlayerController::CursorTrace, TickValue(TickTimerRate_CursorTrace), true);
}

void AAuraPlayerController::StartTickTimerHandle_CursorTrace() const
{
	GetWorld()->GetTimerManager().UnPauseTimer(TickTimerHandle_CursorTrace);
}

void AAuraPlayerController::StopTickTimerHandle_CursorTrace() const
{
	GetWorld()->GetTimerManager().PauseTimer(TickTimerHandle_CursorTrace);
}

void AAuraPlayerController::ClearTickTimerHandle_CursorTrace()
{
	if (TickTimerHandle_CursorTrace.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle_CursorTrace);
	}
}

// Tick Timer => AutoRun
void AAuraPlayerController::InitTickTimerHandle_AutoRun()
{
	GetWorld()->GetTimerManager().SetTimer(
		TickTimerHandle_AutoRun, this, &AAuraPlayerController::AutoRun, TickValue(TickTimerRate_AutoRun), true);
}

void AAuraPlayerController::StartTickTimerHandle_AutoRun() const
{
	GetWorld()->GetTimerManager().UnPauseTimer(TickTimerHandle_AutoRun);
}

void AAuraPlayerController::StopTickTimerHandle_AutoRun() const
{
	GetWorld()->GetTimerManager().PauseTimer(TickTimerHandle_AutoRun);
}

void AAuraPlayerController::ClearTickTimerHandle_AutoRun()
{
	if (TickTimerHandle_AutoRun.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle_AutoRun);
	}
}

// Bind All Actions Use InputTag With DataAsset_AuraInputConfig
void AAuraPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!GetASComponent()) return;

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		GetASComponent()->AbilityInputTagReleased(InputTag);
		return;
	}
	GetASComponent()->AbilityInputTagReleased(InputTag);

	// When FollowTime <= ShortPressThreshold, Character Auto Run To Target Location
	if (!bTargeting && !bShiftKeyDown)
	{
		if (FollowTime <= ShortPressThreshold && GetCharacter())
		{
			const auto NavPath =
				UNavigationSystemV1::FindPathToLocationSynchronously(
					this, GetCharacter()->GetActorLocation(), CachedDestination);
			
			if (!NavPath)
			{
				FollowTime = 0.f;
				bTargeting = false;
				return;
			}
			SplineComponent->ClearSplinePoints();
			
			for (const auto& PointLocation : NavPath->PathPoints)
			{
				SplineComponent->AddSplinePoint(PointLocation, ESplineCoordinateSpace::World);
			}

			if (NavPath->PathPoints.Num() > 0)
			{
				// Ignore NavPath Last Point，Prevents mouse clicks from being located outside the navigation Mesh
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				bAutoRunning = true;
			}
			StartTickTimerHandle_AutoRun();
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!GetASComponent()) return;

	// Not LMB => Execute GA
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		GetASComponent()->AbilityInputTagHeld(InputTag);
		return;
	}

	// LMB && bTargeting Enemy => Execute GA
	if (bTargeting || bShiftKeyDown)
	{
		GetASComponent()->AbilityInputTagHeld(InputTag);
	}
	else // Move Character To Target Location and Add FollowTime
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHitResult.bBlockingHit)
		{
			CachedDestination = CursorHitResult.ImpactPoint;
		}

		if (GetCharacter())
		{
			const auto WorldDirection = (CachedDestination - GetCharacter()->GetActorLocation()).GetSafeNormal();
			GetCharacter()->AddMovementInput(WorldDirection);
		}
	}
}