/* 
Name: ProductManagerController.cs
Description: This controller generates the views for the following reports:
                    - Total Investment Report
                    - Customer Report
                    - Inventory Report
Programmer: Erick Saucedo
Date Coded: 11/29/2014
Date Approved:
Approved By:
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using TEAM2OSCS.Models;
using System.Diagnostics;


namespace TEAM2OSCS.Controllers
{
    public class ProductManagerController : Controller
    {
        public ActionResult TotalInvestmentReport()
        {
            double investmentData = Inventory.getInvestment();
            ViewData["InvestmentReport"] = investmentData.ToString("C2");
            return View();
        }
        public ActionResult PMInventoryReport()
        {
            List<Inventory> inv = Inventory.getInventory();
            if (inv.Count == 0)
                return View("Error");
            ViewData["Inventory"] = inv;

            return View();
        }
        public ActionResult CustomerReport()
        {
            List<Customer> cust = Customer.PMGetCustomer();
            if (cust.Count == 0)
                return View("Error");
            ViewData["Customers"] = cust;
            return View();
        }
    }
}
