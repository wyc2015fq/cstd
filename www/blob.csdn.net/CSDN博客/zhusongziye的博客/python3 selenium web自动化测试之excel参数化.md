# python3 selenium web自动化测试之excel参数化 - zhusongziye的博客 - CSDN博客





2017年09月06日 21:12:25[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1014








简单来说，就是不把测试的数据（或者说用例的参数）直接写在代码里，而是写在excel中，代码从excel表格中读取出来，做到数据分离。简单举个例子，上代码：



```python
#coding=utf-8
from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
import time
import xlrd

def open_excel(file = 'login.xlsx'):
    try:
        data = xlrd.open_workbook(file)  # 打开Excel文件读取数据
        return data
    except Exception, e:
        print str(e)

# 根据索引获取Excel表格中的数据 参数:file：Excel文件路径 colnameindex：表头列名所在行的所以 ，by_index：表的索引
def excel_table_byindex(file = 'login.xlsx',colnameindex=0,by_index=0):
    data = open_excel(file)
    table = data.sheets()[by_index]  # 通过索引顺序获取获取一个工作表
    nrows = table.nrows  # 获取行数
    colnames = table.row_values(colnameindex)  # 获取某一行数据
    list =[]
    for rownum in range(1, nrows):
        row = table.row_values(rownum)
        if row:
            app = {}
            for i in range(len(colnames)):
                app[colnames[i]] = row[i]
                list.append(app)
    return list
def Login():
    listdata = excel_table_byindex("D:\\login.xlsx", 0)
    # listdata = excel_table_byindex("login.xlsx", 0)
    if(len(listdata) <= 0):
        assert 0, u"Excel数据异常"

    for i in range(1, len(listdata)):
        driver = webdriver.Firefox()
        driver.get("http://www.baidu.com")
        assert u"百度一下，你就知道" in driver.title
        # 点击登录按钮
        driver.find_element_by_xpath(".//*[@id='u1']/a[7]").click()
        time.sleep(3)
        url = driver.current_window_handle
        driver.switch_to.window(url)
        time.sleep(3)
        driver.find_element_by_name("userName").clear()
        driver.find_element_by_name("userName").send_keys(listdata[i]['passname'])
        driver.find_element_by_name("password").clear()
        driver.find_element_by_name("password").send_keys(listdata[i]['password'])
        driver.find_element_by_name("memberPass").click()
        time.sleep(15)
        driver.find_element_by_id("TANGRAM__PSP_8__submit").click()

        # driver.find_element_by_id('passname').send_keys(listdata[i]['passname'])
        # driver.find_element_by_id('password').send_keys(listdata[i]['password'])
        # driver.find_element_by_xpath("//div[2]/div[2]/div/div[6]/input").click()
        time.sleep(5)
        try:
            elem = driver.find_element_by_id("kw")
        except NoSuchElementException:
            assert 0, u"登录失败，找不到右上角头像"
        driver.close()

if __name__ == '__main__':
    Login()
```


表格中的数据为：

![](https://img-blog.csdn.net/20170906211722281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


小编不保证你拷贝下来就能跑通过，因为环境有可能不一样。所以，请看明白后举一反三。谢谢您的浏览。



