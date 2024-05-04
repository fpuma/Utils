namespace Puma
{
    [Sharpmake.Generate]
    class UtilsTests : Puma.SharpmakeBase.IApplication
    {
        public UtilsTests()
            : base("UtilsTests", @"utilstests")
        {
        }

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddPrivateDependency<Puma.Utils>(target);
            conf.AddPrivateDependency<Extern.GoogleTest>(target);

            conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
        }
    }
}