using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Data;
using System.Data.SqlClient;
using System.Diagnostics;


namespace TEAM2OSCS.Models
{
    public class Customer
    {
        public int custID { get; set; }
        public string lastName { get; set; }
        public string firstName { get; set; }
        public char middleInitial { get; set; }
        public string dob { get; set; }
        public string addr { get; set; }
        public string city { get; set; }
        public string state { get; set; }
        public int zip { get; set; }
        public string phoneNumber { get; set; }
        public string email { get; set; }
        public string username { get; set; }

        public static List<Customer> PMGetCustomer()
        {
            //for Product Manager Use only.  Omits some data others may find useful
            List<Customer> custs = new List<Customer>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            con.Open();
            try
            {
                //get orders
                SqlCommand cmd = new SqlCommand("SELECT * FROM CUSTOMER;", con);
                using (SqlDataReader reader = cmd.ExecuteReader())
                {
                    while (reader.Read())
                    {
                        Customer temp = new Customer();
                        temp.custID = reader.GetInt32(0);
                        temp.lastName = reader.GetString(1);
                        temp.firstName = reader.GetString(2);
                        temp.addr = reader.GetString(5);
                        temp.city = reader.GetString(6);
                        temp.state = reader.GetString(7);
                        temp.email = reader.GetString(10);
                        temp.username = reader.GetString(11);
                        custs.Add(temp);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
            }
            con.Close();
            return custs;
        }
    }
}