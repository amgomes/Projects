/* 
Name: MarketingManagerController.cs
Description: This controller generates the views for the following reports:
                    - Track Orders Source Report
                    - Total Sales Report
                    - Discounts Report
Programmer: Adam Gomes
Date Coded: 11/27/2014
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
    public class MarketingManagerController : Controller
    {
        public ActionResult TrackOrdersSource()
        {
            List<Order> orders = Order.getOrders();
            if (orders.Count == 0)
                return View("Error");
            ViewData["Orders"] = orders;

            return View();
        }
        public ActionResult TotalSalesReport()
        {
            double websiteSales = Order.getWebsiteSales();
            ViewData["WebSiteSales"] = websiteSales.ToString("C2");
            return View();
        }
        public ActionResult DiscountsReport()
        {
            List<Order> orders = Order.getDiscountedTotals();
            if (orders.Count == 0)
                return View("Error");
            ViewData["Orders"] = orders;
            return View();
        }
    }
}