# Selenium3源码之初识篇 - zhusongziye的博客 - CSDN博客





2018年03月10日 09:09:47[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：802








### 前提

本系列是对Selenium3的Python3版本的源码进行解读分析，所以你应该需要以下准备：

安装Python3，请自行去下载安装或参照公众号（开源优测）中Python3系列中的文章学习怎么安装Python3

使用以下命令安装最新的Selenium3

> 
pip install selenium -U


安装结果如下图：

![](https://img-blog.csdn.net/20180310090813236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





selenium安装成功后你可以在Python的安装目录的lib/site-packages/selenium下找到对应的源码

### 认识下目录




我们通过下面这张图了解下Selenium3的Python3版本源码目录结构：

![](https://img-blog.csdn.net/20180310090831559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




目录说明：

> 
selenium/common
定义了webdriver的异常类


> 
selenium/webdrver
定义了webdriver所有Python实现,包含：
- 
各种浏览器支持

- 
各种UI操作方法

- 
异常处理

- 
定为方法

- 
json wire protocol映射
等等



以下为浏览器支持模块：

> 
selenium/webdriver/android android原生浏览器
selenium/webdriver/chrome 谷歌浏览器
selenium/webdriver/edge 微软edge浏览器
selenium/webdriver/firefox 火狐浏览器
selenium/webdriver/ie 微软ie浏览器
selenium/webdriver/opera opera浏览器
selenium/webdriver/phantomjs phantomjs headless浏览器（运行在内存模式）
selenium/webdriver/safari safari浏览器
selenium/webdriver/webkitgtk WebKitGTKDriver浏览器
selenium/webdriver/blackberry blackberry浏览器


以下为核心代码模块:

> 
selenium/webdriver/common webdriver共用功能
selenium/webdriver/remote webdriver核心代码实现
selenium/webdriver/support webdriver辅助功能代码


以下模块为webdriver对外导出能力：
selenium/webdriver/init.py
代码如下如图所示：


![](https://img-blog.csdn.net/2018031009085677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对外导出了webdriver通用的对象及webdriver版本定义

### 小结

本文简单介绍了Selenium3 Python3源码的目录结构，后续就各功能块的源码进行分享。



