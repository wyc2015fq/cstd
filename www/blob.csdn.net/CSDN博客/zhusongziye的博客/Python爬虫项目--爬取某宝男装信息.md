# Python爬虫项目--爬取某宝男装信息 - zhusongziye的博客 - CSDN博客





2018年11月24日 13:41:29[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：40标签：[Python爬虫项目--爬取某宝男装信息](https://so.csdn.net/so/search/s.do?q=Python爬虫项目--爬取某宝男装信息&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/zhusongziye/article/category/7611456)









本次爬取用到的知识点有:

1. selenium

2. pymysql

3  pyquery

```
'''
文章：Python爬虫项目--爬取某宝男装信息
作者：Star_Zhao
源自：https://www.cnblogs.com/star-zhao/p/9664564.html
'''
```

## 正文

### 1. 分析目标网站

1. 打开某宝首页, 输入"男装"后点击"搜索", 则跳转到"男装"的搜索界面.

2. 空白处"右击"再点击"检查"审查网页元素, 点击"Network".

1) 找到对应的URL, URL里的参数正是Query String Parameters的参数, 且请求方式是GET

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n6IB1kq3WiaeibK2fCI0gs6x6HEAibo860iblgCOkyFM1K6TE4mQj5pSicDIpKyXl58kPluQB17ibibdLK6w/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

　2) 我们请求该URL得到内容就是"Response"里的内容, 那么点击它来确认信息.

![](https://mmbiz.qpic.cn/mmbiz_png/Kad3LZzM7n6IB1kq3WiaeibK2fCI0gs6x6aEM2XCekS5okQ001wVAZn8MicJHt7qOlYTVv8OncLPHjUU6icOhxcmRA/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

　3) 下拉看到"男装"字样, 那么再往下找, 并没有发现有关"男装"的商品信息.

　4) 任意复制一个商品信息, 空白处右击再点击"查看网页源代码", 在源码查找该商品, 即可看到该商品的信息.

　5) 对比网页源代码和"Response"响应内容, 发现源代码<script>..........</script>中的商品信息被替换, 这便是采用了JS加密

　6) 如果去请求上面的URL, 得到的则是加密过的信息, 这时就可以利用Selenium库来模拟浏览器, 进而得到商品信息.

### 2. 获取单个商品界面

1. 请求网站

```
# -*- coding: utf-8 -*-
from selenium import webdriver   #从selenium导入浏览器驱动
browser = webdriver.Chrome()   #声明驱动对象, 即Chrome浏览器
def get_one_page():
    '''获取单个页面'''
    browser.get("https://www.xxxxx.com")  #请求网站
```

2. 输入"男装", 在输入之前, 需要判断输入框是否存在, 如果存在则输入"男装", 不存在则等待显示成功.

```
# -*- coding: utf-8 -*-
from selenium import webdriver  
from selenium.webdriver.common.by import By                       #导入元素定位方法模块
from selenium.webdriver.support.ui import WebDriverWait           #导入等待判断模块
from selenium.webdriver.support import expected_conditions as EC  #导入判断条件模块
browser = webdriver.Chrome()   
def get_one_page():
    '''获取单个页面'''
    browser.get("https://www.xxxxx.com") 
    input = WebDriverWait(browser,10).until(                       #等待判断
        EC.presence_of_element_located((By.CSS_SELECTOR,"#q")))    #若输入框显示成功,则获取,否则等待
    input.send_keys("男装")                                         #输入商品名称
```

3. 下一步就是点击"搜索"按钮, 按钮具有属性: 可点击, 那么加入判断条件.

```
# -*- coding: utf-8 -*-
from selenium import webdriver   
from selenium.webdriver.common.by import By  
from selenium.webdriver.support.ui import WebDriverWait  
from selenium.webdriver.support import expected_conditions as EC  
browser = webdriver.Chrome()   
def get_one_page():
    '''获取单个页面'''
    browser.get("https://www.xxxxx.com")  
    input = WebDriverWait(browser,10).until(                       
        EC.presence_of_element_located((By.CSS_SELECTOR,"#q")))    #
    input.send_keys("男装")   
    button = WebDriverWait(browser,10).until(                                                        #等待判断
        EC.element_to_be_clickable((By.CSS_SELECTOR,"#J_TSearchForm > div.search-button > button"))) #若按钮可点击, 则获取, 否则等待
    button.click()                                                                                   #点击按钮
```



4. 获取总的页数, 同样加入等待判断.

