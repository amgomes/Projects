using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Security.Principal;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;
using System.Web.Security;
using TEAM2OSCS.Models;

namespace TEAM2OSCS.Controllers
{
    public class ReportController : Controller
    {
        static string currentUser = "Visitor";
        public static void setCurrentUser(string input)
        {
            currentUser=input;
        }
        public static string getcurrentUser()
        {
            return currentUser;
        }
        public ActionResult Report()
        {
            if (currentUser.CompareTo("MM") == 0)
                return View("MMIndex");
            if (currentUser.CompareTo("RC") == 0)
                return View("RCIndex");
            if (currentUser.CompareTo("PM") == 0)
                return View("PMIndex");
            if (currentUser.CompareTo("Visitor") != 0)
                return View("CIndex");
            return View();
        }
        [HttpGet]
        public ActionResult SearchTestimonial()
        {
            ViewData["Testimonials"] = Testimonial.AllTestimonial();
            return View();
        }
        [HttpPost]
        public ActionResult SearchTestimonial(FormCollection formValues)
        {
            if (formValues["searchWord"].ToString().CompareTo("") == 0)
            {
                ViewData["Testimonials"] = Testimonial.AllTestimonial();
                return View();
            }
            else
            {
                List<Testimonial> list = Testimonial.SearchTestimonial("" + formValues["searchWord"]);
                if (list == null)
                {
                    ViewData["Testimonials"] = new List<Testimonial>();
                    return View();
                }
                ViewData["Testimonials"] = list;
            }
            return View();
        }
        [HttpGet]
        public ActionResult AddTestimonial()
        {
            ViewData["Message"] = "add Testimonial";
            return View();
        }
        [HttpPost]
        public ActionResult AddTestimonial(FormCollection formValues)
        {
            if (formValues["T"].ToString().CompareTo("") == 0)
            {
                ViewData["Message"] = "Nothing in the box";
                return View();
            }
            else
            {
                ViewData["Message"] = Testimonial.addTestimonial(getcurrentUser(), formValues["T"].ToString());
                return View();
            }
        }
        public ActionResult InventoryItemsReport()
        {
            List<CartItem> inventory = CartItem.getEverything();
            ViewData["Inventory"] = inventory;
            return View();
        }
    }
}
