# python处理web网页cookies实现自动登录 - zhusongziye的博客 - CSDN博客





2017年09月06日 21:03:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：3251








废话少说，直接上代码。嘎嘎。。。



```python
# coding=utf-8
from selenium import webdriver
import time

# 手动登录百度并勾选“下次免登陆”，再用代码加载配置文件打开百度即可实现自动登录。
# 使用代码自动登录百度，退出后，代码再加载配置文件是不能自动登录的，必须手动登录一次并勾选上“下次免登陆”

# 配置文件地址
profile_directory = r'C:\Users\Dell\AppData\Roaming\Mozilla\Firefox\Profiles\8nq06rq1.default'
# 加载配置配置
profile = webdriver.FirefoxProfile(profile_directory)
# 启动浏览器配置
drive = webdriver.Firefox(profile)

# drive = webdriver.Firefox()
drive.get("http://www.baidu.com")
aa = drive.get_cookies()
print(aa)
time.sleep(5)

drive.find_element_by_xpath(".//*[@id='u1']/a[7]").click()
time.sleep(3)
print(11)
url = drive.current_window_handle
drive.switch_to.window(url)
time.sleep(3)
drive.find_element_by_name("userName").clear()
drive.find_element_by_name("userName").send_keys("xxxxx@163.com")
drive.find_element_by_name("password").clear()
drive.find_element_by_name("password").send_keys("xxxxx")
drive.find_element_by_name("memberPass").click()
time.sleep(15)
drive.find_element_by_id("TANGRAM__PSP_8__submit").click()

bb = drive.get_cookies()
print(bb)


# drive.find_element_by_id("kw").send_keys("selenium2")
# drive.find_element_by_id("su").click()
time.sleep(10)
drive.quit()
```





