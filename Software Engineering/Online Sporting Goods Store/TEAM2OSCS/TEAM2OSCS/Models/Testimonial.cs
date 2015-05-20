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
    public class Testimonial
    {
        public string testID { get; set; }
        public string content { get; set; }
        public string testDate { get; set; }
        public string custID { get; set; }
        public static List<Testimonial> AllTestimonial()
        {
            List<Testimonial> r = new List<Testimonial>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("SELECT * FROM Testimonial", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 0)
                    return r;
                int length = dt.Rows.Count;
                for (int i = 0; i < length; i++)
                {
                    Testimonial temp = new Testimonial();
                    temp.testID = (dt.Rows[i][0].ToString());
                    temp.content = (dt.Rows[i][1].ToString());
                    temp.testDate = (dt.Rows[i][2].ToString());
                    temp.custID = (dt.Rows[i][3].ToString());
                    r.Add(temp);
                }
                return r;

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
                return null;
            }
        }
        public static List<Testimonial> SearchTestimonial(string s)
        {
            List<Testimonial> r = new List<Testimonial>();
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("SELECT * FROM Testimonial", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 0)
                    return r;
                int length = dt.Rows.Count;
                for (int i = 0; i < length;i++ )
                {
                    string searchMe=dt.Rows[i][1].ToString();
                    if (searchMe.Contains(s))
                    {
                        Testimonial temp = new Testimonial();
                        temp. testID=(dt.Rows[i][0].ToString());
                        temp.content=(dt.Rows[i][1].ToString());
                        temp.testDate=(dt.Rows[i][2].ToString());
                        temp.custID=(dt.Rows[i][3].ToString());
                        r.Add(temp);
                    }
                }
                return r;

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
                return null;
            }
        }
        public static string addTestimonial(string username, string message)
        {
            SqlConnection con = new SqlConnection(@"Data Source=sqlserver.cs.uh.edu,1044;Initial Catalog=TEAM2OSCS;User ID=TEAM2OSCS;Password=TEAM2OSCS#");
            Debug.WriteLine(username);
            Debug.WriteLine(message);
            try
            {
                SqlDataAdapter mysqlid = new SqlDataAdapter("SELECT * FROM customer Where UserId = '" + username + "';", con);
                DataTable dt = new DataTable();
                mysqlid.Fill(dt);
                if (dt.Rows.Count == 0)
                    return "Not a Customer";
                SqlCommand cmd = new SqlCommand();
                cmd.CommandType = System.Data.CommandType.Text;
                cmd.CommandText = "INSERT INTO Testimonial VALUES ('" + message + "',GETDATE ()," + dt.Rows[0][0].ToString() + ");";
                cmd.Connection = con;

                con.Open();
                cmd.ExecuteNonQuery();
                con.Close();

                return " Thank you for your Testimonial";

            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
                return "NotConnectedToDatabase";
            }
        }
    }
}