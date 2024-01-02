namespace Puma
{
    [Sharpmake.Generate]
    class UtilsTests : Puma.SharpmakeBase.IApplication
    {
        public UtilsTests()
            : base("UtilsTests", @"utilstests")
        {
            AdditionalSourceRootPaths.Add(Puma.Utils.sSourceFolderPath);
        }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPrivateDependency<Puma.Utils>(target);
            conf.AddPrivateDependency<Puma.GoogleTest>(target);

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    }
}