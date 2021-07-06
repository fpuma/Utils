using System.IO;

[module: Sharpmake.Include(@"..\base\*")]

[module: Sharpmake.Include(@"..\utils\*")]
[module: Sharpmake.Include(@"..\extern\*")]

public static class SharpmakeMainClass
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<Puma.UtilsSolution>();
    }
}

