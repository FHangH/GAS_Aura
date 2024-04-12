// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class GAS_AURA_API UAT_TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;
	
	/* Function */
public:
	virtual void Activate() override;
	
	UFUNCTION(BlueprintCallable, DisplayName=TargetDataUnderMouse, Category="Aura|Task", meta=(HidePin=OwningAbility, DefaultToSelf=OwningAbility, BlueprintInternalUseOnly=true))
	static UAT_TargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	void SendMouseCursorData() const;
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, const FGameplayTag ActivationTag) const;
};
