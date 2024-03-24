// Copyright fangh.space


#include "Gameplay/Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SphereComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(RootComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
}

void AAuraEffectActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* AuraAS =
			Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		const_cast<UAuraAttributeSet*>(AuraAS)->SetHealth(AuraAS->GetHealth() + 25.f);

		Destroy();
	}
}

void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}