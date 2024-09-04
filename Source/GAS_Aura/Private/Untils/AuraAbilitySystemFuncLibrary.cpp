// Copyright fangh.space


#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Untils/AuraAbilityTypes.h"
#include "Untils/AuraGameplayTags.h"

// PlayerController
AAuraPlayerController* UAuraAbilitySystemFuncLibrary::GetAuraPlayerController(const UObject* WorldContextObject)
{
	return Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0));
}

// WidgetController
bool UAuraAbilitySystemFuncLibrary::MakeWidgetControllerParams(
	const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetParams, AAuraHUD*& OutAuraHUD)
{
	const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return false;

	const auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return false;

	const auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return false;

	const auto AS = PS->GetAttributeSet();
	if (!AS) return false;

	OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!OutAuraHUD) return false;

	OutWidgetParams = FWidgetControllerParams {PC, PS, ASC, AS};
	return true;
}

UOverlayMainWidgetController* UAuraAbilitySystemFuncLibrary::GetOverlayMainWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetParams;
	
	if (AAuraHUD* AuraHUD; MakeWidgetControllerParams(WorldContextObject, WidgetParams, AuraHUD))
	{
		return AuraHUD->GetOverlayMainWidgetController(WidgetParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemFuncLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetParams;
	
	if (AAuraHUD* AuraHUD; MakeWidgetControllerParams(WorldContextObject, WidgetParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WidgetParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemFuncLibrary::GetSpellMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WidgetParams;
	
	if (AAuraHUD* AuraHUD; MakeWidgetControllerParams(WorldContextObject, WidgetParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WidgetParams);
	}
	return nullptr;
}

// CharacterClass TypeInfo
void UAuraAbilitySystemFuncLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                                const ECharacterClassType ECT, const float Level, UAbilitySystemComponent* ASC)
{
	if (!ASC || !WorldContextObject) return;

	const auto DA_CharacterClassTypeInfo = GetCharacterClassInfo(WorldContextObject);
	if (!DA_CharacterClassTypeInfo) return;

	const auto ClassDefaultInfo = DA_CharacterClassTypeInfo->GetClassDefaultInfo(ECT);
	const auto AvatarActor = ASC->GetAvatarActor();

	auto GE_PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	GE_PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const auto SpecHandle_PrimaryAttributes = ASC->MakeOutgoingSpec(ClassDefaultInfo.GE_PrimaryAttributes, Level, GE_PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle_PrimaryAttributes.Data.Get());

	auto GE_SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	GE_SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const auto SpecHandle_SecondaryAttributes = ASC->MakeOutgoingSpec(DA_CharacterClassTypeInfo->GE_SecondaryAttributes, Level, GE_SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle_SecondaryAttributes.Data.Get());

	auto GE_VitalAttributeContextHandle = ASC->MakeEffectContext();
	GE_VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const auto SpecHandle_VitalAttributes = ASC->MakeOutgoingSpec(DA_CharacterClassTypeInfo->GE_VitalAttributes, Level, GE_VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle_VitalAttributes.Data.Get());
}

UDataAsset_CharacterClassInfo* UAuraAbilitySystemFuncLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const auto AuraGMBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGMBase) return nullptr;
	
	return AuraGMBase->DA_CharacterClassInfo;
}

// Ability
void UAuraAbilitySystemFuncLibrary::GiveStartupAbilities(
	const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClassType ECT)
{
	if (!ASC || !WorldContextObject) return;

	const auto DA_CharacterClassTypeInfo = GetCharacterClassInfo(WorldContextObject);
	if (!DA_CharacterClassTypeInfo) return;

	for (const auto& AbilityClass : DA_CharacterClassTypeInfo->CommonAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{AbilityClass, 1};
		ASC->GiveAbility(GASpec);
	}

	const auto DefaultInfo = DA_CharacterClassTypeInfo->GetClassDefaultInfo(ECT);
	for (const auto& AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			auto GASpec = FGameplayAbilitySpec{AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor())};
			ASC->GiveAbility(GASpec);
		}
	}
}

UDataAsset_AbilityInfo* UAuraAbilitySystemFuncLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const auto AuraGMBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGMBase) return nullptr;

	return AuraGMBase->DA_AbilityInfo;
}

// Gameplay Effect
bool UAuraAbilitySystemFuncLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemFuncLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemFuncLibrary::IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsSuccessfulDeBuff();
	}
	return false;
}

float UAuraAbilitySystemFuncLibrary::GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeBuffDamage();
	}
	return 0.f;
}

float UAuraAbilitySystemFuncLibrary::GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeBuffDuration();
	}
	return 0.f;
}

float UAuraAbilitySystemFuncLibrary::GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeBuffFrequency();
	}
	return 0.f;
}

