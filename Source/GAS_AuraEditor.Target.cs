// Copyright fangh.space

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_AuraEditorTarget : TargetRules
{
	public GAS_AuraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "GAS_Aura" } );
	}
}
