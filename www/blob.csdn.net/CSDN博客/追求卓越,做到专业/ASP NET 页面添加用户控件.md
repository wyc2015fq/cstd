
# ASP.NET 页面添加用户控件 - 追求卓越,做到专业 - CSDN博客


2012年10月29日 15:13:25[Waldenz](https://me.csdn.net/enter89)阅读数：495


1.在本页面上注册<%@ Register src="Controls/GoHome.ascx" tagname="GoHome" tagprefix="ucGH" %>
使用时:<ucGH:GoHome ID="GoHome1" runat="server" />
2.记录在web.config文件中,记录在以下节点中: configuration/system.web/pages/controls
语句为:<add src="~/Controls/GoHome.ascx" tagName="GoHome" tagPrefix="ucGH"/>
使用时:<ucGH:GoHome ID="GoHome1" runat="server" />





