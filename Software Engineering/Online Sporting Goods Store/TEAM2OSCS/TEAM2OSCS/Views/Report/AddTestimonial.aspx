<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Add Testimonial
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Add Testimonial</h2>
    <% using (Html.BeginForm("addTestimonial", "Report")){ %>
        Input <%= Html.TextArea("T") %> <br />
        <input type="submit" name="Enter" value="Enter" />
   <%} %>   
   <%: ViewData["Message"]%>
</asp:Content>
