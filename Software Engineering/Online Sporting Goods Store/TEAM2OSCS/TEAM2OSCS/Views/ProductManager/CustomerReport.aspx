<%-- 
Name: CustomerReport.aspx
Description: This view displays the following information for each customer in the database
    -ID
    -First Name
    -Last Name
    -Email Address
    -Username
    Since this is to be used by the product manager, I am under the assumption that shipping
    information is unnecessary.
Programmer: Erick Saucedo
Date Coded: 11/29/2014
Date Approved: 12/1/2014
Approved By: Adam Gomes
--%>

<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	CustomerReport
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Customer Report</h2>
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Cust ID:</b>
            </td>
            <td class="tableData">
                <b>Last:</b>
            </td>
            <td class="tableData">
                <b>First:</b>
            </td>
            <td class="tableData">
                <b>Email:</b>
            </td>
            <td class="tableData">
                <b>Username:</b>
            </td>

        </tr>
        <% foreach (var thing in (List<TEAM2OSCS.Models.Customer>)ViewData["Customers"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= thing.custID %>
                   </td>
                   <td class="tableData">
                       <%= thing.lastName %>
                   </td>
                   <td class="tableData">
                       <%= thing.firstName %>
                   </td>
                   <td class="tableData">
                       <%= thing.email %>
                   </td>
                   <td class="tableData">
                       <%= thing.username %>
                   </td>
                   
               </tr>
               <%
           } %>
    </table>

</asp:Content>
