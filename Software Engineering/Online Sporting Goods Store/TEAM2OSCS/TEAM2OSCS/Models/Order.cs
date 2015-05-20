/* 
Name: Order.cs
Description: This model is used to gather information from the database for each order.
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
    public class Order
    {
        public int orderID { get; set; }
        public string orderDate { get; set; }
        public string methodPayment { get; set; }
        public int custID { get; set; }
        public int osID { get; set; }
        public string osDesc { get; set; }
        public double originalTotal { get; set; }
        public double discountedTotal { get; set; }

        public static double getWebsiteSales()
        {
            List<Order> orders = new List<Order>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get original total
                SqlCommand cmd = new SqlCommand("SELECT * FROM ORDERS WHERE OS_ID = 6;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Order temp = new Order();
                        temp.orderID = reader.GetInt32(0);
                        temp.originalTotal = OrderLine.getOrderTotal(temp.orderID);
                        orders.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();

            //get total website sales
            double websiteSales = 0.00;
            foreach (var order in orders)
            {
                websiteSales = websiteSales + order.originalTotal;
            }
            return websiteSales;
        }

        public static List<Order> getOrders()
        {
            List<Order> orders = new List<Order>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get orders
                SqlCommand cmd = new SqlCommand("SELECT * FROM ORDERS;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Order temp = new Order();
                        temp.orderID = reader.GetInt32(0);
                        DateTime date = reader.GetDateTime(1);
                        temp.orderDate = date.ToString();
                        temp.methodPayment = reader.GetString(2);
                        temp.custID = reader.GetInt32(3);
                        temp.osID = reader.GetInt32(4);
                        orders.Add(temp);
                    }
                }
                //get order source descriptions
                foreach (var order in orders)
                {
                    //get order source desc
                    SqlDataAdapter mysqlid = new SqlDataAdapter("SELECT OS_DESC FROM OrderSource WHERE OS_ID = " + order.osID + ";", con);
                    DataTable dt = new DataTable();
                    mysqlid.Fill(dt);
                    order.osDesc = dt.Rows[0][0].ToString();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return orders;
        }

        public static List<Order> getDiscountedTotals()
        {
            List<Order> orders = new List<Order>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get original total
                SqlCommand cmd = new SqlCommand("SELECT * FROM ORDERS;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Order temp = new Order();
                        temp.orderID = reader.GetInt32(0);
                        temp.originalTotal = OrderLine.getOrderTotal(temp.orderID);
                        orders.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();

            //calculate discounted totals
            foreach (var order in orders)
            {
                if (order.originalTotal > 200.00)
                {
                    order.discountedTotal = order.originalTotal - (order.originalTotal * .2);
                }
                else if (order.originalTotal > 100.00)
                {
                    order.discountedTotal = order.originalTotal - (order.originalTotal * .1);
                }
                else order.discountedTotal = -1;
            }
            return orders;
        }
        public static List<Order> getStatus(string UserName)
        {
            List<Order> orders = new List<Order>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get original total
                SqlCommand cmd = new SqlCommand("select O.* from orders O, Customer C where c.UserID = '"+UserName+"' and O.C_ID=C.C_ID;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Order temp = new Order();
                        temp.orderID = reader.GetInt32(0);
                        DateTime date = reader.GetDateTime(1);
                        temp.orderDate = date.ToString();
                        orders.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return orders;
        }
    }
}