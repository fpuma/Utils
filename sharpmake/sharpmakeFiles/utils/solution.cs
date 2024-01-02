using System.IO;

namespace Puma
{
    [Sharpmake.Generate]
    public class UtilsSolution : Puma.SharpmakeBase.ISolution
    {
        public UtilsSolution()
            : base("Utils")
        {}

        public override void ConfigureAll(Configuration conf, Sharpmake.Target target)
        {
            base.ConfigureAll(conf, target);

            conf.AddProject<Puma.UtilsTests>( target );
        }
    }
}