
# WebBrowser设置Cookie - 萌萌的It人 www.itmmd.com - CSDN博客


2014年12月02日 18:09:52[Jlins](https://me.csdn.net/dyllove98)阅读数：1774


在winform里面经常会用到WebBrowser，这是一个难点就是如何设置cookies，注意，Docment对象是只读的，所以WebBrowser.Docment.cookie也就只有get方法，没有set方法，下面将介绍如何设置cookies。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicpartialclassWebBrowserControl : Form
    {privateString url;
        [DllImport("wininet.dll", CharSet = CharSet.Auto, SetLastError =true)]publicstaticexternboolInternetSetCookie(stringlpszUrlName,stringlbszCookieName,stringlpszCookieData);publicWebBrowserControl(String path)
        {this.url =path;
            InitializeComponent();//set cookieInternetSetCookie(url,"JSESSIONID", Globals.ThisDocument.sessionID);//navigatewebBrowser.Navigate(url);  
        }
        ...
}![复制代码](http://common.cnblogs.com/images/copycode.gif)


