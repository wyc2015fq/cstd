# 模拟浏览器自动化测试工具Selenium之五Centos系统命令行下部署selenium环境试验 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月16日 13:04:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4729








                
一、背景：

    Selenium是一个web自动化测试框架，也支持从HTML页面上爬取javascript生成的或AJAX的内容。

    selenium2支持通过各种driver（FirfoxDriver、IternetExplorerDriver、OperaDriver、ChromeDriver）驱动真实浏览器完成测试的。

    selenium也是支持无界面浏览器操作的，如集成HtmlUnit和PhantomJs，不是真正的浏览器，运行时不会渲染页面显示内容，但是支持页面元素查找、动态js执行等；由于不进行css和gui渲染，运行效率要比真实的浏览器快。


    现在的问题是在windows+eclipse环境下java开发（通过driver驱动的）的程序如何部署到命令行centos下运行selenium呢？



二、选型：

   对selenium2支持浏览器driver、htmlunit、phantomJs三种进行比较：



1）selenium2集成htmlUnit

    htmlUnit是java实现的类浏览器程序，包含在selenium server中，无须驱动，直接实例化就行。

   用法如下：

   WebDriver driver = new HtmlUnitDriver();//

   WebDriver driver = new HtmlUnitDriver(true);//支持js

   htmlUnit速度快，但对js支持不是很好，适合相对静态的页面爬取。



2）selenium2集成phantomJs

   速度一般、模拟行为接近真实，但不能模拟不同/特定浏览器的行为，如鼠标点击操作，适合非GUI功能性自动测试。

   使用phantomJS配合selenium，可以创建无界面的浏览器。

   但发现似乎只支持Python语言，Java语言没有phantomJs实例，只能放弃。



3）真实浏览器driver

   可真实模拟用户行为，但效率、稳定性低，但支持特定浏览器行为，适合GUI功能测试。


   笔者在开发情况，是选择真实浏览器驱动，支持的比较完整，但要部署到命令行centos下时就需要找解决方案了。  

   Headless Selenium支持在服务器上运行的健壮的自动化操作，不需要显示设备。

   笔者计划在命令行centos下采用Headless Selenium方案，用Xvfb创建虚拟的X窗口。



三、设置centos代理服务器联网：

   1）全局设置：

      #vi /etc/profile

      export http_proxy=yourproxy:port

      export ftp_proxy=yourproxy:port

   2）单独yum的代理设置修改/etc/yum.conf

   3）单独wget的代理设置修改/etc/wgetrc

   设置联网方便yum安装一些组件。



四、centos下部署phantomJS+selenium 

   实际生产中没有选择该方案，这里给出，只是做参考。

   1、安装PhantomJS：

   1）hantomJS 是一个基于WebKit的服务器端 JavaScript API。它全面支持web而不需浏览器支持，其快速，原生支持各种Web标准： DOM 处理, CSS 选择器, JSON, Canvas, 和 SVG。PhantomJS可以用于页面自动化，网络监测，网页截屏，以及无界面测试等。

   2）安装包下载地址：http://phantomjs.org/download.html，包括Windows，Mac OS，Linux版本，自行选择对应 版本下载解压即可（为方便使用，可自已为phantomjs设置环境变量），其中带有一个example文件夹，里面有很多已经写好的代码供使用。

   3）下载 phantomjs-2.1.1-linux-x86_64.tar.bz2 (22.3 MB) 并解压；

       #tar jxvf phantomjs-2.1.1-linux-x86_64.tar.bz2

       #cd phantomjs-2.1.1-linux-x86_64/bin

       进入bin文件夹执行测试

   4）测试：

      —新建文本文件hw.js，输入下面：

         #vi hw.js

         console.log('Hello, world!');

         phantom.exit();

      —执行

        # ./phantomjs hw.js

        提示：/lib64/libz.so.1: no version information available

        更新zlib版本：

        #wget http://zlib.net/zlib-1.2.8.tar.gz

        #tar zxvf zlib-1.2.8.tar.gz

        #cd zlib-1.2.8

        # ./configure

        #make

        #make install

        覆盖原版本，可以先备份一下原版本

        #cp /usr/local/lib/libz.so.1 /lib64/    

   5）建立软链:

        #ln -s /tmp/phantomjs-2.1.1-linux-x86_64/bin/phantomjs /usr/local/bin/phantomjs 

       命令行下直接使用phantomjs命令，不需要进入bin目录下执行。

   6）参考http://www.phperz.com/article/14/1115/35197.html



   2、安装pip：

   1)查看和升级python版本：

     #python -V   是Python 2.6.6

     升级python版本

     #wget https://www.python.org/ftp/python/3.5.2/Python-3.5.2.tgz

     #tar -xf Python-3.5.2.tgz

     #cd Python-3.5.2

     #./configure

     #make

     #make install

     #ln -s /usr/local/bin/python3.5.2 /usr/bin/python

   2)下载get-pip.py

     https://bootstrap.pypa.io/get-pip.py

   3)安装pip：

     #python get-pip.py



   3、安装selenium

   1)pip安装：

     #pip install -U selenium

   现在可以执行Python代码实例化webdriver.PhantomJS('phantomjs')爬虫程序。



五、centos下部署Headless Selenium

    实际生产中选择了该方案，用Xvfb创建虚拟的X窗口，模拟运行浏览器。

    Xvfb是一个实现了X11显示服务协议的显示服务器。 不同于其他显示服务器，Xvfb在内存中执行所有的图形操作，不需要借助任何显示设备。

   1、安装chrome浏览器：

      chrome对centos6不够支持，需要安装很多组件，升级至centos7。

      #yum install google-chrome-stable 

      网上有很多说明配置yum源的，可以参考。

   2、安装Xvfb

      #yum install Xvfb

      #yum -y install libXfont

      #yum install xorg-x11-fonts*

   3、启动Xvfb和浏览器

      #Xvfb -ac:7 -screen 0 1280x1024x8

      启动 chrome

           export  DISPLAY=:7 （和上一步的number号相同）

           chrome www.baidu.com

    由于服务器限制，没有成功部署到，这里先记录做参考。



六、移植程序

    将eclipse下的Java工程打包成jar，提交centos下java -jar执行，在启动浏览器和Xvfb情况下执行。

    参考代码：



```java
package com.test;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.chrome.ChromeDriver;

public class headless {
	public static void main(String[] args) {
		System.getProperties().setProperty("webdriver.chrome.driver","/tmp/chromedriver.exe");	
		WebDriver webDriver = new ChromeDriver();
		webDriver.get("http://www.baidu.com");//访问网址
		System.out.println(webDriver.getTitle());
		webDriver.close();
		webDriver.quit();
	}
}
```


七、总结：了解到selenium框架，并选定Xvfb来虚拟GUI实现centos下运行selenium，当然如果采用python语言可以选择PhantomJS。

           在部署过程中，由于联网环境限制，yum源无法更新也无法安装，因此只能作为参考先记录。

           后续在实际生产中，需要验证：

    1）centos7+chrome+Xvfb部署，验证是否可以在命令行下启动浏览器；

    2）chromedriver_x64.exe驱动是否需要在代码显示加载；

    3）在前面两个都满足情况下，是否可以运行selenium；





