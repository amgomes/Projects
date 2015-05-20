<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Updates Inventory for Shipment
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Updates Inventory for Shipment</h2>
    <% using (Html.BeginForm("updatesInventoryforShipment", "ReceivingClerk")){ %>
         ShipmentID <%= Html.TextBox("ID") %> <br />
        <input type="submit" name="Search" value="Search" />
       <%} %>   
       <%: ViewData["Message"]%>
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>Inventory ID:</b>
            </td>
            <td class="tableData">
                <b>New Amount:</b>
            </td>
        </tr>
        <% foreach (var list in (List<TEAM2OSCS.Models.Inventory>)ViewData["Info"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= list.invID%>
                   </td>
                   <td class="tableData">
                       <%= list.qoh%>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
