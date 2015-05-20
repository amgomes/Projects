using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using TEAM2OSCS.Models;
using System.Diagnostics;

namespace TEAM2OSCS.Controllers
{
    public class CategoriesController : Controller
    {
        public ActionResult Women()
        {
            //select * from Item where cat_id = 1;
            List<Inventory> inventory = Inventory.getItemsWomen();
            ViewData["Inventory"] = inventory;

            return View();
        }
        public ActionResult Kids()
        {
            //select * from Item where cat_id = 2;           
            List<Inventory> inventory = Inventory.getItemsKids();
            ViewData["Inventory"] = inventory;

            return View();
        }
        public ActionResult Men()
        {
            //select * from Item where cat_id = 3;
            List<Inventory> inventory = Inventory.getItemsMen();
            ViewData["Inventory"] = inventory;

            return View();
        }
        public ActionResult SportingGoods()
        {
            //select * from Item where cat_id = 4;
            List<Inventory> inventory = Inventory.getItemsSportingGoods();
            ViewData["Inventory"] = inventory;

            return View();
        }
    }
}