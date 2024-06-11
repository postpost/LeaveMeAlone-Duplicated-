// LeaveMeAlone game by MagicBit. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class LeaveMeAloneTarget : TargetRules
{
	public LeaveMeAloneTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "LeaveMeAlone" } );
	}
}
