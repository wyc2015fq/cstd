# python selenium使用chrome/firefox的已存在的cookie访问网站 - zhusongziye的博客 - CSDN博客





2018年03月21日 11:21:19[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1667








      如果我们在跑自动化程序之前，已经登录了某个网站，那么，我们就可以跳过输入用户名、密码以及验证码的环节，直接读取浏览器中已经保存的cookie来访问网站，就可以顺利进去了。由于selenium启动某个浏览器时，它启动的是一个全新的浏览器，没有带任何cookie，所以我们必须添加一些代码，来让程序加载这些cookie。

我们分别用chrome和firefox来操作，看看对应的实际代码是什么。这里使用python实现。

一、对应chrome的实现



**[python]**[view plain](http://blog.csdn.net/onlyanyz/article/details/45888641#)[copy](http://blog.csdn.net/onlyanyz/article/details/45888641#)



- #!/usr/bin/python
- # -*- coding: utf-8 -*-
- 
- from selenium import webdriver  
- import time  
- import os  
- 
- profile_dir=r"C:\Users\yan\AppData\Local\Google\Chrome\User Data"# 对应你的chrome的用户数据存放路径
- chrome_options=webdriver.ChromeOptions()  
- chrome_options.add_argument("user-data-dir="+os.path.abspath(profile_dir))  
- 
- browser=webdriver.Chrome(chrome_options=chrome_options)  
- browser.maximize_window()  
- browser.get("http://www.baidu.com")  

以上代码即可实现打开chrome时载入cookie来执行后续操作。例如，你已经登录了百度，那么这时打开浏览器，就是已登录状态。否则，你就慢慢识别百度的验证码吧^_^


**[python]**[view plain](http://blog.csdn.net/onlyanyz/article/details/45888641#)[copy](http://blog.csdn.net/onlyanyz/article/details/45888641#)



- browser=webdriver.Chrome(chrome_options=chrome_options)  

这句的参数就是添加了用户数据，如果没有参数，则是启动一个全新的不带cookie的浏览器。


二、对应firefox的实现



**[python]**[view plain](http://blog.csdn.net/onlyanyz/article/details/45888641#)[copy](http://blog.csdn.net/onlyanyz/article/details/45888641#)



- #!/usr/bin/python
- # -*- coding: utf-8 -*-
- 
- from selenium import webdriver  
- import time  
- import os  
- 
- fp=webdriver.FirefoxProfile(r'C:\Users\yan\AppData\Roaming\Mozilla\Firefox\Profiles\btnc8mzb.default')  
- browser=webdriver.Firefox(fp)  
- browser.maximize_window()  
- browser.get("http://www.baidu.com")  

与上面类似。


