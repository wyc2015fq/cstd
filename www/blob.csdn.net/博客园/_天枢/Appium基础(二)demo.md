# Appium基础(二)demo - _天枢 - 博客园
## [Appium基础(二)demo](https://www.cnblogs.com/yhleng/p/7515976.html)
2017-09-13 17:08 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7515976)
具体配置，请看上一篇
一、启动Android模拟 Start  
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170913164210266-287117661.png)
打开如下界面：这个过程可能会有些慢，黑屏等等，请耐心等待
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170913164629172-35926589.png)
**二、appium配置**
**1、点机器人图标**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170913165131078-314116947.png)
PlatformVersion 选19，19就是你SDK安装时的版本。
2、点右侧绿三角，启动appium
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170913165327360-296923413.png)
出现如上界面就成功了，启动有些慢
3、python代码
操作计算器输入137+865＝
```
#*_*coding:utf-8*_*
from selenium import webdriver
from appium import webdriver
#appium webdriver 继承自selenium的webdriver
desired_caps = {}
desired_caps['platformName'] = 'Android' #平台名Android
desired_caps['platformVersion'] = '4.2.2' #这个是模拟器，设置的版本
desired_caps['deviceName'] = 'AndroidEM' #模拟器设置的设备名称
desired_caps['appPackage'] = 'com.android.calculator2' #计算器相对应的包名称
desired_caps['appActivity'] = '.Calculator' #计算器javapackage
driver = webdriver.Remote('http://localhost:4723/wd/hub', desired_caps) #地址不变
driver.find_element_by_name('1').click()
driver.find_element_by_name('3').click()
driver.find_element_by_name('7').click()
driver.find_element_by_name('+').click()
driver.find_element_by_name('8').click()
driver.find_element_by_name('6').click()
driver.find_element_by_name('5').click()
driver.find_element_by_name('=').click()
driver.quit()
```
 结果：
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170913170800953-1468800407.png)
