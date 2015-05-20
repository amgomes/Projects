<%-- 
Name: TrackOrdersSource.aspx
Description: This view displays the Track Orders Source report to the Marketing Manager. It is called by the MarketingMangerController.
             It matches each order placed with its order source and displays it.
Programmer: Adam Gomes
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Jasmine Wiggins
--%>


<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	TrackOrdersSource
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Track Orders Source</h2>

    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Order ID:</b>
            </td>
            <td class="tableData">
                <b>Order Source ID:</b>
            </td>
            <td class="tableData">
                <b>Order Source Description:</b>
            </td>
        </tr>
        <% foreach (var order in (List<TEAM2OSCS.Models.Order>)ViewData["Orders"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= order.orderID %>
                   </td>
                   <td class="tableData">
                       <%= order.osID %>
                   </td>
                   <td class="tableData">
                       <%= order.osDesc %>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
