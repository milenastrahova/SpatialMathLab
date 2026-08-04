using UnrealBuildTool;

public class SpatialMathLabTarget : TargetRules
{
    public SpatialMathLabTarget(TargetInfo Target)
        : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("SpatialMathLab");
    }
}