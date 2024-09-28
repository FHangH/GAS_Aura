// Copyright fangh.space


#include "Gameplay/GAS/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const auto GameplayTags = FAuraGameplayTags::Get();

	// Primary Attributes
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	// Secondary Attributes
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	// Resistance Attributes
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	Map_TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	// Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Clamp CurrentValue
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);
	
	// Check IsDead
	if (EffectProperties.TargetCharacter->Implements<UCombatInterface>() &&
		ICombatInterface::Execute_IsDead(EffectProperties.SourceCharacter)) return;

	// CurrentValue To BaseValue
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	// Meta => IncomingDamage
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleInComingDamage(EffectProperties);
	}
	// Meta => IncomingXP
	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleInComingXP(EffectProperties);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

// Primary
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldValue);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldValue);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldValue);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldValue);
}

// Secondary
void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldValue);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldValue);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldValue);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldValue);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldValue);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldValue);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldValue);
}

// Resistance
void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldValue);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldValue);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldValue);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldValue);
}

// Vital
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldValue);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldValue);
}

void UAuraAttributeSet::HandleInComingDamage(const FEffectProperties& EffectProp)
{
	const auto LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	
	if (LocalIncomingDamage > 0.f)
	{
		const auto NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		if (NewHealth <= 0)
		{
			if (const auto CombatInterface = Cast<ICombatInterface>(EffectProp.TargetAvatarActor))
			{
				CombatInterface->Die(UAuraAbilitySystemFuncLibrary::GetDeathImpulse(EffectProp.EffectContextHandle));
			}
			SendXPEvent(EffectProp);
		}
		else
		{
			if (EffectProp.TargetCharacter->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsBeingShockLoop(EffectProp.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				EffectProp.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			// KnockBack
			const auto KnockBackForce = UAuraAbilitySystemFuncLibrary::GetKnockBackForce(EffectProp.EffectContextHandle);
			if (!KnockBackForce.IsNearlyZero(1.f))
			{
				EffectProp.TargetCharacter->LaunchCharacter(KnockBackForce, true, true);
			}
		}

		const auto IsBlocked = UAuraAbilitySystemFuncLibrary::IsBlockedHit(EffectProp.EffectContextHandle);
		const auto IsCritical = UAuraAbilitySystemFuncLibrary::IsCriticalHit(EffectProp.EffectContextHandle);
		ShowFloatingText(EffectProp, LocalIncomingDamage, IsBlocked, IsCritical);

		if (UAuraAbilitySystemFuncLibrary::IsSuccessfulDeBuff(EffectProp.EffectContextHandle))
		{
			DeBuff(EffectProp);
		}
	}
}

void UAuraAttributeSet::HandleInComingXP(const FEffectProperties& EffectProp)
{
	const auto LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	// Source Character is the owner, since GA_ListenForEvent applies GE_EventBase, adding to IncomingXP
	if (EffectProp.SourceCharacter->Implements<UPlayerInterface>() &&
		EffectProp.SourceCharacter->Implements<UCombatInterface>())
	{
		const auto CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProp.SourceCharacter);
		const auto CurrentXP = IPlayerInterface::Execute_GetXP(EffectProp.SourceCharacter);
		const auto NewLevel = IPlayerInterface::Execute_FindLevelForXP(EffectProp.SourceCharacter, CurrentXP + LocalIncomingXP);

		if (const auto NewLevelUp = NewLevel - CurrentLevel; NewLevelUp > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(EffectProp.SourceCharacter, NewLevelUp);
			auto AttributePointReward { 0 };
			auto SpellPointReward { 0 };
			
			for (int32 i = 0; i < NewLevelUp; ++i)
			{
				AttributePointReward += IPlayerInterface::Execute_GetAttributePointReward(EffectProp.SourceCharacter, CurrentLevel + i);
				SpellPointReward += IPlayerInterface::Execute_GetSpellPointReward(EffectProp.SourceCharacter, CurrentLevel + i);
			}
			
			IPlayerInterface::Execute_AddToAttributePoints(EffectProp.SourceCharacter, AttributePointReward);
			IPlayerInterface::Execute_AddToSpellPoints(EffectProp.SourceCharacter, SpellPointReward);
			bTopOffHealth = true;
			bTopOffMana = true;
			IPlayerInterface::Execute_LevelUp(EffectProp.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(EffectProp.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::DeBuff(const FEffectProperties& EffectProp)
{
	const auto& GameplayTags = FAuraGameplayTags::Get();
	auto EffectContext = EffectProp.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(EffectProp.SourceAvatarActor);

	const auto DamageType = UAuraAbilitySystemFuncLibrary::GetDamageType(EffectProp.EffectContextHandle);
	const auto DeBuffDamage = UAuraAbilitySystemFuncLibrary::GetDeBuffDamage(EffectProp.EffectContextHandle);
	const auto DeBuffDuration = UAuraAbilitySystemFuncLibrary::GetDeBuffDuration(EffectProp.EffectContextHandle);
	const auto DeBuffFrequency = UAuraAbilitySystemFuncLibrary::GetDeBuffFrequency(EffectProp.EffectContextHandle);

	const auto DeBuffName = FName{ FString::Printf(TEXT("DynamicDeBuff_%s"), *DamageType.ToString()) };
	const auto Effect = NewObject<UGameplayEffect>(GetTransientPackage(), DeBuffName);

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DeBuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DeBuffDuration);

	const auto DeBuffTag = GameplayTags.DamageTypesToDeBuffs[DamageType];
	Effect->InheritableOwnedTagsContainer.AddTag(DeBuffTag);
	if (DeBuffTag.MatchesTagExact(GameplayTags.DeBuff_Stun))
	{
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputPressed);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputHeld);
		Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo{});
	auto& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat{DeBuffDamage};
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = GetIncomingDamageAttribute();

	if (const auto MutableSpec = new FGameplayEffectSpec{Effect, EffectContext, 1.f})
	{
		const auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		const auto DeBuffDamageType = MakeShareable(new FGameplayTag{DamageType});
		AuraEffectContext->SetDamageType(DeBuffDamageType);

		EffectProp.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProp)
{
	// Source = Causer of the effect, Target = Target of the effect (Owner of this attribute set)
	EffectProp.EffectContextHandle = Data.EffectSpec.GetContext();
	EffectProp.SourceASC = EffectProp.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(EffectProp.SourceASC) &&
		EffectProp.SourceASC->AbilityActorInfo.IsValid() &&
		EffectProp.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProp.SourceAvatarActor = EffectProp.SourceASC->AbilityActorInfo->AvatarActor.Get();
		EffectProp.SourceController = EffectProp.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if (EffectProp.SourceAvatarActor && EffectProp.SourceController == nullptr)
		{
			if (const auto Pawn = Cast<APawn>(EffectProp.SourceAvatarActor))
			{
				EffectProp.SourceController = Pawn->GetController();
			}
		}
		if (EffectProp.SourceController)
		{
			EffectProp.SourceCharacter = Cast<ACharacter>(EffectProp.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProp.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProp.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProp.TargetCharacter = Cast<ACharacter>(EffectProp.TargetAvatarActor);
		EffectProp.TargetASC = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProp.TargetAvatarActor);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& EffectProp, const float Damage, const bool IsBlockedHit, const bool IsCriticalHit)
{
	if (EffectProp.SourceCharacter != EffectProp.TargetCharacter)
	{
		const auto AuraSourcePC = Cast<AAuraPlayerController>(EffectProp.SourceCharacter->Controller);
		const auto AuraTargetPC = Cast<AAuraPlayerController>(EffectProp.TargetCharacter->Controller);

		if (AuraSourcePC)
		{
			AuraSourcePC->Client_ShowDamageNumber(Damage, EffectProp.TargetCharacter, IsBlockedHit, IsCriticalHit);
			return;
		}
		if (AuraTargetPC)
		{
			AuraTargetPC->Client_ShowDamageNumber(Damage, EffectProp.TargetCharacter, IsBlockedHit, IsCriticalHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& EffectProp)
{
	if (EffectProp.TargetCharacter->Implements<UCombatInterface>())
	{
		const auto TargetLevel = ICombatInterface::Execute_GetPlayerLevel(EffectProp.TargetCharacter);
		const auto TargetClassType = ICombatInterface::Execute_GetCharacterClassType(EffectProp.TargetCharacter);
		const auto XPReward =
			UAuraAbilitySystemFuncLibrary::GetXPRewardForClassAndLevel(EffectProp.SourceCharacter, TargetClassType, TargetLevel);

		const auto GameplayTag = FAuraGameplayTags::Get().Attributes_Meta_IncomingXP;

		FGameplayEventData PayLoad;
		PayLoad.EventTag = GameplayTag;
		PayLoad.EventMagnitude = XPReward;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EffectProp.SourceCharacter, GameplayTag, PayLoad);
	}
}
