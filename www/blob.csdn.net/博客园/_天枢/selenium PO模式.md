# selenium PO模式 - _天枢 - 博客园
## [selenium PO模式](https://www.cnblogs.com/yhleng/p/8057423.html)
2017-12-18 14:24 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8057423)
思想:
1.定义basepage.py用来写公共方法,比如找元素,打开url,切换frame.这样的部分都写在这里.不必每次用都重写.
2.LoginPage.py 每个功能模块一个文件或者一个类
   这都是可以的,将每个动作封装成一个个的小组件.
3.这里用unittest框架来测试.
下面,以安居客登录为例
1.公用功能部分 basepage.py
```
#coding:utf-8
from selenium import webdriver
from selenium.webdriver.support.wait import WebDriverWait
from selenium.common.exceptions import NoSuchElementException
'''
公共方法封装:
1.打开浏览器
2.找元素
3.switch_to
4.send_keys
'''
class Action(object):
    def __init__(self,p_url,s_driver,p_title):
        self.base_url = p_url
        self.base_driver = s_driver
        self.page_title = p_title
    #打开浏览器,并最大化
    def page_open(self,url,title):
        self.base_driver.get(url)
        self.base_driver.maximize_window()
        assert title in self.base_driver.title
    #找元素,并加入
    def findElement(self,*loc):
        try:
            WebDriverWait(self.base_driver,5,0.5).until(lambda driver: self.base_driver.find_element(*loc).is_displayed())
            return self.base_driver.find_element(*loc)
        except:
            print(u'%s未能找到页面%s元素'%(self,loc))
    #切换到frame
    def switch_to_frame(self,doc):
        self.base_driver.switch_to_frame(doc)
    #跳出frame
    def default_switch(self):
        self.base_driver.switch_to.default_content()
    #重写sendkeys
    def send_keys(self,doc,value,clear=True,click=False):
        #loc = getattr(self,'_%s'%doc)
        element = self.findElement(doc)
        if clear: element.clear()
        if click: element.click()
        element.send_keys(value)
```
2.功能模块动作封装anjk_Login.py
每个操作封装成一个个小组件.
这里需要注意的是
self.findElement(*self.pwtab_loc).click()
参数中,带*号,为什么要带星号,因为我们参数是一个元组.中间有By.ID这样的对像,如果不加会有问题.
```
#coding:utf-8
from selenium.webdriver.common.by import By
from scripts import basepage
from selenium import webdriver
class LoginPage(basepage.Action):
    #定位元素
    username_loc = (By.ID,"pwdUserNameIpt")
    password_loc = (By.ID,"pwdIpt")
    pwtab_loc = (By.ID,"pwdTab")
    btn_loc = (By.ID,"pwdSubmitBtn")
    switchFrame = 0
    #打开页面
    def open_url(self):
        self.page_open(self.base_url,self.page_title)
    #切换frame
    def switch_to_f(self):
        self.switch_to_frame(self.switchFrame)
    #切换登录方式
    def switch_login(self):
        self.findElement(*self.pwtab_loc).click()
    #输入用户名
    def inputUserName(self,username):
        self.findElement(*self.username_loc).send_keys(username)
    #输入pndc
    def inputPassWord(self,password):
        self.findElement(*self.password_loc).send_keys(password)
    #单击登录按钮
    def clickLogin(self):
        self.findElement(*self.btn_loc).click()
if __name__=="__main__":
        driver = webdriver.Firefox()
        driver.implicitly_wait(30)
        url = 'https://login.anjuke.com/login/form?history=aHR0cHM6Ly9zaGFuZ2hhaS5hbmp1a2UuY29tLw=='
        username ='123'
        password=''
        Login_page = LoginPage(url,driver,u'登录')
        Login_page.open_url()
        Login_page.switch_to_f()
        Login_page.switch_login()
        Login_page.inputUserName(username)
        Login_page.inputPassWord(password)
        Login_page.clickLogin()
```
3.unittest  LoginTest.py
```
#coding:utf-8
import unittest
from page import anjk_login
from selenium import webdriver
class case_login_ank(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.driver = webdriver.Firefox()
        cls.driver.implicitly_wait(30)
        cls.url = 'https://login.anjuke.com/login/form?history=aHR0cHM6Ly9zaGFuZ2hhaS5hbmp1a2UuY29tLw=='
        cls.username ='123'
        cls.password=''
    def test_case_login(self):
        Login_Page = anjk_login.LoginPage(self.url,self.driver,u'登录')
        Login_Page.open_url()
        Login_Page.switch_to_f()
        Login_Page.switch_login()
        Login_Page.inputUserName(self.username)
        Login_Page.inputPassWord(self.password)
        Login_Page.clickLogin()
    @classmethod
    def tearDownClass(cls):
        cls.driver.quit()
if __name__=="__main__":
    unittest.main()
```
![](https://images2017.cnblogs.com/blog/1149221/201712/1149221-20171218142412256-409158141.png)
