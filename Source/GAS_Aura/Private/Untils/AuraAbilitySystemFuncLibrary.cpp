// Copyright fangh.space


#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayMainWidgetController* UAuraAbilitySystemFuncLibrary::GetOverlayMainWidgetController(
																const UObject* WorldContextObject)
{
	const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	const auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return nullptr;

	const auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return nullptr;

	const auto AS = PS->GetAttributeSet();
	if (!AS) return nullptr;

	const auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;

	const FWidgetControllerParams WidgetControllerParams {PC, PS, ASC, AS};
	return AuraHUD->GetOverlayMainWidgetController(WidgetControllerParams);
}

UAttributeMenuWidgetController* UAuraAbilitySystemFuncLibrary::GetAttributeMenuWidgetController(
																	const UObject* WorldContextObject)
{
	const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	const auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return nullptr;

	const auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return nullptr;

	const auto AS = PS->GetAttributeSet();
	if (!AS) return nullptr;

	const auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;

	const FWidgetControllerParams WidgetControllerParams {PC, PS, ASC, AS};
	return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
}

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

void UAuraAbilitySystemFuncLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	if (!ASC || !WorldContextObject) return;

	const auto DA_CharacterClassTypeInfo = GetCharacterClassInfo(WorldContextObject);
	if (!DA_CharacterClassTypeInfo) return;

	for (const auto& AbilityClass : DA_CharacterClassTypeInfo->CommonAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{AbilityClass, 1};
		ASC->GiveAbility(GASpec);
	}
}

UDataAsset_CharacterClassInfo* UAuraAbilitySystemFuncLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const auto AuraGMBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!AuraGMBase) return nullptr;
	
	return AuraGMBase->DA_CharacterClassInfo;
}
