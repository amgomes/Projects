<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	DiscountsReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">
<h2>Welcome</h2>

    <div id="menucontainer">
        <ul id="menu">    
            <li><%: Html.ActionLink("InventoryItemsReport", "InventoryItemsReport", "Report")%></li>
        </ul>
    </div>

</asp:Content>