# RSS你会用了吗？答曰：不会 - weixin_33985507的博客 - CSDN博客
2007年03月11日 22:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
在查关于RBAC权限管理设计的时候看到了RSS的相关内容，自己也经常用RSS，可是猛的一问，RSS你会用了吗？![11.gif](https://www.cnblogs.com/Emoticons/QQ/11.gif)答曰：不会。
如果想了解这方面的资料的话，大家可以参考一下如下文章（（作者：[李天平](http://blog.csdn.net/litp)转载请注明））：
[Web2.0时代，RSS你会用了吗？(技术实现总结)](http://www.cnblogs.com/ltp/archive/2006/01/15/317725.html)[出自：http://www.cnblogs.com/ltp/archive/2006/01/15/317725.html](http://www.cnblogs.com/ltp/archive/2006/01/15/317725.html)
     如见Web2.0被吵的如火如荼，同时也有很多的评论褒贬不一。有人说Web2.0就是“一帮人在看皇帝的新装而已”。也有人说Web2.0将是一场变革。无论这些争论有没有价值，但我相信，存在就有它的必然性。也许Web2.0更像是一种商业模式，一个观念的变革而非技术。
     不管怎样，它来了，你准备好了吗？
     说到Web2.0，就会提到Blog、TAG、SNS、RSS、wiki这些软件应用和xml、ajax等这些新理论和技术实现。
RSS毋容置疑就是其重要的应用之一。
     那么什么是RSS？它到底有什么用呢？
     RSS是站点与站点之间共享内容的一种简易方式(也称为“聚合内容”)，通常被用于新闻和其他按顺序排列的网站，例如Blog网站。网站提供RSS输出，有利于让用户发现网站内容的更新。网站用户可以在客户端借助于类似新闻资讯阅读器等支持RSS的新闻聚合工具软件，在不打开网站内容页面的情况下阅读支持RSS输出的网站内容。
     RSS是基于XML的一种形式。并且所有的RSS文件都要遵守万维网联盟(W3C)站点发布的XML 1.0规范。具体格式可以查一下RSS 2.0规范，这里就不再重复粘贴了。
下面是我总结的几种RSS的技术实现代码：
**1.在线生成RSS聚合页**。
(1)创建Rss.aspx
<%@ Page language="c#" Codebehind="Rss.aspx.cs" AutoEventWireup="false" Inherits="LiTianPing.Rss" %>
只留下这一行，其余的都删掉。
(2)后台代码；Rss.aspx.cs
  private void Page_Load(object sender, System.EventArgs e)
  {    
   Response.ContentType="text/xml";
   Response.Write(GetRSS());
  }
  /// <summary>
  /// 取得聚合文章
  /// </summary>
  /// <returns></returns>
  public string GetRSS()
  {
   News t=new News();//自己的业务类
   DataSet ds=t.GetListByClass(1);//根据类别得到数据
   StringBuilder strCode=new StringBuilder();
   strCode.Append("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>");
   strCode.Append("<rss version='2.0' xmlns:dc=\"[http://purl.org/dc/elements/1.1/\](http://purl.org/dc/elements/1.1//)"");
   strCode.Append(" xmlns:trackback=\"[http://madskills.com/public/xml/rss/module/trackback/\](http://madskills.com/public/xml/rss/module/trackback//)" ");
   strCode.Append(" xmlns:wfw=\"[http://wellformedweb.org/CommentAPI/\](http://wellformedweb.org/CommentAPI//)" xmlns:slash=\"[http://purl.org/rss/1.0/modules/slash/\](http://purl.org/rss/1.0/modules/slash//)">");
   strCode.Append("<channel>");
   strCode.Append("<title>李天平RSSDemo</title>");
   strCode.Append("<link>http://"+Request.ServerVariables["SERVER_NAME"]+"</link> ");
   strCode.Append("<description>天道酬勤</description> ");  
   strCode.Append("<copyright>Copyright 2005</copyright> ");      
   foreach(DataRow row in ds.Tables[0].Rows)
   {
    string Id=row["Id"].ToString();
    string title=row["title"].ToString();
    string description=row["description"].ToString();
    string pubdate=row["pubdate"].ToString();
    string ClassId=row["ClassId"].ToString();
    //string author=row["author"].ToString();
    strCode.Append("<item>");
    strCode.Append("<title>"+title+"</title>");
    strCode.Append("<link>http://"+Request.ServerVariables["SERVER_NAME"]+"/NewsShow.aspx?ID="+Id+"</link>");
    strCode.Append("<subject>"+description+"</subject>");
    strCode.Append("<description><![CDATA["+description+"]]></description>");    
    strCode.Append("<PubDate>"+pubdate+"</PubDate>");
    strCode.Append("<category>"+ClassId+"</category>");
    strCode.Append("</item>");    
   }
   strCode.Append("</channel>");
   strCode.Append("</rss>");  
   return strCode.ToString();
  }
 (3) **XmlTextWriter实现方式2**；Rss.aspx.cs
  string xmlDoc="rss.xml"; 
 private void Page_Load(object sender, System.EventArgs e)
  {    
   xmlDoc=Server.MapPath(".")+xmlDoc;   
   GetRSS2();
   XmlDocument doc= new XmlDocument();
   doc.Load(xmlDoc);
   Response.ContentType = "text/xml";
   doc.Save(Response.Output);
  }
  /// <summary>
  /// 取得聚合文章
  /// </summary>
  /// <returns></returns>
  public void GetRSS2()
  {
   News t=new News();
   DataSet ds=t.GetListByClass(1);
   XmlTextWriter writer = new XmlTextWriter(xmlDoc,Encoding.UTF8);
   writer.Formatting = Formatting.Indented;
   writer.WriteStartDocument(true);
   writer.WriteComment("RSS页的实现");
   writer.WriteStartElement("rss");
   writer.WriteAttributeString("version","2.0");      
   writer.WriteStartElement("channel");   
   writer.WriteStartElement("title");
   writer.WriteString("李天平RSSDemo");
   writer.WriteEndElement();
   writer.WriteStartElement("link");   
   writer.WriteString("[http://"+Request.ServerVariables["SERVER_NAME](/)"]);
   writer.WriteEndElement();   
   writer.WriteStartElement("description");
   writer.WriteString("天道酬勤");
   writer.WriteEndElement();
   writer.WriteStartElement("copyright");
   writer.WriteString("Copyright 2005");
   writer.WriteEndElement();
   writer.WriteStartElement("language");
   writer.WriteString("zh-cn");
   writer.WriteEndElement();  
   foreach(DataRow row in ds.Tables[0].Rows)
   {
    string Id=row["Id"].ToString();
    string title=row["title"].ToString();
    string description=row["description"].ToString();
    string pubdate=row["pubdate"].ToString();
    string ClassId=row["ClassId"].ToString();
    //string author=row["author"].ToString();
    writer.WriteStartElement("item");
    writer.WriteStartElement("title");
    writer.WriteString(title);
    writer.WriteEndElement();
    writer.WriteStartElement("link");
    writer.WriteString("[http://"+Request.ServerVariables["SERVER_NAME"]+"/NewsShow.aspx?ID="+Id](/NewsShow.aspx?ID=%22+Id)) ;
    writer.WriteEndElement();
    writer.WriteStartElement("description");
    writer.WriteCData(description);
    writer.WriteEndElement();
    writer.WriteStartElement("pubDate");
    writer.WriteString(pubdate);
    writer.WriteEndElement();
    writer.WriteStartElement("category");
    writer.WriteString(ClassId);
    writer.WriteEndElement();
    writer.WriteEndElement();  
   }
   writer.WriteEndElement();
   writer.WriteEndElement();
   writer.Flush();
   writer.Close();
  }
**2.在线RSS阅读获取。**
页面代码：
  <P style="FONT-SIZE: 11px">
   Feed种子：<asp:TextBox id="FeedUrl" runat="server" Width="440px"></asp:TextBox><BR>
   显示条数：<asp:TextBox id="Num" runat="server" Width="48px"></asp:TextBox></P>
  <P style="FONT-SIZE: 11px">
   <asp:Button id="GetFeed" runat="server" Text="获得RSS"></asp:Button><BR>
   <BR>
   <asp:Label id="RssFeed" runat="server"></asp:Label></P>
后台代码：
  /// <summary>
  /// 获得要显示的Feed种子数据
  /// </summary>
  /// <param name="RssUrl"></param>
  /// <param name="showNewsCount"></param>
  /// <returns></returns>
  public string LoadRSS(string RssUrl, int showNewsCount)
  { 
   string strRssList = "";
   string strMsg;
   try
   {      
    XmlDocument objXMLDoc = new XmlDocument();    
    objXMLDoc.Load(RssUrl);   
    XmlNodeList objItems = objXMLDoc.GetElementsByTagName("item");    
    if(showNewsCount > 30)
     showNewsCount = 10;   
    if(showNewsCount < 1)
     showNewsCount = objItems.Count;
    string title="";
    string link="";  
    int i;
    if(objXMLDoc.HasChildNodes == true)
    {
     i = 1;
        foreach(XmlNode objNode in objItems)
     {
      if(i<=showNewsCount)
      {
       if(objNode.HasChildNodes == true)
       {
        XmlNodeList objItemsChild = objNode.ChildNodes;
        foreach(XmlNode objNodeChild in objItemsChild)
        {
         switch(objNodeChild.Name)
         {
          case "title":
           title = objNodeChild.InnerText;
           break;
          case "link":
           link = objNodeChild.InnerText;
           break;          
         }         
        } 
        i = i+1;
        strRssList += "<a href=" + link +" target=_blank>" + title + "</a><br>";  
       }       
      }
     }        
    }
    strMsg = strRssList;
   }
   catch
   {
    strMsg = "RSS Feed 源数据出错！";
   }
   return strMsg;
  }
  //获取按钮,获取指定RSS
  private void GetFeed_Click(object sender, System.EventArgs e)
  {
   if(FeedUrl.Text == "")//RSS地址
   {
    RssFeed.Text = "信息源不能为空，您可刷新重试或联系管理员！";
    return ;
   }  
   RssFeed.Text = LoadRSS(FeedUrl.Text,Convert.ToInt32(Num.Text)); //获取指定数目  
  }
**3.无刷新动态更新的在线RSS阅读获取**
<%@ Page language="c#" Codebehind="XmlHttp.aspx.cs" AutoEventWireup="false" Inherits="LiTianPing.XmlHttp" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
 <HEAD>
  <title>XmlHttp</title>
  <meta name="GENERATOR" Content="Microsoft Visual Studio .NET 7.1">
  <meta name="CODE_LANGUAGE" Content="C#">
  <meta name="vs_defaultClientScript" content="JavaScript">
  <meta name="vs_targetSchema" content="[http://schemas.microsoft.com/intellisense/ie5](http://schemas.microsoft.com/intellisense/ie5)">
  <style>td { font-size: 9pt}
  </style>
  <script>
  <!-- 
  var oDiv 
  var xh     
  function getXML()
  {   
   xh =new ActiveXObject("Microsoft.XMLHTTP"); 
   oDiv = document.getElementById("rssitem");
   oDiv.style.display= "";
   try
   {
    //如果是本地改成下面语句直接读取
    //xh.open("GET","/Rss.aspx",false);    
    xh.open("GET","[Http://ltp.cnblogs.com/Rss.aspx",false](http://www.cnblogs.com/Rss.aspx%22,false));
    xh.send(null);
    loadData(xh,oDiv);
   }
   catch(e)
   {
    error(oDiv);
   }
  }
  function loadData(xh,oDiv)
  {
   if (xh.readyState == 4)
   {
    xml = xh.responsexml;
    var i;
    var nodes = xml.selectNodes("/rss/channel/item");
    var bloglink = xml.selectSingleNode("/rss/channel/link").text;
    oDiv.innerHTML = "";
    var html;    
    for(i=0;i<nodes.length;i++)
    {    
      html += "<div >";
      html += "    <div >";
      html += "        <a class=font1 href='" +nodes[i].selectSingleNode("link").text + "' target='_blank'>" + nodes[i].selectSingleNode("title").text +"</a><BR><BR>";
      html += "    </div>";
      html += "    <div >";
      html += "        " +nodes[i].selectSingleNode("description").text;
      html += "    </div>"            
      //html += "    </div>";
      html += "</div><hr>";
    }
    oDiv.innerHTML = html;
   }
   else
   {
    error(oDiv);
   }
  }
  function error(oDiv)
  {
   oDiv.innerHTML = "载入失败";
  }
  -->
  </script>
 </HEAD>
 <body MS_POSITIONING="GridLayout" onload="window.setTimeout('getXML()',200);">
  <form id="Form1" method="post" runat="server">
   <div align=center>
   <table cellpadding="0" cellspacing="0" border="0" width="80%">
    <tr>
     <td>
      <div id="rssitem" style="WIDTH:80%">数据载入中...</div>
     </td>
    </tr>
   </table>
   </div>
  </form>
 </body>
</HTML> 
