<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Invoice Report
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Invoice Report</h2>
          
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData1">
                <!--O_DATE Order Date -->
                <b>Order Date:</b>
            </td>
            <td class="tableData1">
                <!--METHPMT Method Of Payment -->
                <b>Method Of Payment:</b>
            </td>
            <td class="tableData1">
                <!-- INV_ID Inventory ID -->
                <b>Inventory ID:</b>
            </td>
            <td class="tableData1">
                <!-- OL_QUANTITY Quantity -->
                <b>Quantity:</b>
            </td>
            <td class="tableData1">
                <!-- OL_PRICE Price -->
                <b>Price:</b>
            </td>
        </tr>
        <% foreach (var order in (List<TEAM2OSCS.Models.OrderLine>)ViewData["Testimonials"])
           { %>
               <tr>
                   <td class="tableData1">
                       <%= order.DT%>
                   </td>
                   <td class="tableData1">
                       <%= order.Pt %>
                   </td>
                   <td class="tableData1">
                       <%= order.invID %>
                   </td>
                   <td class="tableData1">
                       <%= order.olQuantity%>
                   </td>
                   <td class="tableData1">
                       <%= order.olPrice%>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
