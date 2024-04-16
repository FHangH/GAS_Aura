// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bIsHighLight {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	int32 Level {1};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	ECharacterClassType CharacterClassType {ECharacterClassType::ECT_Warrior};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bHitReacting {false};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	float BaseWalkSpeed {250.f};
	
	/* Function */
public:
	AAuraEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);
	
public:
	// Enemy Interface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;
};