```
# -*- coding: utf-8 -*-

import re
from selenium import webdriver
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
browser = webdriver.Chrome()
def get_one_page():
    '''获取单个页面'''
    browser.get("https://www.xxxxx.com")
    input = WebDriverWait(browser, 10).until(
        EC.presence_of_element_located((By.CSS_SELECTOR, "#q")))
    input.send_keys("男装")
    button = WebDriverWait(browser, 10).until(
        EC.element_to_be_clickable(
            (By.CSS_SELECTOR, "#J_TSearchForm > div.search-button > button")))
    button.click()  
    pages = WebDriverWait(browser, 10).until(  # 等待判断
        EC.presence_of_element_located(
            (By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.total")))  # 若总页数加载成功,则获取总页数,否则等待 
    return pages.text
def main():
    pages = get_one_page()
    print(pages)
if __name__ == '__main__':
    main()
```



5. 打印出来的不是我们想要的结果, 利用正则表达式获取, 最后再利用try...except捕捉异常

```
# -*- coding: utf-8 -*-
import re
from selenium import webdriver   
from selenium.common.exceptions import TimeoutException   
from selenium.webdriver.common.by import By  
from selenium.webdriver.support.ui import WebDriverWait  
from selenium.webdriver.support import expected_conditions as EC  
browser = webdriver.Chrome()  
def get_one_page():
    '''获取单个页面'''
    try:
        browser.get("https://www.xxxxx.com")  
        input = WebDriverWait(browser,10).until(
　　　　　　EC.presence_of_element_located((By.CSS_SELECTOR,"#q")))    
        input.send_keys("男装")   
        button = WebDriverWait(browser,10).until(
　　　　　　EC.element_to_be_clickable((By.CSS_SELECTOR,"#J_TSearchForm > div.search-button > button"))) 
        button.click()            
        pages = WebDriverWait(browser,10).until(
　　　　　　EC.presence_of_element_located((By.CSS_SELECTOR,"#mainsrp-pager > div > div > div > div.total")))  
        return pages.text
    except TimeoutException:
        return get_one_page()   #如果超时,继续获取
def main():
    pages = get_one_page()
    pages = int(re.compile("(\d+)").findall(pages)[0]) #采用正则表达式提取文本中的总页数
    print(pages)
if __name__ == '__main__':
    main()
```

关于Selenium的更多内容,可参看官方文档https://selenium-python.readthedocs.io/waits.html

### 3. 获取多个商品界面

采用获取"到第 页"输入框方式, 切换到下一页, 同样是等待判断

需要注意的是, 最后要加入判断: 高亮是否是当前页  

```
def get_next_page(page):
    try:
        input = WebDriverWait(browser, 10).until(                                                                                         
            EC.presence_of_element_located((By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.form > input")))                       # 若输入框加载成功,则获取,否则等待
        input.send_keys(page)                                                                                                               # 输入页码
        button = WebDriverWait(browser, 10).until(                                                                                       
            EC.element_to_be_clickable((By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.form > span.btn.J_Submit")))               # 若按钮可点击,则获取,否则等待
        button.click()                                                                                                                      # 点击按钮
        WebDriverWait(browser,10).until(
            EC.text_to_be_present_in_element((By.CSS_SELECTOR,"#mainsrp-pager > div > div > div > ul > li.item.active > span"),str(page)))  # 判断高亮是否是当前页
    except TimeoutException:                                                                                                                # 超时, 继续请求
        return get_next_page(page)
def main():
    pages = get_one_page()
    pages = int(re.compile("(\d+)").findall(pages)[0])
    for page in range(1,pages+1):
        get_next_page(page)
if __name__ == '__main__':
    main()
```

### 4. 获取商品信息

首先, 判断信息是否加载成功, 紧接着获取源码并初始化, 进而解析.

需要注意的是, 在"get_one_page"和"get_next_page"中调用之后, 才可执行

```
def get_info():
    """获取详情"""
    WebDriverWait(browser,20).until(EC.presence_of_element_located((
        By.CSS_SELECTOR,"#mainsrp-itemlist .items .item")))                #判断商品信息是否加载成功
    text = browser.page_source                                             #获取网页源码
    html = pq(text)                                                        #初始化网页源码
    items = html('#mainsrp-itemlist .items .item').items()                 #采用items方法会得到生成器
    for item in items:                                                     #遍历每个节点对象
        data = []
        image = item.find(".pic .img").attr("data-src")                    #用find方法查找子孙节点,用attr方法获取属性名称
        price = item.find(".price").text().strip().replace("\n","")        #用text方法获取文本,strip()去掉前后字符串,默认是空格
        deal = item.find(".deal-cnt").text()[:-2]
        title = item.find(".title").text().strip()
        shop = item.find(".shop").text().strip()
        location = item.find(".location").text()
        data.append([shop, location, title, price, deal, image])
        print(data)
```

### 5. 保存到MySQL数据库

