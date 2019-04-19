# Unittest + python - _天枢 - 博客园
## [Unittest + python](https://www.cnblogs.com/yhleng/p/7485861.html)
2017-09-06 17:13 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7485861)
Unittest简单应用
```
#_*_coding:utf8_*_
import unittest
from selenium import webdriver
import time
class Test_apiTest(unittest.TestCase):
    def setUp(self): #相当于python __init__  加载时运行  相当于构造函数
        self.driver = webdriver.Ie()
        self.url = "http://www.baidu.com"
    def tearDown(self): #相当于python __del__  结束于运行  相当于析构函数
        self.driver.quit()
    def test_searchBD(self): #测试方法，必须以test开头
        ie = self.driver.get(self.url)
        self.driver.find_element_by_id('kw').send_keys('selenium')
        self.driver.find_element_by_id('su').click()
if __name__ == '__main__':
    #unittest.main()
    suite = unittest.TestSuite() #实例一个套件
    suite.addTest(Test_apiTest().test_searchBD()) #将测试Case方法增加入套件，如果有多个Test Case可以多次增加。  
    unittest.TextTestRunner().run(suite)#执行测试   执行测试Case的顺序，跟suite.addTest顺序先后有关。
```
