<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Search Testimonial
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Search Testimonial</h2>
    <% using (Html.BeginForm("SearchTestimonial", "Report")){ %>
        Find <%= Html.TextBox("searchWord") %> <br />
        <input type="submit" name="Search" value="Search" />
       <%} %>   
    <table class="center">
        <tr class="rowHeader">
            <td class="tableData">
                <b>T_ID:</b>
            </td>
            <td class="tableData">
                <b>Content:</b>
            </td>
            <td class="tableData">
                <b>T_Date:</b>
            </td>
            <td class="tableData">
                <b>C_ID:</b>
            </td>
        </tr>
        <% foreach (var order in (List<TEAM2OSCS.Models.Testimonial>)ViewData["Testimonials"])
           { %>
               <tr>
                   <td class="tableData">
                       <%= order.testID%>
                   </td>
                   <td class="tableData">
                       <%= order.content %>
                   </td>
                   <td class="tableData">
                       <%= order.testDate %>
                   </td>
                   <td class="tableData">
                       <%= order.custID%>
                   </td>
               </tr>
               <%
           } %>
    </table>
</asp:Content>
