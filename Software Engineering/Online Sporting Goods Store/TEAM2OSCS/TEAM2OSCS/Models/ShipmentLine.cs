using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TEAM2OSCS.Models
{
    public class ShipmentLine
    {
        public int shipID { get; set; }
        public int invID { get; set; }
        public int slQuantity { get; set; }
        public string slDateReceived { get; set; }
    }
}