```
def save_to_mysql(data):
    """存储到数据库"""
    # 创建数据库连接对象
    db= pymysql.connect(host = "localhost",user = "root",password = "password",port = 3306, db = "spiders",charset = "utf8")
    # 获取游标
    cursor = db.cursor()
    #创建数据库
    cursor.execute("CREATE TABLE IF NOT EXISTS {0}(shop VARCHAR(20),location VARCHAR(10),title VARCHAR(255),price VARCHAR(20),deal VARCHAR(20), image VARCHAR(255))".format("男装"))
    #SQL语句
    sql = "INSERT INTO {0} values(%s,%s,%s,%s,%s,%s)".format("男装")
    try:
            #传入参数sql,data
        if cursor.execute(sql,data):
            #插入数据库
            db.commit()
            print("********已入库**********")
    except:
        print("#########入库失败#########")
        #回滚,相当什么都没做
        db.rollback()
    #关闭数据库
    db.close()
```

## 完整代码

```
# -*- coding: utf-8 -*-
'''
有需要Python学习资料的小伙伴吗?小编整理一套Python资料和PDF，感兴趣者可以加学习群：548377875，反正闲着也是闲着呢，不如学点东西啦~~
'''
import re
import pymysql
from selenium import webdriver   
from selenium.common.exceptions import TimeoutException
from selenium.webdriver.common.by import By  
from selenium.webdriver.support.ui import WebDriverWait  
from selenium.webdriver.support import expected_conditions as EC  
from pyquery import PyQuery as pq
browser = webdriver.Chrome()  
def get_one_page(name):
    '''获取单个页面'''
    print("-----------------------------------------------获取第一页-------------------------------------------------------")
    try:
        browser.get("https://www.xxxxx.com")  
        input = WebDriverWait(browser,10).until(                       
             EC.presence_of_element_located((By.CSS_SELECTOR,"#q")))   
        input.send_keys(name)   
        button = WebDriverWait(browser,10).until(                                                        
             EC.element_to_be_clickable((By.CSS_SELECTOR,"#J_TSearchForm > div.search-button > button"))) 
        button.click()            
        pages = WebDriverWait(browser,10).until(                                                           
          EC.presence_of_element_located((By.CSS_SELECTOR,"#mainsrp-pager > div > div > div > div.total")))  
        print("----即将解析第一页信息----")
        get_info(name)
        print("----第一页信息解析完成----")
        return pages.text
    except TimeoutException:
        return get_one_page(name)   
def get_next_page(page,name):
     """获取下一页"""
     print("---------------------------------------------------正在获取第{0}页----------------------------------------".format(page))
     try:
        input = WebDriverWait(browser, 10).until(
             EC.presence_of_element_located((By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.form > input")))                       
        input.send_keys(page)                                                                                                               
        button = WebDriverWait(browser, 10).until(
            EC.element_to_be_clickable((By.CSS_SELECTOR, "#mainsrp-pager > div > div > div > div.form > span.btn.J_Submit")))               
        button.click()                                                                                                                      
        WebDriverWait(browser,10).until(
             EC.text_to_be_present_in_element((By.CSS_SELECTOR,"#mainsrp-pager > div > div > div > ul > li.item.active > span"),str(page)))  
        print("-----即将解析第{0}页信息-----".format(page))
        get_info(name)
        print("-----第{0}页信息解析完成-----".format(page))
     except TimeoutException:                                                                                                                
        return get_next_page(page,name)
def get_info(name):
    """获取详情""" 
    WebDriverWait(browser,20).until(EC.presence_of_element_located((
        By.CSS_SELECTOR,"#mainsrp-itemlist .items .item")))                
    text = browser.page_source                                             
    html = pq(text)                                                        
    items = html('#mainsrp-itemlist .items .item').items()                 
    for item in items:                                                     
        data = []
        image = item.find(".pic .img").attr("data-src")                    
        price = item.find(".price").text().strip().replace("\n","")        
        deal = item.find(".deal-cnt").text()[:-2]
        title = item.find(".title").text().strip()
        shop = item.find(".shop").text().strip()
        location = item.find(".location").text()
        data.append([shop, location, title, price, deal, image])
        for dt in data:
            save_to_mysql(dt,name)
def save_to_mysql(data,name):
    """存储到数据库"""
    db= pymysql.connect(host = "localhost",user = "root",password = "password",port = 3306, db = "spiders",charset = "utf8")
    cursor = db.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS {0}(shop VARCHAR(20),location VARCHAR(10),title VARCHAR(255),price VARCHAR(20),deal VARCHAR(20), image VARCHAR(255))".format(name))
    sql = "INSERT INTO {0} values(%s,%s,%s,%s,%s,%s)".format(name)
    try:
        if cursor.execute(sql,data):
            db.commit()
         print("********已入库**********")
    except:
        print("#########入库失败#########")
        db.rollback()
    db.close()
def main(name):
    pages = get_one_page(name)
    pages = int(re.compile("(\d+)").findall(pages)[0])
    for page in range(1,pages+1):
       get_next_page(page,name)
if __name__ == '__main__':
    name = "男装"
    main(name)
```



