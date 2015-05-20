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
    public class CustomerController : Controller
    {
        public static List<CartItem> cart = new List<CartItem>();
        public static void clearCart()
        {
            cart.Clear();
        }
        [HttpGet]
        public ActionResult AddToCart () 
        {
            ViewData["cart"] = cart;
            return View();
        }

        [HttpGet]
        public ActionResult RemoveFromCart() 
        {
            ViewData["cart"] = cart;
            return View();
        }

        [HttpGet]
        public ActionResult Checkout ()
        {
            ViewData["message"] = "Select Method of payment and Then Check out";
            ViewData["cart"] = cart;
            return View();
        }

        [HttpPost]
        public ActionResult AddToCart(FormCollection formValues)
        {
            try
            {
                int tp = Convert.ToInt32(formValues["Word"].ToString());
            }
            catch (Exception ex)
            {
                ViewData["cart"] = cart;
                return View();
            }
            int id = Convert.ToInt32(formValues["Word"].ToString());
            int am = 1;
            try
            {
                am = Convert.ToInt32(formValues["num"].ToString());
            }
            catch (Exception ex)
            {
               am = 1;
            }
            bool found = false;
            foreach (CartItem list in cart)
            {
                if(list.invID==id)
                {
                    list.amount += am;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                CartItem temp = new CartItem();
                temp.set(id, am);
                if(temp.invID!=0)
                    cart.Add(temp);
            }
            ViewData["cart"] = cart;
            return View();
        }

        [HttpPost]
        public ActionResult RemoveFromCart(FormCollection formValues)
        {
            try
            {
                int tp = Convert.ToInt32(formValues["Word"].ToString());
            }
            catch (Exception ex)
            {
                ViewData["cart"] = cart;
                return View();
            }
            int id = Convert.ToInt32(formValues["Word"].ToString());
            foreach (CartItem list in cart)
            {
                if (list.invID == id)
                {
                    cart.Remove(list);
                    break;
                }
            }
            ViewData["cart"] = cart;
            return View();
        }

        [HttpPost]
        public ActionResult Checkout(FormCollection formValues)
        {
            string pay = formValues["Word"].ToString();
            foreach (CartItem list in cart)
            {
                if (!list.checkInventory())
                {
                    ViewData["message"] = "Sorry we dont have enough "+list.itemDesc;
                    ViewData["cart"] = cart;
                    return View();
                }
            }
            string temp = "";
            if(cart.Count!=0)
                temp=cart[0].order(ReportController.getcurrentUser(), pay);
            if (temp.CompareTo("") != 0)
            {
                foreach (CartItem list in cart)
                {
                    list.orderLine(temp);
                }
                ViewData["message"] = "The order went through";
                clearCart();
                ViewData["cart"] = cart;
                return View();
            }
            else
            {

                ViewData["message"] = "Somthing went wrong";
                ViewData["cart"] = cart;
                return View();
            }
        }
        public ActionResult OrderStatus() //encountering issues
        {
            //ViewData["Testimonials"] = new List<Testimonial>();
            List<Order> order = Order.getStatus(ReportController.getcurrentUser());
            if (order.Count == 0)
            {
                return View("No orders to be displayed.");
            }
            ViewData["Order"] = order;

            return View();
        }
        public ActionResult getInvoice()
        {
            List<OrderLine> orderLine = OrderLine.getInvoice(ReportController.getcurrentUser());
            if (orderLine.Count == 0)
            {
                return View("No invoices to be displayed.");
            }
            
            ViewData["OrderLine"] = orderLine;
            
            return View();
        }
        public ActionResult ProduceInvoice()
        {
            List<OrderLine> order = OrderLine.getInventory(ReportController.getcurrentUser());
            ViewData["Testimonials"] = order;
            return View();
        }
    }
}
