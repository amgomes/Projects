/* 
Name: OrderLine.cs
Description: This model is used to gather information from the database for each line in a order.
Programmer: Adam Gomes
Date Coded: 11/27/2014
Date Approved:
Approved By:
*/

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
    public class OrderLine
    {
        public int orderID { get; set; }
        public int invID { get; set; }
        public int olQuantity { get; set; }
        public double olPrice { get; set; }
        public double total { get; set; }
        public string Pt { get; set; }
        public string DT { get; set; }

        public static double getOrderTotal(int orderID)
        {
            List<double> orders = new List<double>();
            double total = 0.00;
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get orders
                SqlCommand cmd = new SqlCommand("SELECT OL_QUANTITY, OL_PRICE FROM ORDER_LINE WHERE O_ID = " + orderID + ";", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        int quantity = reader.GetInt32(0);
                        double price = (double)reader.GetFloat(1);
                        total = total + (quantity * price);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return total;
        }
        public static List<OrderLine> getInvoice(string UserName)
        {
            List<OrderLine> ordersLine = new List<OrderLine>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get original total
                SqlCommand cmd = new SqlCommand("SELECT * FROM ORDER_LINE WHERE UserID ='" + UserName + "';", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        OrderLine temp = new OrderLine();
                        temp.orderID = reader.GetInt32(0);
                        temp.invID = reader.GetInt32(1);
                        temp.olPrice = (double)reader.GetFloat(2);
                        temp.olQuantity = reader.GetInt32(0);
                        ordersLine.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return ordersLine;
        }
        public static List<OrderLine> getInventory(string Username)
        {
            List<OrderLine> r = new List<OrderLine>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get original total
                SqlDataAdapter mysqlid = new SqlDataAdapter("select OL.*, O.METHPMT,O.O_date from Order_line OL, Orders O, Customer C where C.C_ID = O.C_ID and OL.O_ID = O.O_ID and C.userID = '" + Username + "';", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                int rows = dt.Rows.Count;
                for (int i = 0; i < rows; i++)
                {
                    OrderLine temp = new OrderLine();
                    temp.orderID = Convert.ToInt32(dt.Rows[i][0].ToString());
                    temp.invID = Convert.ToInt32(dt.Rows[i][1].ToString());
                    temp.olQuantity = Convert.ToInt32(dt.Rows[i][2].ToString());
                    temp.olPrice = Convert.ToDouble(dt.Rows[i][3].ToString());
                    temp.Pt = dt.Rows[i][4].ToString();
                    temp.DT = dt.Rows[i][5].ToString();
                    r.Add(temp);
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return r;
        }
    }
}