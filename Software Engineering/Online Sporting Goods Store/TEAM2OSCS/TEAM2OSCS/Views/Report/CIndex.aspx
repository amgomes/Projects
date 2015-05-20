<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Index
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Welcome</h2>
    <p>Please choose from one of the following options:</p>
    <div id="menucontainer">
        <ul id="menu">
            <li><%=Html.ActionLink("InventoryItemsReport", "InventoryItemsReport", "Report")%></li>
            <li><%=Html.ActionLink("Add to cart", "AddToCart", "Customer")%></li>
            <li><%=Html.ActionLink("Remove from cart", "RemoveFromCart", "Customer")%></li>
            <li><%=Html.ActionLink("Checkout", "Checkout", "Customer")%></li>
            <li><%=Html.ActionLink("ProduceInvoice", "ProduceInvoice", "Customer")%></li>
            <li><%=Html.ActionLink("OrderStatus", "OrderStatus", "Customer")%></li>
            <li><%: Html.ActionLink("AddTestimonial", "AddTestimonial", "Report")%></li>
        </ul>
    </div>

</asp:Content>
