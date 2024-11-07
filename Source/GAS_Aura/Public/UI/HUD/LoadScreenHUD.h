// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class ULoadScreenWidget;
class UMVVM_LoadScreen;

UCLASS()
class GAS_AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()

	/* Property */
public:
	// Widget
	UPROPERTY(EditDefaultsOnly, Category="HUD|LoadScreen")
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category="HUD|LoadScreen")
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	// MVVM
	UPROPERTY(EditDefaultsOnly, Category="HUD|MVVM")
	TSubclassOf<UMVVM_LoadScreen> MVVM_LoadScreenClass;

	UPROPERTY(BlueprintReadOnly, Category="HUD|MVVM")
	TObjectPtr<UMVVM_LoadScreen> MVVM_LoadScreen;
	
	/* Function */
protected:
	virtual void BeginPlay() override;

	void SetUserInputMode() const;
};
