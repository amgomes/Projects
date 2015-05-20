<%-- 
Name: Men.aspx
Description: This view displays all of the items for category = 3 ("Men's Clothing").
Programmer: Adam Gomes
Date Coded: 11/29/2014
Date Approved:
Approved By:
--%>

<%@ Page Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage" %>

<asp:Content ID="menTitle" ContentPlaceHolderID="TitleContent" runat="server">
    Men
</asp:Content>

<asp:Content ID="MenContent" ContentPlaceHolderID="MainContent" runat="server">
    <h2>Men</h2>

        <div class="categoryImage">
            <img id="Img1" src="~/Images/parka.jpg" alt="Mens Parks" runat="server" height="220" width="220"/>
            <p id="center">Parka</p>
        </div>
     <br />
     <br />
     <br />
     <br />
     <br />
     <div class="inventory">
     <table class="center">
        <tr class="rowHeader">
            <td class="tableData1">
                <b>Inv ID:</b>
            </td>
            <td class="tableData1">
                <b>Description:</b>
            </td>
            <td class="tableData1">
                <b>Color:</b>
            </td>
            <td class="tableData1">
                <b>Size:</b>
            </td>
            <td class="tableData1">
                <b>Price:</b>
            </td>
            <td class="tableData1">
                <b>Quantity:</b>
            </td>
        </tr>
             <% foreach (var inv in (List<TEAM2OSCS.Models.Inventory>)ViewData["Inventory"])
           { %>
               <tr>
                   <td class="tableData1">
                       <%= inv.invID%>
                   </td>
                   <td class="tableData1">
                       <%= inv.itemDesc%>
                   </td>
                   <td class="tableData1">
                       <%= inv.color%>
                   </td>
                   <td class="tableData1">
                       <%= inv.size%>
                   </td>
                   <td class="tableData1">
                       <%= inv.invPrice.ToString("C2")%>
                   </td>
                   <td class="tableData1">
                       <%= inv.qoh%>
                   </td>
               </tr>
               <%
           } %>
    </table>    
    </div>

    <p>
    <!-- Lime Talk Live Chat -->
    <script type="text/javascript">
        var limetalk = (function () {
            var lc = document.createElement("script"); lc.type = "text/javascript";
            lc.async = true; lc.src = "//www.limetalk.com/livechat/ca688c7255ce748eb08b56869cabb54e";
            document.getElementsByTagName("head")[0].appendChild(lc); var fnr = function (fn) {
                var l = limetalk;
                if (l.readyList) { l.ready(fn); }
                else { l.rl = l.rl || []; l.rl.push(fn); }
            }; fnr.ready = fnr; return fnr;
        })(); 
     </script> 
     <!-- Lime Talk Live Chat END -->
    </p>
</asp:Content>
