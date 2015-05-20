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
    public class Inventory
    {
        public int invID { get; set; }
        public int itemID { get; set; }
        public string color { get; set; }
        public string size { get; set; }
        public double invPrice { get; set; }
        public int qoh { get; set; }
        public int category { get; set; }
        public string itemDesc { get; set; }
        public string image { get; set; }

        public void updateQOH(int Quantity)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlCommand cmd = new SqlCommand();
                cmd.CommandType = System.Data.CommandType.Text;
                cmd.CommandText = "update inventory set INV_QOH = " + (qoh + Quantity) + " where INV_ID = " + invID + ";";
                cmd.Connection = con;

                con.Open();
                cmd.ExecuteNonQuery();
                con.Close();

                qoh += Quantity;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
        }
        public static double getInvestment()
        {
            double investment = 0.0;
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get inventory info
                SqlCommand cmd = new SqlCommand("SELECT * FROM INVENTORY;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        int quantity = reader.GetInt32(5);
                        double price = (double)reader.GetFloat(4);
                        investment = investment + price * quantity;
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return investment;
        }

        public static List<Inventory> getInventory()
        {
            List<Inventory> inv = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get orders
                SqlCommand cmd = new SqlCommand("SELECT * FROM INVENTORY;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Inventory temp = new Inventory();
                        temp.invID = reader.GetInt32(0);
                        temp.itemID = reader.GetInt32(1);
                        temp.invPrice = (double)reader.GetFloat(4);
                        temp.qoh = reader.GetInt32(5);
                        inv.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return inv;
        }
        public static List<Inventory> getItemsWomen()
        {
            List<Inventory> inventory = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get inventory
                SqlCommand cmd = new SqlCommand("SELECT ITEM.ITEM_IMAGE, INVENTORY.INV_ID, ITEM.ITEM_DESC, INVENTORY.COLOR, INVENTORY.INV_SIZE, INVENTORY.INV_PRICE, INVENTORY.INV_QOH FROM INVENTORY JOIN ITEM ON INVENTORY.ITEM_ID = ITEM.ITEM_ID WHERE ITEM.CAT_ID = 1;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Inventory temp = new Inventory();
                        temp.image = reader.GetString(0);
                        temp.invID = reader.GetInt32(1);
                        temp.itemDesc = reader.GetString(2);
                        temp.color = reader.GetString(3);
                        if (reader.IsDBNull(4))
                        {
                            temp.size = "N/A";
                        }
                        else
                        {
                            temp.size = reader.GetString(4);
                        }
                        temp.invPrice = (double)reader.GetFloat(5);
                        temp.qoh = reader.GetInt32(6);
                        inventory.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Exception is being thrown!!!");
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return inventory;
        }
        public static List<Inventory> getItemsKids()
        {
            List<Inventory> inventory = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get inventory
                SqlCommand cmd = new SqlCommand("SELECT ITEM.ITEM_IMAGE, INVENTORY.INV_ID, ITEM.ITEM_DESC, INVENTORY.COLOR, INVENTORY.INV_SIZE, INVENTORY.INV_PRICE, INVENTORY.INV_QOH FROM INVENTORY JOIN ITEM ON INVENTORY.ITEM_ID = ITEM.ITEM_ID WHERE ITEM.CAT_ID = 2;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Inventory temp = new Inventory();
                        temp.image = reader.GetString(0);
                        temp.invID = reader.GetInt32(1);
                        temp.itemDesc = reader.GetString(2);
                        temp.color = reader.GetString(3);
                        if (reader.IsDBNull(4))
                        {
                            temp.size = "N/A";
                        }
                        else
                        {
                            temp.size = reader.GetString(4);
                        }
                        temp.invPrice = (double)reader.GetFloat(5);
                        temp.qoh = reader.GetInt32(6);
                        inventory.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Exception is being thrown!!!");
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return inventory;
        }
        public static List<Inventory> getItemsMen()
        {
            List<Inventory> inventory = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get inventory
                SqlCommand cmd = new SqlCommand("SELECT ITEM.ITEM_IMAGE, INVENTORY.INV_ID, ITEM.ITEM_DESC, INVENTORY.COLOR, INVENTORY.INV_SIZE, INVENTORY.INV_PRICE, INVENTORY.INV_QOH FROM INVENTORY JOIN ITEM ON INVENTORY.ITEM_ID = ITEM.ITEM_ID WHERE ITEM.CAT_ID = 3;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Inventory temp = new Inventory();
                        temp.image = reader.GetString(0);
                        temp.invID = reader.GetInt32(1);
                        temp.itemDesc = reader.GetString(2);
                        temp.color = reader.GetString(3);
                        if (reader.IsDBNull(4))
                        {
                            temp.size = "N/A";
                        }
                        else
                        {
                            temp.size = reader.GetString(4);
                        }
                        temp.invPrice = (double)reader.GetFloat(5);
                        temp.qoh = reader.GetInt32(6);
                        inventory.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Exception is being thrown!!!");
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return inventory;
        }
        public static List<Inventory> getItemsSportingGoods()
        {
            List<Inventory> inventory = new List<Inventory>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get inventory
                SqlCommand cmd = new SqlCommand("SELECT ITEM.ITEM_IMAGE, INVENTORY.INV_ID, ITEM.ITEM_DESC, INVENTORY.COLOR, INVENTORY.INV_SIZE, INVENTORY.INV_PRICE, INVENTORY.INV_QOH FROM INVENTORY JOIN ITEM ON INVENTORY.ITEM_ID = ITEM.ITEM_ID WHERE ITEM.CAT_ID = 4;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Inventory temp = new Inventory();
                        temp.image = reader.GetString(0);
                        temp.invID = reader.GetInt32(1);
                        temp.itemDesc = reader.GetString(2);
                        temp.color = reader.GetString(3);
                        if (reader.IsDBNull(4))
                        {
                            temp.size = "N/A";
                        }
                        else
                        {
                            temp.size = reader.GetString(4);
                        }
                        temp.invPrice = (double)reader.GetFloat(5);
                        temp.qoh = reader.GetInt32(6);
                        inventory.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine("Exception is being thrown!!!");
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return inventory;
        }
    }
}