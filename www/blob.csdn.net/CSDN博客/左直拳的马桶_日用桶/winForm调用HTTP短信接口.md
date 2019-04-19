# winForm调用HTTP短信接口 - 左直拳的马桶_日用桶 - CSDN博客
2010年01月28日 18:35:00[左直拳](https://me.csdn.net/leftfist)阅读数：3150标签：[winform																[dataset																[浏览器																[session																[url																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)](https://so.csdn.net/so/search/s.do?q=winform&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
最近我们使用了一个短信接口，是http的，给出的访问方式都是一个个网址，参数就附在后面，又是?又是&之类。真是第一次见。
怎么调用呢？总不能在服务器开个浏览器，上面在不停地访问对方站点吧？我想用winform的方式。
可以使用HttpWebRequest + HttpWebResponse。测试之后，发现是可以访问，但不能保持会话。发短信之前需要登录，现在这种方式，登录是一个网址，发送是另外一个网址，登录之后再想发送，对方站点却不认，认为你没有登录。如果用浏览器没有问题，估计就是会话的原因。
众所周知，.NET里面说什么会话（SESSION）有三种实现方式，其中使用最多的是COOKIE。所以应该保存这个cookie。
CookieContainer cc = new CookieContainer();//声明一个全局变量，用于保存COOKIE
        DataTable IGetIt(string url)
        {//这个就是访问短信接口的函数
            HttpWebRequest request = HttpWebRequest.Create(url) as HttpWebRequest;
            request.Method = "GET";
            request.CookieContainer = cc;//设置COOKIE。第一次估计是空的，但访问一次后，应当有内容在里面
            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
            DataSet ds = new System.Data.DataSet();
            ds.ReadXml(response.GetResponseStream(), System.Data.XmlReadMode.Auto);
            return ds.Tables[0];
     }
这样就行鸟。
