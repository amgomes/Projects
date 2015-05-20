<%-- 
Name: TotalSalesReport.aspx
Description: This view displays the Total Sales report to the Marketing Manager. It is called by the MarketingMangerController.
             It gets the total sales price for each order placed by the website (os_id = 6) and adds it all together to get the total.
Programmer: Adam Gomes
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Jasmine Wiggins
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	TotalSalesReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Total Sales Report</h2>

    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Total Website Sales:</b>
            </td>
            <td class="tableData">
                <% = ViewData["WebsiteSales"] %>
            </td>
        </tr>
    </table>

</asp:Content>
