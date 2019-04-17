# Ajax之xmlhttp.open()的用法 - 在思索中前行！ - CSDN博客





2014年12月01日 22:10:50[_Tham](https://me.csdn.net/txl16211)阅读数：997







1  问题描述：
xmlhttp:open方法，请求页面的时候，更新页面数据后，第2次拿到的结果还是上次的信息
2  解决办法：
改用POST方式
3  说明：
xmlhttp:open方法
创建一个新的http请求，并指定此请求的方法、URL以及验证信息
语法
oXMLHttpRequest.open(bstrMethod, bstrUrl, varAsync, bstrUser, bstrPassword);
参数
bstrMethod
http方法，例如：POST、GET、PUT及PROPFIND。大小写不敏感。
/*****

POST：用"POST"方式发送数据,可以大到4MB

GET：用"GET"方式发送数据,只能256KB

如果请求带有参数的化实用POST方式，POST方式将参数放置在页面的隐藏控件内

没有参数使用GET方式

对于请求的页面在中途可能发生更改的，也最好用POST方式

用GET方式可能会拿不到最新的信息

*****/
bstrUrl
请求的URL地址，可以为绝对地址也可以为相对地址。
varAsync[可选]
布尔型，指定此请求是否为异步方式，默认为true。如果为真，当状态改变时会调用onreadystatechange属性指定的回调函数。
bstrUser[可选]
如果服务器需要验证，此处指定用户名，如果未指定，当服务器需要验证时，会弹出验证窗口。
bstrPassword[可选]
验证信息中的密码部分，如果用户名为空，则此值将被忽略。

Example
下面的例子演示从服务器请求book.xml,并显示其中的book字段。
var xmlhttp = new ActiveXObject("Msxml2.XMLHTTP.3.0");
xmlhttp.open("GET","http://localhost/books.xml", false);  没有参数且不会发生改变的用GET，否则用POST
xmlhttp.send();
var book = xmlhttp.responseXML.selectSingleNode("//book[@id=''''bk101'''']");
alert(book.xml);
备注

调用此方法后，可以调用send方法向服务器发送数据。



```
调用页面 
<script language="javascript" type="text/javascript"> 
function UpdateData() 
{ 
var ServerUrl = window.location.href; 
var str = ServerUrl.split("/"); 
var ServerUrl = "http://"+str[2]; 
var xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); 
var serverURL=ServerUrl+"/GetTemp.aspx"; 
xmlhttp.Open("POST",serverURL, false); 
xmlhttp.Send("adst"); 
xmlhttp = null; 
} 
</script> 
处理页面，得到传入的数据 
private string GetInput() 
{ 
System.IO.Stream s = Request.InputStream; 
int count = 0; 
byte[] buffer = new byte[1024]; 
StringBuilder builder = new StringBuilder(); 
while ((count = s.Read(buffer, 0, 1024)) > 0) 
{ 
builder.Append(Encoding.UTF8.GetString(buffer, 0, count)); 
} 
return builder.ToString(); 
}
```






