# Selenium Webdriver常见错误、异常解答 - zhusongziye的博客 - CSDN博客





2018年03月26日 20:48:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3697








## Q：启动IE浏览器时突然报下面错误，不能正常使用。

WebDriverException: Message: Unexpected error launching Internet Explorer. Browser zoom level was set to 94%. It should be set to 100%

A：原因是IE页面的使用的的显示比例不是100%导致的，把页面显示调整成100%恢复正常。






## Q:找不到元素，脚本报NoSuchElementException:Unable to find element 异常怎么办?

NoSuchElementException: Message: no such element 当找不到元素的时候，就会抛这样的异常，应该如何解决呢？


A:这个问题不是某一个特定原因造成的，与自己的脚本开发经验有关系，提供解决思路，按照这个思路挨个去找，相信你能解决。
- 
    检查使用的元素标识是否正确 如： “username” 中写成了 “usernome”


- 
    检查使用查找的元素标签是否是唯一的 。如：在HTML代码元素中只能有一个 “id = kw” 的元素


- 
    检查查找的元素是否在frame标签下。如果是需要写切换到frame中在进行查找


- 
检查元素属性是否是会变动的，可以尝试使用其它的（如：css、xpath等）属性查找看看

- 
    检查元素属性是否是隐藏的

- 
    可以在元素查找前面加个等待时间试试

- 
    使用Webdriver实在无法定位的话，使用Js进行定位。





## Q: 启动firefox浏览器，报“Plugin Container for Firefox已停止工作” 处理办法?




