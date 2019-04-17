# python3 selenium之火狐Firefox浏览器加载浏览器配置 - zhusongziye的博客 - CSDN博客





2017年09月06日 21:27:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1772








本博客所有selenium环境都是win 7环境，mac的环境暂不研究！小编比较穷，无力购买mac！等小编有钱了，再说吧。。。

童鞋们在跑selenium自动化时，发现打开的firefox没有firebug插件图标有木有？？是不是感到不方便啊，呵呵。。。小编，告诉你加载浏览器配置文件后，就可以啦，就可以方便童鞋们定位啦！！！

话说，浏览器配置文件在哪里呢？看，看图：

![](https://img-blog.csdn.net/20170906213839854?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


根据你自己的电脑路径，进行更改就好啦！简单吧！

上代码：



```python
# coding=utf-8
from selenium import webdriver
import time
# 配置文件地址
profile_directory = r'C:\Users\Dell\AppData\Roaming\Mozilla\Firefox\Profiles\8nq06rq1.default'
# 加载配置配置
profile = webdriver.FirefoxProfile(profile_directory)
# 启动浏览器配置
drive = webdriver.Firefox(profile)

# drive = webdriver.Firefox()
drive.get("http://www.baidu.com")

drive.find_element_by_id("kw").send_keys("selenium2")
drive.find_element_by_id("su").click()
time.sleep(3)
drive.quit()
```





