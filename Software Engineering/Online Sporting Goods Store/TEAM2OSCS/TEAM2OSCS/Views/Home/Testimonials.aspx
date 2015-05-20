<%@ Page Language="C#" MasterPageFile="~/Views/Shared/Site.Master" Inherits="System.Web.Mvc.ViewPage" %>

<asp:Content ID="testimonialTitle" ContentPlaceHolderID="TitleContent" runat="server">
    Testimonials
</asp:Content>

<asp:Content ID="testimonialContent" ContentPlaceHolderID="MainContent" runat="server">
    <h2>Search For or Add a Testimonial:</h2>
<!DOCTYPE html>
    <head>
<!-- JAVASCRIPT to clear search text when the field is clicked -->
<script type="text/javascript">
    window.onload = function () {
        //Get submit button
        var submitbutton = document.getElementById("tfq");
        //Add listener to submit button
        if (submitbutton.addEventListener) {
            submitbutton.addEventListener("click", function () {
                if (submitbutton.value == 'Search Testimonials') {
                    submitbutton.value = '';
                }
            });
        }
    }
    </script>

    </head>
    <body>
	<!-- HTML for SEARCH BAR -->
	<div id="tfheader">
		<form id="tfnewsearch" runat="server">
		        <input type="text" id="tfq" class="tftextinput2" name="q" size="21" maxlength="120" value="Search Testimonials"><input type="submit" value=">" class="tfbutton2"><br />
                <asp:GridView ID="GridView1" runat="server" ShowFooter="true" AutoGenerateColumns="False"  
                    DataKeyNames="T_ID" DataSourceID="SqlDataSource1" 
                    EmptyDataText="There are no data records to display.">
                    <Columns>
                        <asp:BoundField DataField="T_ID" HeaderText="T_ID" ReadOnly="True" 
                            SortExpression="T_ID" />
                        <asp:BoundField DataField="CONTENT" HeaderText="CONTENT" 
                            SortExpression="CONTENT" />
                        <asp:BoundField DataField="T_DATE" HeaderText="T_DATE" 
                            SortExpression="T_DATE" />
                        <asp:BoundField DataField="C_ID" HeaderText="C_ID" SortExpression="C_ID" />
             
            <asp:TemplateField HeaderText="">
            <FooterStyle HorizontalAlign="Right" />
            <FooterTemplate>
             <asp:Button ID="ButtonAdd" runat="server" Text="Add New Testimonial" />
            </FooterTemplate>
             </asp:TemplateField>
                    </Columns>
   
                </asp:GridView>
                <asp:SqlDataSource ID="SqlDataSource1" runat="server" 
                    ConnectionString="<%$ ConnectionStrings:TEAM2OSCSConnectionString1 %>" 
                    DeleteCommand="DELETE FROM [TESTIMONIAL] WHERE [T_ID] = @T_ID" 
                    InsertCommand="INSERT INTO [TESTIMONIAL] ([T_ID], [CONTENT], [T_DATE], [C_ID]) VALUES (@T_ID, @CONTENT, @T_DATE, @C_ID)" 
                    ProviderName="<%$ ConnectionStrings:TEAM2OSCSConnectionString1.ProviderName %>" 
                    SelectCommand="SELECT [T_ID], [CONTENT], [T_DATE], [C_ID] FROM [TESTIMONIAL]" 
                    UpdateCommand="UPDATE [TESTIMONIAL] SET [CONTENT] = @CONTENT, [T_DATE] = @T_DATE, [C_ID] = @C_ID WHERE [T_ID] = @T_ID">
                    <DeleteParameters>
                        <asp:Parameter Name="T_ID" Type="Int32" />
                    </DeleteParameters>
                    <InsertParameters>
                        <asp:Parameter Name="T_ID" Type="Int32" />
                        <asp:Parameter Name="CONTENT" Type="String" />
                        <asp:Parameter Name="T_DATE" Type="String" />
                        <asp:Parameter Name="C_ID" Type="Int32" />
                    </InsertParameters>
                    <UpdateParameters>
                        <asp:Parameter Name="CONTENT" Type="String" />
                        <asp:Parameter Name="T_DATE" Type="String" />
                        <asp:Parameter Name="C_ID" Type="Int32" />
                        <asp:Parameter Name="T_ID" Type="Int32" />
                    </UpdateParameters>
                    
                </asp:SqlDataSource>
       </form>
		<div class="tfclear"></div>
	</div>

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