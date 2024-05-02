// Copyright fangh.space

#include "Character/AuraEnemy.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"
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

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = {0.f, 480.f, 0.f};
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
		if (HasAuthority())
		{
			UAuraAbilitySystemFuncLibrary::GiveStartupAbilities(this, ASComponent, CharacterClassType);
		}
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
		ASComponent->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().Effects_HitReact).AddUObject(this, &ThisClass::HitReactTagChanged);

		OnHealthChangedDelegate.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority() || !NewController) return;
	AuraAIController = Cast<AAuraAIController>(NewController);

	if (!AuraAIController || !BehaviorTree)
	{
		UE_LOG(Aura, Warning, TEXT("AuraAIController Or BehaviorTree is not set for %s"), *GetName());
		return;
	}
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName{"HitReacting"}, false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName{"RangedAttacker"}, CharacterClassType != ECharacterClassType::ECT_Warrior);
}

void AAuraEnemy::InitAbilityActorInfo()
{
	if (ASComponent)
	{
		ASComponent->InitAbilityActorInfo(this, this);
		Cast<UAuraAbilitySystemComponent>(ASComponent)->AbilityActorInfoSet();

		if (HasAuthority())
		{
			InitializeDefaultAttributes();
		}
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	if (ASComponent)
	{
		UAuraAbilitySystemFuncLibrary::InitializeDefaultAttributes(this, CharacterClassType, Level, ASComponent);
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

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	if (!AuraAIController) return;
	
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName{"HitReacting"}, bHitReacting);
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

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	
	Super::Die();
}
