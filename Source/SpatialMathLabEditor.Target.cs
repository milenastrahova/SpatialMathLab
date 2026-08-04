using UnrealBuildTool;

public class SpatialMathLabEditorTarget : TargetRules
{
    public SpatialMathLabEditorTarget(TargetInfo Target)
        : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("SpatialMathLab");
    }
}