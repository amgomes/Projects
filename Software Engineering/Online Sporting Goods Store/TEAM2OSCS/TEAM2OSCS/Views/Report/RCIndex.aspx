﻿<%@ Page Title="" Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage<dynamic>" %>

<asp:Content ID="Content1" ContentPlaceHolderID="TitleContent" runat="server">
	Index
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="MainContent" runat="server">

    <h2>Welcome Receiving Clerk</h2>
    <p>Please choose from one of the following options:</p>
    <ul>
        <li><%=Html.ActionLink("IncomingShipmentsReport", "IncomingShipmentsReport", "ReceivingClerk")%></li>
        <li><%=Html.ActionLink("OutstandingShipmentsReport", "OutstandingShipmentsReport", "ReceivingClerk")%></li>
        <li><%=Html.ActionLink("updatesInventoryforShipment", "updatesInventoryforShipment", "ReceivingClerk")%></li>
    </ul>

</asp:Content>