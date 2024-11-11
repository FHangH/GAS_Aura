// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighLightInterface.h"
#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighLightInterface
{
	GENERATED_BODY()

	/* Property */
private:
	// Highlight Status
	UPROPERTY(BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bIsHighLight { false };

	// Hit Reacting
	UPROPERTY(BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bHitReacting { false };

	// Life
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	float LifeSpan { 5.f };

	// Combat Target
	UPROPERTY(BlueprintReadWrite, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	TObjectPtr<AActor> CombatTarget;

protected:
	// Level
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy")
	int32 Level { 1 };

	// Heath UI
	UPROPERTY(BlueprintReadOnly, Category="Aura|Enemy")
	TObjectPtr<UWidgetComponent> HealthBar;

	// Delegate
	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|Enemy")
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;

	// AI Tree
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
	virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount) override;
	
public:
	// HighLight Interface
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& Location) override;

	// Enemy Interface
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	// Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
};
