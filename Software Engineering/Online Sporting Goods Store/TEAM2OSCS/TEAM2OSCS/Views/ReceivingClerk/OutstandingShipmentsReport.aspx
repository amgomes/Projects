<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Outstanding Shipments Report
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Outstanding Shipments Report</h2>

    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>shipID :</b>
            </td>
            <td class="tableData">
                <b>shipDateExpected:</b>
            </td>
            <td class="tableData">
                <b>InventoryID:</b>
            </td>
        </tr>
        <% foreach (var list in (List<TEAM2OSCS.Models.Shipment>)ViewData["Info"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= list.shipID%>
                   </td>
                   <td class="tableData">
                       <%= list.shipDateExpected%>
                   </td>
                   <td class="tableData">
                       <%= list.Inv_id%>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
