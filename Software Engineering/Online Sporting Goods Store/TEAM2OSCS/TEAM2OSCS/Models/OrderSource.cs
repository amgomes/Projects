/* 
Name: Order.cs
Description: This model is used to gather information from the database for each order's source.
Programmer: Adam Gomes
Date Coded: 11/27/2014
Date Approved:
Approved By:
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data.SqlClient;
using System.Diagnostics;

namespace TEAM2OSCS.Models
{
    public class OrderSource
    {
        public int osID { get; set; }
        public string osDesc { get; set; }

    }
}