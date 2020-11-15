using System.IO;

[module: Sharpmake.Include(@"..\base\*")]

//Examples
[module: Sharpmake.Include(@"..\utils\*")]

public static class SharpmakeMainClass
{
    [Sharpmake.Main]
    public static void SharpmakeMain(Sharpmake.Arguments sharpmakeArgs)
    {
        sharpmakeArgs.Generate<Puma.UtilsSolution>();
    }
}