FGameplayTag UAuraAbilitySystemFuncLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *AuraEffectContext->GetDamageType();
	}
	return FGameplayTag{};
}

FVector UAuraAbilitySystemFuncLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemFuncLibrary::GetKnockBackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockBackForce();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemFuncLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsBlockedHit)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bIsBlockedHit);
	}
}

void UAuraAbilitySystemFuncLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsCriticalHit)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bIsCriticalHit);
	}
}

void UAuraAbilitySystemFuncLibrary::SetIsSuccessfulDeBuff(FGameplayEffectContextHandle& EffectContextHandle, const bool bIsSuccessfulDeBuff)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsSuccessfulDeBuff(bIsSuccessfulDeBuff);
	}
}

void UAuraAbilitySystemFuncLibrary::SetDeBuffDamage(FGameplayEffectContextHandle& EffectContextHandle, const float InDamage)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeBuffDamage(InDamage);
	}
}

void UAuraAbilitySystemFuncLibrary::SetDeBuffDuration(FGameplayEffectContextHandle& EffectContextHandle, const float InDuration)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeBuffDuration(InDuration);
	}
}

void UAuraAbilitySystemFuncLibrary::SetDeBuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, const float InFrequency)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeBuffFrequency(InFrequency);
	}
}

void UAuraAbilitySystemFuncLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDamageType(MakeShared<FGameplayTag>(InDamageType));
	}
}

void UAuraAbilitySystemFuncLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(InDeathImpulse);
	}
}

void UAuraAbilitySystemFuncLibrary::SetKnockBackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockBackForce)
{
	if (const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockBackForce(InKnockBackForce);
	}
}

// Gameplay Mechanics
void UAuraAbilitySystemFuncLibrary::GetLivePlayersWithRadius(
	const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingPlayers, const TArray<AActor*>& ActorsToIgnore,
	const float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(
			Overlaps, SphereOrigin, FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const auto& Overlap : Overlaps)
		{
			if (!Overlap.GetActor()->Implements<UCombatInterface>()) continue;
			if (!ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingPlayers.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

TArray<FRotator> UAuraAbilitySystemFuncLibrary::EvenlySpacedRotators(
	const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const auto LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const auto DeltaSpread = Spread / (NumRotators - 1);
		
		for (int32 i = 0; i < NumRotators; ++i)
		{
			const auto Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemFuncLibrary::EvenlyRotatedVectors(
	const FVector& Forward, const FVector& Axis, const float Spread, const int32 NumberVectors)
{
	TArray<FVector> Vectors;

	const auto LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumberVectors > 1)
	{
		const auto DeltaSpread = Spread / (NumberVectors - 1);
		
		for (int32 i = 0; i < NumberVectors; ++i)
		{
			const auto Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
}

// Check
bool UAuraAbilitySystemFuncLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	if (!FirstActor ||!SecondActor) return false;
	const auto BothArePlayers = FirstActor->ActorHasTag(FName{"Player"}) && SecondActor->ActorHasTag(FName{"Player"});
	const auto BothAreEnemies = FirstActor->ActorHasTag(FName{"Enemy"}) && SecondActor->ActorHasTag(FName{"Enemy"});
    return !(BothArePlayers || BothAreEnemies);
}

// XP Reward
int32 UAuraAbilitySystemFuncLibrary::GetXPRewardForClassAndLevel(
	const UObject* WorldContextObject, const ECharacterClassType ECT, const int32 Level)
{
	if (!WorldContextObject) return 0;

	const auto DA_CharacterClassTypeInfo = GetCharacterClassInfo(WorldContextObject);
	if (!DA_CharacterClassTypeInfo) return 0;

	const auto& Info = DA_CharacterClassTypeInfo->GetClassDefaultInfo(ECT);
	const auto XPReward = Info.XPReward.GetValueAtLevel(Level);
	
	return static_cast<int32>(XPReward);
}

// ApplyDamage
FGameplayEffectContextHandle UAuraAbilitySystemFuncLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const auto GameplayTags = FAuraGameplayTags::Get();
	const auto SourceAvatarActor = DamageEffectParams.SourceASComponent->GetAvatarActor();
	
	auto EffectContextHandle = DamageEffectParams.SourceASComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockBackForce(EffectContextHandle, DamageEffectParams.KnockBackForce);

	const auto EffectSpecHandle =
		DamageEffectParams.SourceASComponent->MakeOutgoingSpec(
			DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.DeBuff_Chance, DamageEffectParams.DeBuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.DeBuff_Damage, DamageEffectParams.DeBuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.DeBuff_Duration, DamageEffectParams.DeBuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.DeBuff_Frequency, DamageEffectParams.DeBuffFrequency);
	
	DamageEffectParams.TargetASComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectContextHandle;
}
