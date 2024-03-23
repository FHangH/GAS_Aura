// Copyright fangh.space

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_AuraTarget : TargetRules
{
	public GAS_AuraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GAS_Aura" } );
	}
}
