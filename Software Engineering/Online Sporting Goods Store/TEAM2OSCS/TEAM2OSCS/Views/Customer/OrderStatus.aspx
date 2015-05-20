<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Order Status Report
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Order Status Report</h2>
       
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Order ID:</b>
            </td>
            <td class="tableData">
                <b>Order Date:</b>
            </td>
            <td class="tableData">
                <b>Expected Arrival: 10 days from Order Date</b>
            </td>
        </tr>
        <% foreach (var order in (List<TEAM2OSCS.Models.Order>)ViewData["Order"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= order.orderID%>
                   </td>
                   <td class="tableData">
                       <%= order.orderDate %>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
