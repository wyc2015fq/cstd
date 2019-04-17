# 双十一用python秒杀京东好货! - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





好久没用python了，都写不来了。

需要用到selenium 和 Chromedriver；

我只是记录一下几个坑；

第一个坑：自己电脑里安装了两个版本的python ，3.5和3.6 结果我在pip selenium的时候自动安装在了3.5上面，而我编译器里面运行的还是3.6结果提示找不到对应的库，然后我就很纳闷了，还好以前有过相似的库找不到的现象，于是去setting里面找project interpreter 里面的包，遂找到原因。

第二个坑：貌似还要pip 一下 chromedriver ，除此之外我还遇到了版本问题，一开始找的driver版本太低了。

第三个坑：要将driver.exe直接放在c盘的目录下，你藏的深了不知道为什么找不到了。

```
# _*_ coding:utf-8_*_

from selenium import webdriver
import datetime
import time

print("ready to loading...")
# driver=webdriver.Chrome('D:\geckodriver\chromedriver.exe')
#driver=webdriver.Firefox(executable_path = 'D:\geckodriver\geckodriver.exe')
driver=webdriver.Chrome('C:\chromedriver.exe')
print("start webdriver")
def login(uname, passwd):
    #driver = webdriver.
    print("start process")
    driver.get("http://www.jd.com")
    print("get web in")
    time.sleep(3)
    driver.find_element_by_link_text("你好，请登录").click()
    time.sleep(3)
    driver.find_element_by_link_text("账户登录").click()
    time.sleep(3)
    print("已登陆")
    driver.find_element_by_name("loginname").send_keys(uname)
    driver.find_element_by_name("nloginpwd").send_keys(passwd)
    driver.find_element_by_id("loginsubmit").click()
    time.sleep(3)
    driver.get("https://item.m.jd.com/product/3243688.html")
    time.sleep(3)

    buy_time = '2017-11-11 10:00:00'
    print("buy_time :" + buy_time + "等待时间到达")
    while True:
        now = datetime.datetime.now()
        if now.strftime('%Y-%m-%d %H:%M:%S') == buy_time:
            driver.find_element_by_id("choose-btn-ko").click()
            time.sleep(3)
            driver.find_element_by_link_text("保存收货人信息").click()
            time.sleep(3)
            driver.find_element_by_id("order-submit").click()
            print("已提交订单")
        time.sleep(0.5)

# entrance
login('账号','密码')   # your username and password

driver.close()
```

没有仔细的学习。。。











