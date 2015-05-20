<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Search Testimonial
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Remove from Cart</h2>
    <% using (Html.BeginForm("RemoveFromCart", "Customer")){ %>
        ID <%= Html.TextBox("Word") %> <br />
        <input type="submit" name="Remove" value="Remove" />
       <%} %>   
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData1">
                <b>ID:</b>
            </td>
            <td class="tableData1">
                <b>item Desc:</b>
            </td>
            <td class="tableData1">
                <b>color:</b>
            </td>
            <td class="tableData1">
                <b>size :</b>
            </td>
            <td class="tableData1">
                <b>invPric :</b>
            </td>
            
            <td class="tableData1">
                <b>amount  :</b>
            </td>
            <td class="tableData1">
                <b>category :</b>
            </td>
        </tr>
        <% foreach (var list in (List<TEAM2OSCS.Models.CartItem>)ViewData["cart"])
           { %>
               <tr>
                   <td class="tableData1">
                       <%= list.invID%>
                   </td>
                   <td class="tableData1">
                       <%= list.itemDesc%>
                   </td>
                   <td class="tableData1">
                       <%= list.color%>
                   </td>
                   <td class="tableData1">
                       <%= list.size%>
                   </td>
                   <td class="tableData1">
                       <%= list.invPrice%>
                   </td>
                   <td class="tableData1">
                       <%= list.amount%>
                   </td>
                   <td class="tableData1">
                       <%= list.category%>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
