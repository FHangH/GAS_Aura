// Copyright fangh.space

#include "Character/AuraEnemy.h"
#include "Untils/RenderDepth.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
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
