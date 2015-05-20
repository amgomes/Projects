using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;
using System.Globalization;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Security;

using System.Data;
using System.Data.SqlClient;
using System.Diagnostics;


namespace TEAM2OSCS.Models
{

    public class Shipment
    {
        public string shipID { get; set; }
        public string shipDateExpected { get; set; }
        public string Inv_id { get; set; }
        public static List<Shipment> IncomingShipmentReport()
        {
            List<Shipment> r = new List<Shipment>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select s.* , sl.INV_ID from Shipment s , Shipment_Line sl  where s.SHIP_DATE_EXPCTED < GETDATE () and sl.SHIP_ID = s.SHIP_ID;", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                int rows = dt.Rows.Count;
                for (int i = 0; i < rows; i++)
                {
                    Shipment temp =new Shipment();
                    temp.shipID =  dt.Rows[i][0].ToString();
                    temp.shipDateExpected = dt.Rows[i][1].ToString();
                    temp.Inv_id = dt.Rows[i][2].ToString();
                    r.Add(temp);
                }
                return r;
    
            }
            catch (Exception ex)
            {
                return null;
            }
        }
        public static List<Shipment> OutstandingShipmentReport()
        {
            List<Shipment> r = new List<Shipment>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select s.* , sl.INV_ID from Shipment s , Shipment_Line sl  where s.SHIP_DATE_EXPCTED >= GETDATE () and sl.SHIP_ID = s.SHIP_ID;", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                int rows = dt.Rows.Count;
                for (int i = 0; i < rows; i++)
                {
                    Shipment temp = new Shipment();
                    temp.shipID = dt.Rows[i][0].ToString();
                    temp.shipDateExpected = dt.Rows[i][1].ToString();
                    temp.Inv_id = dt.Rows[i][2].ToString();
                    r.Add(temp);
                }
                return r;

            }
            catch (Exception ex)
            {
                return null;
            }
        }
        public static List<Inventory> updatesInventoryforShipment(string s)
        {
            List<Inventory> r = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select i.*, s.SL_QUANTITY from inventory i, Shipment_line s where i.INV_ID = s.INV_ID and s.Ship_ID = "+s+";", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                int rows = dt.Rows.Count;
                for (int i = 0; i < rows; i++)
                {
                    Inventory temp = new Inventory();
                    temp.invID = Convert.ToInt32(dt.Rows[i][0].ToString());
                    temp.itemID = Convert.ToInt32(dt.Rows[i][1].ToString());
                    temp.color = dt.Rows[i][2].ToString();
                    temp.size = dt.Rows[i][3].ToString();
                    temp.invPrice = Convert.ToDouble(dt.Rows[i][4].ToString());
                    temp.qoh = Convert.ToInt32(dt.Rows[i][5].ToString());
                    temp.updateQOH(Convert.ToInt32(dt.Rows[i][6].ToString()));
                    r.Add(temp);
                }
                return r;

            }
            catch (Exception ex)
            {
                return null;
            }
        }
    }
}