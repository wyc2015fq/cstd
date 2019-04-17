# python3 Selenium启动加载用户配置的浏览器（Chrome/Firefox） - zhusongziye的博客 - CSDN博客





2018年03月21日 11:03:12[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1092










      Selenium操作浏览器是不加载任何配置的，网上关于Firefox加载配置的多点，Chrome资料很少，下面是关于加载Chrome配置的方法：

**一、加载所有Chrome配置**

　　用Chrome地址栏输入chrome://version/，查看自己的“个人资料路径”，然后在浏览器启动时，调用这个配置文件，代码如下：

```python
#coding=utf-8
from selenium import webdriver
option = webdriver.ChromeOptions()
option.add_argument('--user-data-dir=C:\Users\Administrator\AppData\Local\Google\Chrome\User Data') #设置成用户自己的数据目录
driver=webdriver.Chrome(chrome_options=option)
```

![](https://img-blog.csdn.net/20180321105536283?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**二、修改浏览器的User-Agent来伪装你的浏览器访问手机m站**


```python
#coding=utf-8
from selenium import webdriver

option = webdriver.ChromeOptions()
option.add_argument('--user-agent=iphone')
driver = webdriver.Chrome(chrome_options=option)
driver.get('http://www.taobao.com/')
```


**三、浏览器启动时安装crx扩展**


```python
#coding=utf-8
from selenium import webdriver
option = webdriver.ChromeOptions()
option.add_extension('d:\crx\AdBlock_v2.17.crx') #自己下载的crx路径
driver = webdriver.Chrome(chrome_options=option)
driver.get('http://www.taobao.com/')
```



　可以去https://sites.google.com/a/chromium.org/chromedriver/capabilities查看更多，或者去http://stackoverflow.com/查找。



下边是收集的一些配置信息：

　　一些Chrome的地址栏命令（这些命令会不停的变动，所有不一定都是好用的）

　　在Chrome的浏览器地址栏中输入以下命令，就会返回相应的结果。这些命令包括查看内存状态，浏览器状态，网络状态，DNS服务器状态，插件缓存等等。

　　about:version - 显示当前版本

　　about:memory - 显示本机浏览器内存使用状况

　　about:plugins - 显示已安装插件

　　about:histograms - 显示历史记录

　　about:dns - 显示DNS状态

　　about:cache - 显示缓存页面

　　about:gpu -是否有硬件加速

　　about:flags -开启一些插件 //使用后弹出这么些东西：“请小心，这些实验可能有风险”，不知会不会搞乱俺的配置啊！

　　chrome://extensions/ - 查看已经安装的扩展



　　其他的一些关于Chrome的实用参数及简要的中文说明（使用方法同上，当然也可以在shell中使用）

　　–user-data-dir=”[PATH]” 指定用户文件夹User Data路径，可以把书签这样的用户数据保存在系统分区以外的分区。

　　–disk-cache-dir=”[PATH]“ 指定缓存Cache路径

　　–disk-cache-size= 指定Cache大小，单位Byte

　　–first run 重置到初始状态，第一次运行

　　–incognito 隐身模式启动

　　–disable-javascript 禁用Javascript

　　--omnibox-popup-count="num" 将地址栏弹出的提示菜单数量改为num个。我都改为15个了。

　　--user-agent="xxxxxxxx" 修改HTTP请求头部的Agent字符串，可以通过about:version页面查看修改效果

　　--disable-plugins 禁止加载所有插件，可以增加速度。可以通过about:plugins页面查看效果

　　--disable-javascript 禁用JavaScript，如果觉得速度慢在加上这个

　　--disable-java 禁用java

　　--start-maximized 启动就最大化

　　--no-sandbox 取消沙盒模式

　　--single-process 单进程运行

　　--process-per-tab 每个标签使用单独进程

　　--process-per-site 每个站点使用单独进程

　　--in-process-plugins 插件不启用单独进程

　　--disable-popup-blocking 禁用弹出拦截

　　--disable-plugins 禁用插件

　　--disable-images 禁用图像

　　--incognito 启动进入隐身模式

　　--enable-udd-profiles 启用账户切换菜单

　　--proxy-pac-url 使用pac代理 [via 1/2]

　　--lang=zh-CN 设置语言为简体中文

　　--disk-cache-dir 自定义缓存目录

　　--disk-cache-size 自定义缓存最大值（单位byte）

　　--media-cache-size 自定义多媒体缓存最大值（单位byte）

　　--bookmark-menu 在工具 栏增加一个书签按钮

　　--enable-sync 启用书签同步





