<%@ Page Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage" %>

<asp:Content ID="aboutTitle" ContentPlaceHolderID="TitleContent" runat="server">
    About Us
</asp:Content>

<asp:Content ID="aboutContent" ContentPlaceHolderID="MainContent" runat="server">
    <h2>Meet the Team </h2>
    <p id="center">Thank for you viewing our Online Sporting Good Store. <br /> Our team has spent countless hours working to provide our customers with the highest level
        quality and care and we hope it shows!</p>
    <div id="teamMain">
        <div class="team">
            <img id="Img1" src="~/Images/vincent.png" alt="Vincent Diaz" runat="server" height="220" width="220"/>
            <p>Vincent Diaz <br /> Team Leader </p>
        </div>
        <div class="team">
            <img id="Img2" src="~/Images/jasmine.png" alt="Jasmine Wiggins" runat="server" height="220" width="220"/>
            <p>Jasmine Wiggins <br /> Software Quality Analyst</p>
        </div>
        <div class="team">
            <img id="Img3" src="~/Images/adam.png" alt="Adam Gomes" runat="server" height="220" width="220"/>
            <p>Adam Gomes <br /> Software Quality Analyst</p>
        </div>
        <div class="team">
            <img id="Img4" src="~/Images/stefan.png" alt="Stefan Theard" runat="server" height="220" width="220"/>
            <p>Stefan Theard <br /> Database Administrator</p>
        </div>
        <div class="team">
            <img id="Img5" src="~/Images/default.png" alt="Samuel Huang" runat="server" height="220" width="220"/>
            <p>Samuel Huang <br /> Database Administrator</p>
        </div>
        <div class="team">
            <img id="Img6" src="~/Images/erick.png" alt="Erick Saucedo" runat="server" height="220" width="220"/>
            <p>Erick Saucedo <br /> Team Member</p>
        </div>
    </div>
    <p>

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
