<%-- 
Name: PMInventoryReport.aspx
Description: This view displays the key inventory statistics. Namely:
    -Inventory Id
    -Item ID
    -Price
    -Number in stock
Programmer: Erick Saucedo
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Adam Gomes
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	PMInventoryReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Inventory Report</h2>
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Inv ID:</b>
            </td>
            <td class="tableData">
                <b>Item ID:</b>
            </td>
            <td class="tableData">
                <b>Price:</b>
            </td>
            <td class="tableData">
                <b>QOH:</b>
            </td>
        </tr>
        <% foreach (var thing in (List<TEAM2OSCS.Models.Inventory>)ViewData["Inventory"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= thing.invID %>
                   </td>
                   <td class="tableData">
                       <%= thing.itemID %>
                   </td>
                   <td class="tableData">
                       <%= thing.invPrice.ToString("C2") %>
                   </td>
                   <td class="tableData">
                       <%= thing.qoh %>
                   </td>
               </tr>
               <%
           } %>
    </table>

</asp:Content>