![](https://img-blog.csdn.net/20180326204023562?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


A:添加用户环境变量 MOZ_DISABLE_OOP_PLUGINS，值为1。

或者：尝试删除firefox安装目录中的 plugin-container.exe 文件

如果firefox、ipython打开着，关闭进程，重启firefox、ipython




## Q:启动firefox会加载火狐官网响应特别慢，如何去掉这个加载提升跳转URL速度？

A:修改firefox-profile文件，删除默认的profile,添加一个自定义的profile

 进入CMD 输入 firefox -ProfileManager -no-remote 打开配置项进行删除和创建。

## ![](https://img-blog.csdn.net/20180326204211940?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 修改代码中启动Firefox的驱动为self.driver = webdriver.Firefox(firefox_profile='selenium',timeout=10)

## Q:如何使用默认已存在的profile启动firefox？

A:参考：http://stackoverflow.com/questions/11095294/using-the-default-firefox-profile-with-selenium-webdriver-in-python

使用已存在profile启动firefox可以自动登陆已经登陆过的站点。代码如下：
- 
`fp = webdriver.FirefoxProfile('/path/to/your/existing/profile')`

- 
`browser = webdriver.Firefox(fp)`

如何创建一个定制的profile？戳这里：https://support.mozilla.org/en-US/kb/profile-manager-create-and-remove-firefox-profiles
这里在windows上有个坑，就是路径分隔符在windows上是\而不是/ ,把这个弄明白然后指定对路径基本就可以了。

PS：这里还有另一个坑。就是如果你使用默认的profile的话，请一定关闭friefox以后再运行代码，否则会因为profile的文件锁问题而发生异常。就是说

一次只能打开一个firefox实例，如果你使用默认的profile的话。

## Q:如何在打开Firefox的同时打开firebug ?

A:首先下载Firebug XPI文件（这个就是friefox的扩展程序文件--by乙醇），然后再调用firefox profile的add_extension方法。
- 
`from selenium import webdriver`

- 
`fp = webdriver.FirefoxProfile()`

- 
`fp.add_extension(extension='firebug-1.8.4.xpi')`

- 
`fp.set_preference("extensions.firebug.currentVersion", "1.8.4") #Avoid startup screen`

- 
`browser = webdriver.Firefox(firefox_profile=fp)`


## Q:使用switch_to_frame后，如何返回原frame?

A:switch_to_default_content




## Q:同级目录导入方法和在pycharm中显示红色下划线但能正常运行的解决方法?

A: 


![](https://img-blog.csdn.net/20180326204418402?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设我需要在public\base.py中导入data\all_test.py，首先需要在data中新建一个空白的__init__.py，然后在base.py中写入
- 
`sys.path.append("..")`

- 
`from data import all_test`

- 




虽然不会报错，但在pycharm中会显示成红色，并且导入的模块中的函数在引用时不会有补全和提示功能。其实导入已经正常了，该问题是pycharm的问题，在pycharm的setting中做如下设置就可以了：


把data或者data的上级目录设置成Sources，此时红色下划线会消失，并且也会有导入函数的补全和提示功能。

备：在pycharm上做了以下设置时，其实不需要sys.path.append("..")也是可以导入的，但这不符合python的语法，如果你这样写，离开了pycharm的环境就会报错，所以还是得加上这一句

![](https://img-blog.csdn.net/20180326204429810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## Q:Jenkins中控制台中文展示和自动更新代码的变更记录中文描述信息显示乱码，如何解决?

A:在启动Jenkins的启动配置文件中，添加文件编码的参数：-Dfile.encoding=utf-8。

Windows系统文件路径：Jenkins安装目录中的 jenkins.xml
- 
`<arguments>-Xrs -Xmx256m -Dfile.encoding=utf-8 -Dhudson.lifecycle=hudson.lifecycle.WindowsServiceLifecycle -jar "%BASE%\jenkins.war" --httpPort=8080</arguments>`


Linux RPM包服务安装： /etc/sysconfig/jenkins 文件中配置

           JENKINS_JAVA_OPTIONS="-Djava.awt.headless=true -Dfile.encoding=utf-8 -Dsun.jnu.encoding=utf-8"












## Q:在pycharm中经常出现下划线怎么办?

如下图：


![](https://img-blog.csdn.net/20180326204527362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


A:点击要忽略的错误所在，然后按住ALT+ENTER，在弹出下拉菜单选择ignore errors like this ,就可以快速忽略了，如下图：


![](https://img-blog.csdn.net/20180326204557997?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





Q:出现如下图的错误时怎么处理？
![](https://img-blog.csdn.net/20180326204615493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
A:检查jar包，将  selenium-server-standalone-2.41.0.jar(可能jar包版本会出现不一致) 加入到build path中。然后clear up 一下项目即可。






Q:脚本中的元素和组件怎么定义？




A:元素与组件，组件可以包含元素

通常所说的元素，就是指HTML页面，你可以看到的HTML 元素，那么在脚本里面，比如 find_element_by_id("kw1") 就是定位某一个元素，你也可以把它当作组件。

对于组件来说，我们一般会把一些有操作的意义，或者功能，封装在一个方法或者类里面，那么这个方法或者类，就可以看作一个组件。

就像你要定义一个登录组件
- 
`def  login(user,pass){`

- 
`    find_element_by_id(user)`

- 
`    find_element_by_id(pass)`

- 
`    find_element_by_id("submit").click()`

- 
`}`


这整个方法，就可以看着是一个组件。而user，pass ，submit 就是元素。


## ***Q：启动IE浏览器时突然报下面错误，不能正常使用。***

*WebDriverException: Message: Unexpected error launching Internet Explorer. Browser zoom level was set to 94%. It should be set to 100%*

A：原因是IE页面的使用的的显示比例不是100%导致的，把页面显示调整成100%恢复正常。






## ***Q:找不到元素，脚本报NoSuchElementException:Unable to find element 异常怎么办?***

*NoSuchElementException: Message: no such element 当找不到元素的时候，就会抛这样的异常，应该如何解决呢？*

A:这个问题不是某一个特定原因造成的，与自己的脚本开发经验有关系，提供解决思路，按照这个思路挨个去找，相信你能解决。
- 
    检查使用的元素标识是否正确 如： “username” 中写成了 “usernome”


- 
    检查使用查找的元素标签是否是唯一的 。如：在HTML代码元素中只能有一个 “id = kw” 的元素


- 
    检查查找的元素是否在frame标签下。如果是需要写切换到frame中在进行查找


- 
检查元素属性是否是会变动的，可以尝试使用其它的（如：css、xpath等）属性查找看看

- 
    检查元素属性是否是隐藏的

- 
    可以在元素查找前面加个等待时间试试

- 
    使用Webdriver实在无法定位的话，使用Js进行定位。





## ***Q: 启动firefox浏览器，报“Plugin Container for Firefox已停止工作” 处理办法?***



![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI6BprlhiaVALR8a1K4Gf6fWKR8vJ9jauPew6qOJf1o8geNPia5kSwtG4uw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


A:添加用户环境变量 MOZ_DISABLE_OOP_PLUGINS，值为1。

或者：尝试删除firefox安装目录中的 plugin-container.exe 文件

如果firefox、ipython打开着，关闭进程，重启firefox、ipython




## *Q:启动firefox会加载火狐官网响应特别慢，如何去掉这个加载提升跳转URL速度？*

A:修改firefox-profile文件，删除默认的profile,添加一个自定义的profile

 进入CMD 输入 firefox -ProfileManager -no-remote 打开配置项进行删除和创建。

## ![](https://mmbiz.qpic.cn/mmbiz_jpg/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI67icdThz8LtO0YEyzXeP23XyRS0EhCNUea2cbPKicPjKD5UmLlWXdGia9Q/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

## 修改代码中启动Firefox的驱动为self.driver = webdriver.Firefox(firefox_profile='selenium',timeout=10)

## *Q:如何使用默认已存在的profile启动firefox？*

A:参考：http://stackoverflow.com/questions/11095294/using-the-default-firefox-profile-with-selenium-webdriver-in-python

使用已存在profile启动firefox可以自动登陆已经登陆过的站点。代码如下：
- 
`fp = webdriver.FirefoxProfile('/path/to/your/existing/profile')`

- 
`browser = webdriver.Firefox(fp)`

如何创建一个定制的profile？戳这里：https://support.mozilla.org/en-US/kb/profile-manager-create-and-remove-firefox-profiles
这里在windows上有个坑，就是路径分隔符在windows上是\而不是/ ,把这个弄明白然后指定对路径基本就可以了。

PS：这里还有另一个坑。就是如果你使用默认的profile的话，请一定关闭friefox以后再运行代码，否则会因为profile的文件锁问题而发生异常。就是说

一次只能打开一个firefox实例，如果你使用默认的profile的话。

## *Q:如何在打开Firefox的同时打开firebug ?*

*A:*首先下载Firebug XPI文件（这个就是friefox的扩展程序文件--by乙醇），然后再调用firefox profile的add_extension方法。
- 
`from selenium import webdriver`

- 
`fp = webdriver.FirefoxProfile()`

- 
`fp.add_extension(extension='firebug-1.8.4.xpi')`

- 
`fp.set_preference("extensions.firebug.currentVersion", "1.8.4") #Avoid startup screen`

- 
`browser = webdriver.Firefox(firefox_profile=fp)`


## *Q:使用switch_to_frame后，如何返回原frame?*

A:switch_to_default_content




## *Q:同级目录导入方法和在pycharm中显示红色下划线但能正常运行的解决方法?*

A: 


![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI64WDr3Ms8w2KxJQnqxI96GoZWVZLVEuTZf7oZZmbfKKibRb85gXYArkg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

假设我需要在public\base.py中导入data\all_test.py，首先需要在data中新建一个空白的__init__.py，然后在base.py中写入
- 
`sys.path.append("..")`

- 
`from data import all_test`

- 




虽然不会报错，但在pycharm中会显示成红色，并且导入的模块中的函数在引用时不会有补全和提示功能。其实导入已经正常了，该问题是pycharm的问题，在pycharm的setting中做如下设置就可以了：


把data或者data的上级目录设置成Sources，此时红色下划线会消失，并且也会有导入函数的补全和提示功能。

备：在pycharm上做了以下设置时，其实不需要sys.path.append("..")也是可以导入的，但这不符合python的语法，如果你这样写，离开了pycharm的环境就会报错，所以还是得加上这一句

![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI6f5iaxlkZqickDLxGOicQs7QaIZC5nc4uuh7oddlsvZVUiaXnh12kTssrNA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)




## *Q:Jenkins中控制台中文展示和自动更新代码的变更记录中文描述信息显示乱码，如何解决?*

A:在启动Jenkins的启动配置文件中，添加文件编码的参数：-Dfile.encoding=utf-8。

Windows系统文件路径：Jenkins安装目录中的 jenkins.xml
- 
`<arguments>-Xrs -Xmx256m -Dfile.encoding=utf-8 -Dhudson.lifecycle=hudson.lifecycle.WindowsServiceLifecycle -jar "%BASE%\jenkins.war" --httpPort=8080</arguments>`


Linux RPM包服务安装： /etc/sysconfig/jenkins 文件中配置

           JENKINS_JAVA_OPTIONS="-Djava.awt.headless=true -Dfile.encoding=utf-8 -Dsun.jnu.encoding=utf-8"






开源优测

分享软件测试开源技术、经验、方案的首发平台

![](https://mmbiz.qpic.cn/mmbiz_jpg/8vQRibibAC6qWBqV4WiblBKPQXFGicoGTqfLqQBxrHIdM5yAtQ3m751q53bRChapicInJNV5fXevhL5GZnDVsXrXHpw/640?wx_fmt=jpeg&tp=webp&wxfrom=5&wx_lazy=1)

**长按二维码/微信扫描 关注开源优测**

*苦叶子个人微信：liyimin1912*


有问题，可留言或加好友进微信群







## *Q:在pycharm中经常出现下划线怎么办?*

如下图：


![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI6V1pboOYJGYcnog26ibSibICicID8IgM9whib1iaTEAeWPNMcuBT5j1Kpj8A/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

A:点击要忽略的错误所在，然后按住ALT+ENTER，在弹出下拉菜单选择ignore errors like this ,就可以快速忽略了，如下图：


![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI6hqg79wXOuDUFklXSU7rQcPI5apzjHbB3zicoxvJqvHtQftiaaCyB1LUA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)





***Q:出现如下图的错误时怎么处理？***
![](https://mmbiz.qpic.cn/mmbiz_png/8vQRibibAC6qUNibKRsWUUveuJXEbLe9lI6Svy91dZyVR9mjMicqsDl3iamwx26EvauLQSCnkbWrq7JuaibVd7XMQRwQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)
A:检查jar包，将  selenium-server-standalone-2.41.0.jar(可能jar包版本会出现不一致) 加入到build path中。然后clear up 一下项目即可。






***Q:脚本中的元素和组件怎么定义？***




A:元素与组件，组件可以包含元素

通常所说的元素，就是指HTML页面，你可以看到的HTML 元素，那么在脚本里面，比如 find_element_by_id("kw1") 就是定位某一个元素，你也可以把它当作组件。

对于组件来说，我们一般会把一些有操作的意义，或者功能，封装在一个方法或者类里面，那么这个方法或者类，就可以看作一个组件。

就像你要定义一个登录组件
- 
`def  login(user,pass){`

- 
`    find_element_by_id(user)`

- 
`    find_element_by_id(pass)`

- 
`    find_element_by_id("submit").click()`

- 
`}`


这整个方法，就可以看着是一个组件。而user，pass ，submit 就是元素




来自为知笔记(Wiz)




