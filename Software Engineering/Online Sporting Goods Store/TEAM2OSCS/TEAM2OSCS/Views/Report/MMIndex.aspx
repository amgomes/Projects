<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Index
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Welcome Marketing Manager</h2>
    <p>Please choose from one of the following options:</p>
    <ul>
        <li><%=Html.ActionLink("Track Orders Source", "TrackOrdersSource", "MarketingManager")%></li>
        <li><%=Html.ActionLink("Total Sales Report", "TotalSalesReport", "MarketingManager")%></li>
        <li><%=Html.ActionLink("Discounts Report", "DiscountsReport", "MarketingManager")%></li>
    </ul>

</asp:Content>
