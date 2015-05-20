using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using TEAM2OSCS.Models;
using System.Diagnostics;

namespace TEAM2OSCS.Controllers
{
    public class ReceivingClerkController : Controller
    {
        public ActionResult IncomingShipmentsReport()
        {
            List<Shipment> list = Shipment.IncomingShipmentReport();
            ViewData["Info"] = list;
            return View();
        }
        public ActionResult OutstandingShipmentsReport()
        {
            List<Shipment> list = Shipment.OutstandingShipmentReport();
            ViewData["Info"] = list;
            return View();
        }

        [HttpGet]
        public ActionResult updatesInventoryforShipment()
        {
            ViewData["Message"] = "No Shipment";
            ViewData["Info"]=new List<Inventory>();
            return View();
        }
        [HttpPost]
        public ActionResult updatesInventoryforShipment(FormCollection formValues)
        {
            List<Inventory> list =Shipment.updatesInventoryforShipment(formValues["ID"].ToString());
            if(list == null)
            {
                ViewData["Message"] = "No Shipment";
                ViewData["Info"]=new List<Inventory>();
                return View();
            }
            if(list.Count==0)
                ViewData["Message"] = "No Shipment";
            else
                ViewData["Message"] = "Inventory Updated";
            ViewData["Info"] = list;
            return View();
        }
    }
}
