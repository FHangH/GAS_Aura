// Copyright fangh.space

#include "Character/AuraEnemy.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Untils/RenderDepth.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	ASComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASComponent->SetIsReplicated(true);
	ASComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AS = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	if (const auto AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const auto AuraAS = Cast<UAuraAttributeSet>(AS); AuraAS && ASComponent)
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);

		OnHealthChangedDelegate.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	if (ASComponent)
	{
		ASComponent->InitAbilityActorInfo(this, this);
		Cast<UAuraAbilitySystemComponent>(ASComponent)->AbilityActorInfoSet();

		InitializeDefaultAttributes();
	}
}

void AAuraEnemy::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void AAuraEnemy::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
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
