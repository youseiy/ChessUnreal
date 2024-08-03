using UnrealBuildTool;

public class OSSBlueprintAsync : ModuleRules
{
    public OSSBlueprintAsync(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
                ,"OnlineSubsystem",
                "OnlineSubsystemSteam",
                "OnlineSubsystemEOS",
                "OnlineSubsystemEOSPlus",
                "OnlineSubsystemUtils",
            }
        );
    }
}