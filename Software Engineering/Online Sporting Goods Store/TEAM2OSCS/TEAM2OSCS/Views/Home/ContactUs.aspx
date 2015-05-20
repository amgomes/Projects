<%@ Page Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage" %>

<asp:Content ID="contactUsTitle" ContentPlaceHolderID="TitleContent" runat="server">
    Contact Us
</asp:Content>

<asp:Content ID="contactUsContent" ContentPlaceHolderID="MainContent" runat="server">
    <h2>CONTACT US</h2>
    <body>

    <table style="width:100%; text-align: center; background: linear-gradient(#ffffff, #D2D2D2);
    background: -moz-linear-gradient(#ffffff, #D2D2D2);
    background: -webkit-linear-gradient(#ffffff, #D2D2D2); /* For Safari 5.1 to 6.0 */
    background: -o-linear-gradient(#ffffff, #D2D2D2); /* For Opera 11.1 to 12.0 */
    background: linear-gradient(#ffffff, #D2D2D2); /* Standard syntax */" >
    <tr>
    <td><img id="Mail" src="~/Images/mail.png" alt="Mail Icon" runat="server" height="72" width="61"/><b>EMAIL US A QUESTION OR CONCERN</b></td>
    <td><img id="Chat" src="~/Images/chat.png" alt="Chat Icon" runat="server" height="72" width="61"/><b>CHAT LIVE WITH A TEAM2OSCS EXPERT</b></td>
    </tr>

    <tr>
    <td><p>support@TEAM2OSCS.com</p><p>We will get back to you within 24 hours</p></td>
    <td><p>Chat with a representative below</p><p>Mon-Fri 9am - 7pm</p></td>
    </tr>
    </table>

    </body>
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
