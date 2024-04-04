// Copyright fangh.space

#include "Character/AuraEnemy.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Untils/RenderDepth.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	ASComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASComponent->SetIsReplicated(true);
	ASComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AS = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	if (ASComponent)
	{
		ASComponent->InitAbilityActorInfo(this, this);
		Cast<UAuraAbilitySystemComponent>(ASComponent)->AbilityActorInfoSet();
	}
}

void AAuraEnemy::HighLightActor()
{
	bIsHighLight = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(RENDER_DEPTH_RED);
	WeaponMeshComponent->SetRenderCustomDepth(true);
	WeaponMeshComponent->SetCustomDepthStencilValue(RENDER_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
	bIsHighLight = false;
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(RENDER_DEPTH_NULL);
	WeaponMeshComponent->SetRenderCustomDepth(false);
	WeaponMeshComponent->SetCustomDepthStencilValue(RENDER_DEPTH_NULL);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}
