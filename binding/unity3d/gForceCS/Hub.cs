using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace gf
{
    public sealed class Hub
    {
        public static Hub Instance
        {
            get
            {
                return instance;
            }
        }

        public int init(int comport)
        {
            return gForce.hub_init(comport);
        }

        public int deinit()
        {
            return gForce.hub_deinit();
        }

        private static readonly Hub instance = new Hub();

        static Hub()
        {
        }

        private Hub()
        {
        }
    }

}
