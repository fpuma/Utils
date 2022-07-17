namespace Puma
{
    [Sharpmake.Export]
    class GoogleTest : Puma.Common.IExternBinaries
    {
        public GoogleTest()
            : base("GTest", @"googletest-release-1.12.1")
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
                conf.LibraryFiles.Add(@"gmock_main_d.lib");
                conf.LibraryFiles.Add(@"gmock_d.lib");
                conf.LibraryFiles.Add(@"gtest_main_d.lib");
                conf.LibraryFiles.Add(@"gtest_d.lib");

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