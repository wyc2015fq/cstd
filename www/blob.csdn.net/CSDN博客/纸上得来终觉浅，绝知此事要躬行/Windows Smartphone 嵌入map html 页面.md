# Windows Smartphone 嵌入map html 页面 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月23日 15:59:52[boonya](https://me.csdn.net/boonya)阅读数：930标签：[windows																[webservice																[webbrowser																[sdk																[c#](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[C#](https://blog.csdn.net/boonya/article/category/1776321)





前言：开发架构          BS       &      CS ；实现地图导航功能。


**BS：Spring+hibernate、Java、JDK1.7      （设想环境）**

**CS：webservice、C#、.Net framwork 3.5&Windows mobile 6.0 SDK**

**1、创建“智能设备项目”。**

![](https://img-blog.csdn.net/20140623154633343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140623154656156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**2、使用“webBrowser”空间创建框架内置浏览器。**

引入COM下面 windows web程序支持包，在窗体源码中编写如下代码：



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MapSmart
{
    [System.Runtime.InteropServices.ComVisibleAttribute(true)]    
    public partial class FrmMap : Form
    {
        public FrmMap()
        {
            InitializeComponent();
        }

        private void FrmMap_Load(object sender, EventArgs e)
        {
            // JSP中 调用window方法
            // onclick="window.external.functionname()"；

            //为HTML文件传递JS参数
            //object oSum = wb.Document.InvokeScript("sayHello", new object[] { 1, 2 });

            string url = "http://map.baidu.com/";
            Uri uri = new Uri(url);
            this.wb.Navigate(uri, "");

        }
    }
}
```
引入webbrowser所需要的库：



![](https://img-blog.csdn.net/20140623155930656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**3、启动调试查看结果（引入html地址后）效果图。**

![](https://img-blog.csdn.net/20140623154824234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140623154904390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20140623154943343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**4、参数传递**

1、REST风格URL传递参数。

2、脚本方法调用参数传递：如2中代码注释处。](https://so.csdn.net/so/search/s.do?q=sdk&t=blog)](https://so.csdn.net/so/search/s.do?q=webbrowser&t=blog)](https://so.csdn.net/so/search/s.do?q=webservice&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)




