using UnrealBuildTool;

public class SpatialMathLab : ModuleRules
{
    public SpatialMathLab(ReadOnlyTargetRules Target)
        : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "SpatialMathCore"
            }
        );
    }
}
