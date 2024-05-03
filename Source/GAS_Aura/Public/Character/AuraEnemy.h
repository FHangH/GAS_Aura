// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bIsHighLight {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bHitReacting {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	float BaseWalkSpeed {250.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	float LifeSpan {5.f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	TObjectPtr<AActor> CombatTarget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy")
	int32 Level {1};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy")
	ECharacterClassType CharacterClassType {ECharacterClassType::ECT_Warrior};

	UPROPERTY(BlueprintReadOnly, Category="Aura|Enemy")
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;
	
	UPROPERTY(EditAnywhere, Category="Aura|Enemy|AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY(BlueprintReadOnly, Category="Aura|Enemy|AI")
	TObjectPtr<AAuraAIController> AuraAIController;
	
	/* Function */
public:
	AAuraEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// Override AuraCharacterBase
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	// Delegate
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount);
	
public:
	// Enemy Interface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
};
