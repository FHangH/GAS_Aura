// CopyRight fangh.space

#pragma once

#include "CoreMinimal.h"

// Tick Frequency PerSecond
#define TICK_0 0.f
#define TICK_1 1.f
#define TICK_3 0.3333333f
#define TICK_5 0.2f
#define TICK_10 0.1f
#define TICK_15 0.0666666f
#define TICK_20 0.05f
#define TICK_30 0.0333333f
#define TICK_45 0.0222222f
#define TICK_60 0.0166667f
#define TICK_90 0.0111111f
#define TICK_120 0.00833333f

UENUM(BlueprintType)
enum class ETICK_RATE : uint8
{
	ER_TICK_0		UMETA(Displayname="TICK_0"),
	ER_TICK_1		UMETA(Displayname="TICK_1"),
	ER_TICK_3		UMETA(Displayname="TICK_3"),
	ER_TICK_5		UMETA(Displayname="TICK_5"),
	ER_TICK_10		UMETA(Displayname="TICK_10"),
	ER_TICK_15		UMETA(Displayname="TICK_15"),
	ER_TICK_20		UMETA(Displayname="TICK_20"),
	ER_TICK_30		UMETA(Displayname="TICK_30"),
	ER_TICK_45		UMETA(Displayname="TICK_45"),
	ER_TICK_60		UMETA(Displayname="TICK_60"),
	ER_TICK_90		UMETA(Displayname="TICK_90"),
	ER_TICK_120		UMETA(Displayname="TICK_120"),
	ER_TICK_Ultra	UMETA(Displayname="TICK_Ultra"),
};

FORCEINLINE static float TickValue(const ETICK_RATE& ER)
{
	if (ER == ETICK_RATE::ER_TICK_0) return TICK_0;
	if (ER == ETICK_RATE::ER_TICK_1) return TICK_1;
	if (ER == ETICK_RATE::ER_TICK_3) return TICK_3;
	if (ER == ETICK_RATE::ER_TICK_5) return TICK_5;
	if (ER == ETICK_RATE::ER_TICK_10) return TICK_10;
	if (ER == ETICK_RATE::ER_TICK_15) return TICK_15;
	if (ER == ETICK_RATE::ER_TICK_20) return TICK_20;
	if (ER == ETICK_RATE::ER_TICK_30) return TICK_30;
	if (ER == ETICK_RATE::ER_TICK_45) return TICK_45;
	if (ER == ETICK_RATE::ER_TICK_60) return TICK_60;
	if (ER == ETICK_RATE::ER_TICK_90) return TICK_90;
	if (ER == ETICK_RATE::ER_TICK_120) return TICK_120;
	return TICK_0;
}