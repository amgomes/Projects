<%-- 
Name: DiscountsReport.aspx
Description: This view displays the overall cost for the items in the store
Programmer: Erick Saucedo
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Adam Gomes
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	TotalInvestmentReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Total Investment Report</h2>

    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Total Investment:</b>
            </td>
            <td class="tableData">
                <% = ViewData["InvestmentReport"] %>
            </td>
        </tr>
    </table>

</asp:Content>
