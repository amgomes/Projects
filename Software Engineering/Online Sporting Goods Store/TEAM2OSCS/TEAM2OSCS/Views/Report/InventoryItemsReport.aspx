<%-- 
Name: InventoryItemsReport.aspx
Description: This view displays the Inventory Item Report available to Customers and Visitors
Programmer: Vincent Diaz
Date Coded: 11/29/2014
Date Approved:
Approved By:
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Inventory Item Report
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Inventory Item Report</h2>

    <table class="center">
        <tr class="rowHeader">
            <td class="tableData1">
                <b>ID:</b>
            </td>
            <td class="tableData1">
                <b>Item Desc:</b>
            </td>
            <td class="tableData1">
                <b>Color:</b>
            </td>
            <td class="tableData1">
                <b>Size :</b>
            </td>
            <td class="tableData1">
                <b>InvPric :</b>
            </td>
            
            <td class="tableData1">
                <b>QOH :</b>
            </td>
            <td class="tableData1">
                <b>Category :</b>
            </td>
        </tr>
        <% foreach (var inv in (List<TEAM2OSCS.Models.CartItem>)ViewData["Inventory"])
           { %>
               <tr>
                   <td class="tableData1">
                       <%= inv.invID%>
                   </td>
                   <td class="tableData1">
                       <%= inv.itemDesc%>
                   </td>
                   <td class="tableData1">
                       <%= inv.color%>
                   </td>
                   <td class="tableData1">
                       <%= inv.size%>
                   </td>
                   <td class="tableData1">
                       <%= inv.invPrice%>
                   </td>
                   <td class="tableData1">
                       <%= inv.amount%>
                   </td>
                   <td class="tableData1">
                       <%= inv.category%>
                   </td>
               </tr>
               <%
           } %>
    </table>

</asp:Content>
