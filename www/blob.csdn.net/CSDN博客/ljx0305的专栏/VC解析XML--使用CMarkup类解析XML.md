# VC解析XML--使用CMarkup类解析XML - ljx0305的专栏 - CSDN博客
2008年05月28日 11:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：3643
**VC解析XML--使用CMarkup类解析XML。**![](http://himg.baidu.com/sys/portrait/item/28f579786966756e00.jpg)**--yxifu**
(一) 先讲一下XML中的物殊字符，手动填写时注意一下。![](http://icon.cnzz.com/icon.gif)
字符                   字符实体
&                       &或&#38;
'                       '或'
>                       >或>
<                       <或&<
"                        "或"
(二) CMarkup类的源代码。
这是目前的最新版本；
这是官网示例文件，取出里面的Markup.cpp和Markup.h，导入你的工程里面，CMarkup类就可以用了；
下载地址：[http://www.firstobject.com/Markup90.zip](http://www.firstobject.com/Markup90.zip)　2007年10月1日官网最新版
如果下载不了，可以到官网去下载　[http://www.firstobject.com/dn_markup.htm](http://www.firstobject.com/dn_markup.htm)
(三) 创建一个XML文档。
CMarkup xml;
xml.AddElem( "ORDER" );
xml.AddChildElem( "ITEM" );
xml.IntoElem();
xml.AddChildElem( "SN", "132487A-J" );
xml.AddChildElem( "NAME", "crank casing" );
xml.AddChildElem( "QTY", "1" );
xml.Save("c://UserInfo.xml");
效果如下：
CMarkup xml;
 ( xml.FindChildElem("ITEM") )
{
      xml.IntoElem();
      xml.FindChildElem( "SN" );
      CString csSN = xml.GetChildData();
      xml.FindChildElem( "QTY" );
     nQty = atoi( xml.GetChildData() );
      xml.OutOfElem();
}添加在最后面,使用的是AddElem;添加在最前面,使用InsertElem。CMarkup xml;xml.Load("c://.xml");
xml.AddElem( "ORDER" );
xml.IntoElem(); 
      xml.AddElem( "ITEM" );
      xml.IntoElem(); 
      xml.AddElem( "SN", "**4238764-A"** ); //添加元素
      xml.AddElem( "NAME", "**bearing"** );//添加元素
      xml.AddElem( "QTY", "**15"** );//添加元素
      xml.OutOfElem(); xml.AddElem( "SHIPMENT" );
xml.IntoElem(); 
xml.AddElem( "POC" );//添加元素
xml.SetAttrib( "type", "**non-emergency**");//添加属性
xml.IntoElem(); 
xml.AddElem( "NAME", "**John Smith**");//添加元素
xml.AddElem( "TEL", "555-1234");//添加元素
xml.Save("c://.xml");
效果如下：
(六) 修改元素和属性
如将POC中的属性type改成：change;
元素TEL改成：123456789
        CMarkup xml;
if (xml.Load("UserInfo.xml"))
{
   CString strUserID = _T("");
   xml.ResetMainPos();
   if (xml.FindChildElem("SHIPMENT"))
   {
    xml.IntoElem();
    if (xml.FindChildElem("POC"))
    {
     xml.IntoElem();
     CString str_type=xml.GetAttrib("type");
     MessageBox(str_type);
     xml.SetAttrib("type","change");
     strUserID = xml.GetData();
     if (xml.FindChildElem("TEL"))
     {
      xml.IntoElem();
      xml.SetData("123456789");
      xml.Save("UserInfo.xml");
      return;
     }
    }
   }
}
(七)删除元素：
删除SN=132487A-J的项目。
CMarkup xml;
if (xml.Load("UserInfo.xml"))
{
   CString strUserID = _T("");
   xml.ResetMainPos();
   if (xml.FindChildElem("ITEM"))
   {
    xml.IntoElem();
    CString str_sn;
    xml.FindChildElem("SN");
    str_sn=xml.GetChildData();
    if(str_sn=="132487A-J")
    {
     xml.RemoveElem();
     xml.Save("UserInfo.xml");
    }
   }
}
转自：[http://hi.baidu.com/yxifu/blog/item/fa1569225bda52a44623e8f0.html](http://hi.baidu.com/yxifu/blog/item/fa1569225bda52a44623e8f0.html)
