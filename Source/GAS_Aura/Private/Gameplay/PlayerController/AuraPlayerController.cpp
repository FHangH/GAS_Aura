// Copyright fangh.space

#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Untils/AuraLog.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (const auto EInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
