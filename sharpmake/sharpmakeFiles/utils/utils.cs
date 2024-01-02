namespace Puma
{
    [Sharpmake.Export]
    class Utils : Puma.SharpmakeBase.IHeaderOnly
    {
        static private readonly string sSourceFilesFolderName = @"utils";
        static public readonly string sSourceFolderPath = Puma.SharpmakeUtils.GetSourcePath() + @"\" + sSourceFilesFolderName;

        public Utils()
            : base("Utils", @"utils")
        { }

        public override void ConfigureIncludes(Configuration conf, Sharpmake.Target target)
        {
            conf.IncludePaths.Add(@"\include");
        }
    }
}