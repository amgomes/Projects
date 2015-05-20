<%-- 
Name: DiscountsReport.aspx
Description: This view displays the Discounts report to the Marketing Manager. It is called by the MarketingMangerController.
             It gets the total sales price for each order and calculates the total discount based on the following rules:
                    - If sales price for the order is greater than $200, apply a 20% discount
                    - If sales price for the order is greater than $100, apply a 10% discount
                    - Else no discount is applied
Programmer: Adam Gomes
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Jasmine Wiggins
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	DiscountsReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Discounts Report</h2>
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Order ID:</b>
            </td>
            <td class="tableData">
                <b>Original Amount:</b>
            </td>
            <td class="tableData">
                <b>Discounted Amount:</b>
            </td>
        </tr>
        <% foreach (var order in (List<TEAM2OSCS.Models.Order>) ViewData["Orders"])
           { %>
               <tr>
                   <td class="tableData">
                       <% = order.orderID %>
                   </td>
                   <td class="tableData">
                       <% = order.originalTotal.ToString("C2")%>
                   </td>
                   <td class="tableData">
                       <% if (order.discountedTotal == -1)
                          {%>
                           <% = "N/A" %>
                       <% } else {%>
                           <% = order.discountedTotal.ToString("C2") %>
                       <% } %>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>