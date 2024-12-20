// Copyright fangh.space

#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Untils/AuraLog.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Gameplay/Actor/MagicCircle.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighLightInterface.h"
#include "UI/Widget/DamageTextWidgetComponent.h"
#include "Untils/AuraCollision.h"
#include "Untils/AuraGameplayTags.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASComponent()
{
	if (AuraAbilitySystemComponent)
	{
		return AuraAbilitySystemComponent;
	}
	return AuraAbilitySystemComponent =
		Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetCharacter()));
}

void AAuraPlayerController::SetCursorTraceMode_Implementation(const bool bEnable)
{
	IsCursorTraceMode = bEnable;
	if (bEnable)
	{
		StartTickTimerHandle_AutoRun();
		StartTickTimerHandle_CursorTrace();
		return;
	}
	StopTickTimerHandle_AutoRun();
	StopTickTimerHandle_CursorTrace();
}

void AAuraPlayerController::HighLightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighLightInterface>())
	{
		if (InActor) IHighLightInterface::Execute_HighLightActor(InActor);
	}
}

void AAuraPlayerController::UnHighLightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighLightInterface>())
	{
		if (InActor) IHighLightInterface::Execute_UnHighLightActor(InActor);
	}
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
	if (GetASComponent() && GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed)) return;
	
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
	if (!IsCursorTraceMode) return;
	UpdateMagicCircleLocation();
	
	if (GetASComponent() && GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighLightActor(LastActor);
		UnHighLightActor(ThisActor);
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	const auto TraceChannel = IsValid(Decal_MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHitResult.GetActor()) && CursorHitResult.GetActor()->Implements<UHighLightInterface>())
	{
		ThisActor = CursorHitResult.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}
	
	if (LastActor != ThisActor)
	{
		UnHighLightActor(LastActor);
		HighLightActor(ThisActor);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!IsCursorTraceMode) return;
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

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (IsValid(Decal_MagicCircleClass) && !IsValid(Decal_MagicCircle))
	{
		Decal_MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(Decal_MagicCircleClass);

		if (DecalMaterial)
		{
			Decal_MagicCircle->Decal_MagicCircle->SetMaterial(0, DecalMaterial);
		}
		else
		{
			UE_LOG(Aura, Warning, TEXT("DecalMaterial is nullptr in %s"), *GetName());
		}
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("Maybe Decal_MagicCircleClass is nullptr in %s"), *GetName());
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(Decal_MagicCircle))
	{
		Decal_MagicCircle->Destroy();
		Decal_MagicCircle = nullptr;
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation() const
{
	if (Decal_MagicCircle)
	{
		Decal_MagicCircle->SetActorLocation(CursorHitResult.ImpactPoint);
	}
}

// RPC
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
void AAuraPlayerController::AbilityInputPressed(const FGameplayTag InputTag)
{
	if (GetASComponent() && GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed)) return;
	
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) && IsValid(ThisActor))
	{
		TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		bAutoRunning = false;
	}
	else
	{
		TargetingStatus = ETargetingStatus::NoTargeting;	
	}
	
	if (GetASComponent())
	{
		GetASComponent()->AbilityInputPressed(InputTag);
	}
}

void AAuraPlayerController::AbilityInputReleased(const FGameplayTag InputTag)
{
	if (GetASComponent() && GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased)) return;

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		GetASComponent()->AbilityInputTagReleased(InputTag);
		return;
	}
	GetASComponent()->AbilityInputTagReleased(InputTag);

	// When FollowTime <= ShortPressThreshold, Character Auto Run To Target Location
	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		if (FollowTime <= ShortPressThreshold && GetCharacter())
		{
			if (IsValid(ThisActor) && ThisActor->Implements<UHighLightInterface>())
			{
				IHighLightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetASComponent() && !GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				if (ClickNiagaraSystem)
				{
					// TODO: Spawn Niagara System At Click Location Not NavPath Last PointLocation (Z not true)
					auto ClickSpawnLocation = CachedDestination;
					ClickSpawnLocation.Z = 0.f;
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, ClickSpawnLocation);
				}
				else
				{
					UE_LOG(Aura, Warning, TEXT("ClickNiagaraSystem is nullptr"));
				}
			}
			
			const auto NavPath =
				UNavigationSystemV1::FindPathToLocationSynchronously(
					this, GetCharacter()->GetActorLocation(), CachedDestination);
			
			if (!NavPath)
			{
				FollowTime = 0.f;
				TargetingStatus = ETargetingStatus::NoTargeting;
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
		TargetingStatus = ETargetingStatus::NoTargeting;
	}
}

void AAuraPlayerController::AbilityInputHeld(const FGameplayTag InputTag)
{
	if (GetASComponent() && GetASComponent()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld)) return;

	// Not LMB => Execute GA
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		GetASComponent()->AbilityInputTagHeld(InputTag);
		return;
	}

	// LMB && bTargeting Enemy => Execute GA
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
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