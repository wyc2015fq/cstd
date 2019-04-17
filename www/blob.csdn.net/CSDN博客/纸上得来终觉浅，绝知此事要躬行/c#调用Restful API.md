# c#调用Restful API - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月09日 10:57:53[boonya](https://me.csdn.net/boonya)阅读数：7146








*原文*[http://www.makeyuan.com/2014/02/27/1117.html](http://www.makeyuan.com/2014/02/27/1117.html?utm_source=tuicool)






**一、概述**

System.Net命名空间包含了 **HttpWebRequest** 和 **HttpWebResponse** 类的获取从Web服务器和基于HTTP的Web服务的数据。通常你还需要添加一个引用System.Web，这将给你访问HttpUtility类，它提供的方法对HTML和URL编码和解码的文本字符串。 虽然一些网络服务也可以返回其他格式的数据，如JSON和序列化的PHP，但最容易利用的还是XML，因为在.NET Framework广泛支持这种格式的读取和操作。


**二、获取数据**

1、以Get方式获取
using System;
using System.IO;
using System.Net;
using System.Text;

// Create the web request
HttpWebRequest request = WebRequest.Create("http://developer.yahoo.com/") as HttpWebRequest;

// Get responseusing (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
{
// Get the response stream
StreamReader reader = new StreamReader(response.GetResponseStream());

// Console application output
Console.WriteLine(reader.ReadToEnd());
}
2、以Post方式获取
using System.Web;

Uri address = new Uri("http://api.search.yahoo.com/ContentAnalysisService/V1/termExtraction");

// Create the web request
HttpWebRequest request = WebRequest.Create(address) as HttpWebRequest;

// Set type to POST
request.Method = "POST";
request.ContentType = "application/x-www-form-urlencoded";

// Create the data we want to sendstring appId = "YahooDemo";
string context = "Italian sculptors and painters of the renaissance"
+ "favored the Virgin Mary for inspiration";
string query = "madonna";

StringBuilder data = new StringBuilder();
data.Append("appid=" + HttpUtility.UrlEncode(appId));
data.Append("&context=" + HttpUtility.UrlEncode(context));
data.Append("&query=" + HttpUtility.UrlEncode(query));

// Create a byte array of the data we want to sendbyte[] byteData = UTF8Encoding.UTF8.GetBytes(data.ToString());

// Set the content length in the request headers
request.ContentLength = byteData.Length;

// Write datausing (Stream postStream = request.GetRequestStream())
{
postStream.Write(byteData, 0, byteData.Length);
}

// Get responseusing (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
{
// Get the response stream
StreamReader reader = new StreamReader(response.GetResponseStream());

// Console application output
Console.WriteLine(reader.ReadToEnd());
}
### 三、HTTP 验证请求

有些朋友在调用Restful Api时，可能要提供用户名和密码进行Http身份验证，这里我们可以通过为请求增加一个 **NetworkCredentials** 实例来完成.

// Create the web request
HttpWebRequest request
= WebRequest.Create("https://api.del.icio.us/v1/posts/recent") as HttpWebRequest;

// Add authentication to request
request.Credentials = new NetworkCredential("username", "password");

// Get responseusing (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
{
// Get the response stream
StreamReader reader = new StreamReader(response.GetResponseStream());

// Console application output
Console.WriteLine(reader.ReadToEnd());
}



