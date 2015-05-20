using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace TEAM2OSCS.Models
{
    public class Item
    {
        public int itemID { get; set; }
        public string itemDesc { get; set; }
        public int catID { get; set; }
        public string item_Image { get; set; }
    }
}