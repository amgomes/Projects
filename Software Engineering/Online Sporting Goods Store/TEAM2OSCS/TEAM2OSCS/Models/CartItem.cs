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
    public class CartItem
    {
        public int invID { get; set; }
        public int itemID { get; set; }
        public string color { get; set; }
        public string size { get; set; }
        public double invPrice { get; set; }
        public int amount { get; set; } 
        public string itemDesc { get; set; }
        public string category { get; set; }
        public void set(int inventoryID, int am)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select * from Inventory where INV_ID ="+inventoryID+";", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 1)
                {
                    invID = Convert.ToInt32(dt.Rows[0][0].ToString());
                    itemID = Convert.ToInt32(dt.Rows[0][1].ToString());
                    color = dt.Rows[0][2].ToString();
                    size = dt.Rows[0][3].ToString();
                    invPrice = Convert.ToDouble(dt.Rows[0][4].ToString());
                    amount = am;

                    SqlDataAdapter mysqlid2 = new SqlDataAdapter("select * from Item where ITEM_ID =" + itemID + ";", con);
                    DataTable dt2 = new DataTable();
                    mysqlid2.Fill(dt2);

                    itemDesc = dt2.Rows[0][1].ToString();
                    int catnum = Convert.ToInt32(dt.Rows[0][1].ToString());

                    SqlDataAdapter mysqlid3 = new SqlDataAdapter("select * from CATEGORY where CAT_ID =" + catnum + ";", con);
                    DataTable dt3 = new DataTable();
                    mysqlid2.Fill(dt3);

                    category = dt3.Rows[0][1].ToString();

                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
        }
        public bool checkInventory()
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select * from Inventory where INV_ID =" + invID + ";", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 1)
                {
                    if (amount <= Convert.ToInt32(dt.Rows[0][5].ToString()))
                        return true;
                }
                return false;
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString()); 
                return false;
            }
            
        }
        public string order(string UserName, string payment)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select * from CUSTOMER where UserID ='" + UserName + "';", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                string CID=dt.Rows[0][0].ToString();
                SqlCommand cmdO = new SqlCommand();
                cmdO.CommandType = System.Data.CommandType.Text;
                cmdO.CommandText = "INSERT INTO ORDERS VALUES(GETDATE (), '"+payment+"', "+CID+", 6);";
                cmdO.Connection = con;

                con.Open();
                cmdO.ExecuteNonQuery();
                con.Close();

                SqlDataAdapter mysqlid2 = new SqlDataAdapter("select O_ID from orders order By O_ID DESC;", con);
                DataTable dt2 = new DataTable();
                mysqlid2.Fill(dt2);
                return dt2.Rows[0][0].ToString();

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
                return "";
            }
        }
        public void orderLine(string OID)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlCommand cmdOL = new SqlCommand();
                cmdOL.CommandType = System.Data.CommandType.Text;
                cmdOL.CommandText = "INSERT INTO ORDER_LINE VALUES(" + OID + ", " + invID + ", " + amount + ", " + invPrice + ");";
                cmdOL.Connection = con;

                con.Open();
                cmdOL.ExecuteNonQuery();
                con.Close();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
        }

        public void setQOH(int inventoryID)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select * from Inventory where INV_ID =" + inventoryID + ";", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 1)
                {
                    invID = Convert.ToInt32(dt.Rows[0][0].ToString());
                    itemID = Convert.ToInt32(dt.Rows[0][1].ToString());
                    color = dt.Rows[0][2].ToString();
                    size = dt.Rows[0][3].ToString();
                    invPrice = Convert.ToDouble(dt.Rows[0][4].ToString());
                    amount = Convert.ToInt32(dt.Rows[0][5].ToString()); ;

                    SqlDataAdapter mysqlid2 = new SqlDataAdapter("select * from Item where ITEM_ID =" + itemID + ";", con);
                    DataTable dt2 = new DataTable();
                    mysqlid2.Fill(dt2);

                    itemDesc = dt2.Rows[0][1].ToString();
                    int catnum = Convert.ToInt32(dt.Rows[0][1].ToString());

                    SqlDataAdapter mysqlid3 = new SqlDataAdapter("select * from CATEGORY where CAT_ID =" + catnum + ";", con);
                    DataTable dt3 = new DataTable();
                    mysqlid2.Fill(dt3);

                    category = dt3.Rows[0][1].ToString();

                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
        }

        public static List<CartItem> getEverything()
        {
            List<CartItem> r =new List<CartItem>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("select INV_ID from Inventory;", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                int length = dt.Rows.Count;
                for (int i = 0; i < length; i++)
                {
                    CartItem temp = new CartItem();
                    temp.setQOH(Convert.ToInt32(dt.Rows[i][0].ToString()));
                    r.Add(temp);
                }

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            return r;
        }
    }
}