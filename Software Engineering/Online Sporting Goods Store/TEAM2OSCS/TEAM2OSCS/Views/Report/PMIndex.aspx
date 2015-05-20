<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>
<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	PMIndex
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Welcome Product Manager</h2>
    <p>Please choose from one of the following options:</p>
    <ul>
        <li><%=Html.ActionLink("Total Investment Report", "TotalInvestmentReport", "ProductManager")%></li>
        <li><%=Html.ActionLink("Customer Report", "CustomerReport", "ProductManager")%></li>
        <li><%=Html.ActionLink("Inventory Report", "PMInventoryReport", "ProductManager")%></li>
    </ul>

</asp:Content>

