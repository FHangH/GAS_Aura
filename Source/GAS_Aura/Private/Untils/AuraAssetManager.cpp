// Copyright fangh.space


#include "Untils/AuraAssetManager.h"
#include "Untils/AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitNativeGameplayTags();
}
