# C#访问HTTP请求 - Big Smile - CSDN博客
2017年11月05日 21:45:00[王啸tr1912](https://me.csdn.net/tr1912)阅读数：8210
代码连接：[http://download.csdn.net/download/tr1912/10104222](http://download.csdn.net/download/tr1912/10104222)
# 一、业务需求
      由于需要在一个写好的C/S系统上调用一个用java写好的restful风格的接口，所以我就去查找了一下用.net程序发送get，或者post请求的方法，去请求已经用java写好的web接口，由于纯窗体风格的C/S系统没有自带的http请求方法，所以需要用到system.net这个引用才能实现我们的调用要求。
# 二、代码实现
      下面是实现的代码，我先贴出来：
```
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
namespace echuUI
{
    public class HTTPSendingHelper
    {
        /// <summary>   
        /// Get方式获取url地址输出内容   
        /// </summary> /// <param name="url">url</param>   
        /// <param name="encoding">返回内容编码方式，例如：Encoding.UTF8</param>   
        public static String SendRequest(String url, Encoding encoding)
        {
            HttpWebRequest webRequest = (HttpWebRequest)WebRequest.Create(url);
            webRequest.Method = "GET";
            HttpWebResponse webResponse = (HttpWebResponse)webRequest.GetResponse();
            StreamReader sr = new StreamReader(webResponse.GetResponseStream(), encoding);
            return sr.ReadToEnd();
        }
    }
}
```
        其实在这个里面，我们主要用的using System.Net;这个引用来实现的具体操作，这个里面封装了HttpWebRequest这个操作对象，我们不难发现，这些东西，其实微软的这个.net平台已经为我们准备完成了，所以我们可以直接就调用了，顺便查了一下在ASP.net中是怎么用的：
```
//get方法调用接口获取json文件内容
        public void GetFunction()
        {
           
            string serviceAddress = "http://222.111.999.444:8687/tttr/usercrd/12/b7e50cb45a?userid=9999";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(serviceAddress);
            request.Method = "GET";
            request.ContentType = "text/html;charset=UTF-8";
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            Stream myResponseStream = response.GetResponseStream();
            StreamReader myStreamReader = new StreamReader(myResponseStream, Encoding.UTF8);
            string retString = myStreamReader.ReadToEnd();
            myStreamReader.Close();
            myResponseStream.Close();
            Response.Write(retString);
        }
        ////post方法调用接口获取json文件内容
        public void PostFunction()
        {
            string serviceAddress = "http://222.111.999.444:8687/tttr/usercrd/uuu/12/dfd7e4";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(serviceAddress);
            request.Method = "POST";
            request.ContentType = "application/json";
            string strContent = @"{ ""mmmm"": ""89e"",""nnnnnn"": ""0101943"",""kkkkkkk"": ""e8sodijf9""}";
            using (StreamWriter dataStream = new StreamWriter(request.GetRequestStream()))
            {
                dataStream.Write(strContent);
                dataStream.Close();
            }
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            string encoding = response.ContentEncoding;
            if (encoding == null || encoding.Length < 1)
            {
                encoding = "UTF-8"; //默认编码  
            }
            StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.GetEncoding(encoding));
            string retString = reader.ReadToEnd();
            //解析josn
            JObject jo = JObject.Parse(retString);
            Response.Write(jo["message"]["mmmm"].ToString());
          
        }
```
        我们可以看到，他就是调用的这个HttpWebRequest对象，好像也得做这种引用，看来.net封装的这种东西都是共用性很强的东西啊。
