# ajaxpro.2.dll 简单应用 - weixin_33985507的博客 - CSDN博客
2007年06月26日 09:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
1.添加 ajaxpro.2.dll 引用
2.web.config 
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<system.web>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<httpHandlers>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<add verb="*" path="*.ashx" type="AjaxPro.AjaxHandlerFactory,AjaxPro.2"/>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</httpHandlers>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</system.web>
3.服务器端代码
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Data;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Configuration;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Collections;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web.Security;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web.UI;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web.UI.WebControls;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web.UI.WebControls.WebParts;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Web.UI.HtmlControls;
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)public partial class Ajax : System.Web.UI.Page
![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)protectedvoid Page_Load(object sender, EventArgs e)
![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        AjaxPro.Utility.RegisterTypeForAjax(typeof(Ajax));
![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)    [AjaxPro.AjaxMethod]
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicstring GetServerTimeString()
![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)        System.Threading.Thread.Sleep(2000);
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return DateTime.Now.ToString();
![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
4.客户端代码
![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)<%![dot.gif](https://www.cnblogs.com/Images/dot.gif)@ Page Language="C#" AutoEventWireup="true" CodeFile="Ajax.aspx.cs" Inherits="Ajax"%>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<html xmlns="http://www.w3.org/1999/xhtml">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<head runat="server">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<title>ajaxpro.2.dll 简单应用</title>
![ExpandedBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockStart.gif)![ContractedBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedBlock.gif)<script language="javascript" type="text/javascript">![dot.gif](https://www.cnblogs.com/Images/dot.gif)
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)function ajax_callback(response)
![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){             
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)             document.getElementById("div1").innerHTML = response.value;
![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)function getServerTime()
![ExpandedSubBlockStart.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![ContractedSubBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![dot.gif](https://www.cnblogs.com/Images/dot.gif){
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)          document.getElementById("div1").innerHTML ="请稍后，正在获取服务器时间..";
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)          Ajax.GetServerTimeString(ajax_callback);
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![ExpandedSubBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)       }
![InBlock.gif](https://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![ExpandedBlockEnd.gif](https://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)</script>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</head>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<body>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<form id="form1" runat="server">
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<div>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<div id="div1">点击按钮获取服务器端时间</div>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)<input type="button" value=" click " onclick="javascript:getServerTime();"/>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</div>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</form>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</body>
![None.gif](https://www.cnblogs.com/Images/OutliningIndicators/None.gif)</html>
