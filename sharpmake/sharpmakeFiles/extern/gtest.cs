namespace Puma
{
    [Sharpmake.Export]
    class GoogleTest : Puma.Common.IExternBinaries
    {
        public GoogleTest()
            : base("GTest", @"googletest-release-1.10.0")
        { }

        public override void ConfigureIncludes(Configuration conf, Sharpmake.Target target)
        {
            conf.IncludePaths.Add(@"\include");
        }
        
        public override void ConfigureLink(Configuration conf, Sharpmake.Target target)
        {
            conf.LibraryPaths.Add(SourceRootPath + @"\lib");

            if (target.Optimization == Sharpmake.Optimization.Debug)
            {
                conf.LibraryFiles.Add(@"gmock_maind.lib");
                conf.LibraryFiles.Add(@"gmockd.lib");
                conf.LibraryFiles.Add(@"gtest_maind.lib");
                conf.LibraryFiles.Add(@"gtestd.lib");

            }
            else
            {
                conf.LibraryFiles.Add(@"gmock_main.lib");
                conf.LibraryFiles.Add(@"gmock.lib");
                conf.LibraryFiles.Add(@"gtest_main.lib");
                conf.LibraryFiles.Add(@"gtest.lib");
            }

        }
    }
}