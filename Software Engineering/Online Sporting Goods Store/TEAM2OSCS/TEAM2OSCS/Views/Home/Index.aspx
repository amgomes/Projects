<%@ Page Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage" %>

<asp:Content ID="indexContent1" ContentPlaceHolderID="TitleContent" runat="server">
    Home Page
</asp:Content>

<asp:Content ID="indexContent2" ContentPlaceHolderID="MainContent" runat="server">
    <h2>Welcome To <%:ViewData["Message"] %></h2>
    <h3>The top supplier of high quality sporting goods</h3>

    <div class="teamMain">
            <img id="Img1" src="~/Images/jj.jpg" alt="Vincent Diaz" runat="server" height="600" width="1000"/>
    </div>
 
 <!--Start of Tawk.to Script-->
<script type="text/javascript">
    var $_Tawk_API = {}, $_Tawk_LoadStart = new Date();
    (function () {
        var s1 = document.createElement("script"), s0 = document.getElementsByTagName("script")[0];
        s1.async = true;
        s1.src = 'https://embed.tawk.to/547b73d6eebdcbe357960a62/default';
        s1.charset = 'UTF-8';
        s1.setAttribute('crossorigin', '*');
        s0.parentNode.insertBefore(s1, s0);
    })();
</script>
<!--End of Tawk.to Script-->
    </p>
    
</asp:Content>
