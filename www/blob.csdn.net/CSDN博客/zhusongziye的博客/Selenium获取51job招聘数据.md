# Selenium获取51job招聘数据 - zhusongziye的博客 - CSDN博客





2018年10月30日 20:07:49[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：141








在这里，Selenium结合lxml来获取51job招聘网站西安地区自动化测试招聘的公司名称，薪资范围，职位要求和招聘的Title，具体实现的思路是访问为：
- 
www.51job.com后，在搜索输入框输入搜索的关键字“自动化测试”，点击搜索按钮

- 
获取第一页（列表页显示的信息包含了各个公司的招聘信息）的源码，获取源码后

- 
使用lxml来对源码进行解析，获取每个公司招聘详情信息的URL，也就是链接地址

- 
然后点击跳转到每个公司招聘的详情页面，再获取详情页面的源码，再使用lxml进行解析，获取到具体招聘单位的公司名称，招聘Title，职位要求，薪资范围


如上是实现的思路，具体见实现的代码。这里Selenium版本是3.13版本，Chrome浏览器版本号是68，Python使用的版本是Python3.6的版本。

     首先导入需要的库，见源码：

```python
from selenium import webdriver
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from lxml import etree
import requests
import time as t
import re
import csv
```

然后定义Job类，编写构造函数，以及打开www.51job.com后，关键字搜索，获取到第一页的列表源码并且解析，获取每个公司招聘的详情页面的链接地址，见实现的源码：

```python
class Job(object):
    '''selenium结合网络爬虫获取5job西安地区招聘自动化测试工程师的薪资和要求'''
    def __init__(self):
        self.info=[]
        self.driver=webdriver.Chrome()
        self.url='http://www.51job.com'

    def parse_list_page(self):
        self.driver.implicitly_wait(30)
        self.driver.maximize_window
        self.driver.get(self.url)
        #输入搜索的关键字
    self.driver.find_element_by_id('kwdselectid').send_keys('自动化测试工程师')
        #点击搜索按钮
    self.driver.find_element_by_xpath('/html/body/div[3]/div/div[1]/div/button').click()
        source=self.driver.page_source
        #对搜索后的源码进行解析
    html=etree.HTML(source)
        #获取所有招聘信息的详情URL
        links=html.xpath('//div[@class="dw_table"]/div[@class="el"]//span[not(@class="t2")]/a/@href')
        for link in links:
            t.sleep(3)
            #对详情页面的数据进行分析
         self.request_detail_page(link)
```

然后编写方法request_detail_page解析每个公司招聘详情页面的数据，这地方同时涉及到多窗口的操作（这部分的知识不做解释，如有疑问，可查看本人写的Selenium的文章），在方法requets_detail_page中，点击每个详情的链接地址跳转到详情页，见源码：

```python
def request_detail_page(self,url):
    '''在列表页点击跳转到详情页面'''
    #切换窗口
  self.driver.execute_script("window.open('%s')"%url)
    #切换到详情页
  self.driver.switch_to_window(self.driver.window_handles[1])
    #获取详情页的页面资源
  source=self.driver.page_source
    #解析详情页面后关闭页面
  self.driver.close()
    #继续切换到列表页
  self.driver.switch_to_window(self.driver.window_handles[0])
    self.parse_detail_page(source)
```

然后是方法parse_detail_page，该方法是指到详情页面后，获取详情的源码，对源码进行解析并且获取到招聘单位的公司名称，招聘Title，职位要求，薪资范围，并且把这些数据放到一个字典中，见parse_detail_page方法的源码：

```python
def parse_detail_page(self,source):
    '''对招聘详情页的数据进行解析'''
    #对详情页的数据进行分析
  html=etree.HTML(source)
    #获取公司基本信息
  infos=html.xpath('//div[@class="cn"]')
    for info in infos:
        #获取公司名称
    companyName=info.xpath('//p[@class="cname"]/a/@title')[0].strip()
        #获取招聘title
        title=info.xpath('./h1/text()')[0].strip()
        #获取招聘薪资
   salary=info.xpath('./strong/text()')[0]
        #获取职位信息
    position=html.xpath('//div[@class="bmsg job_msg inbox"]/p/text()')
        position=''.join(position).strip()
    jobInfo={
        '公司名称':companyName,
        '招聘职位':title,
        '薪资范围':salary,
        '职位信息':position
    }
    print(jobInfo)
```

最后写一个方法run，在该方法里面调用方法parse_list_page的方法就可以了，见完整的源码：

```python
from selenium import webdriver
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.webdriver.common.by import By
from lxml import etree
import requests
import time as t
import re
import csv

class Job(object):
    '''selenium结合网络爬虫获取5job西安地区招聘自动化测试工程师的薪资和要求'''
    def __init__(self):
        self.info=[]
        self.driver=webdriver.Chrome()
        self.url='http://www.51job.com'


    def run(self):
        self.parse_list_page()
        self.driver.quit()

    def parse_list_page(self):
        self.driver.implicitly_wait(30)
        self.driver.maximize_window
        self.driver.get(self.url)
        #输入搜索的关键字
    self.driver.find_element_by_id('kwdselectid').send_keys('自动化测试工程师')
        #点击搜索按钮
    self.driver.find_element_by_xpath('/html/body/div[3]/div/div[1]/div/button').click()
        source=self.driver.page_source
        #对搜索后的源码进行解析
    html=etree.HTML(source)
        #获取所有招聘信息的详情URL
        links=html.xpath('//div[@class="dw_table"]/div[@class="el"]//span[not(@class="t2")]/a/@href')
        for link in links:
            t.sleep(3)
            #对详情页面的数据进行分析
     self.request_detail_page(link)

    def request_detail_page(self,url):
        '''在列表页点击跳转到详情页面'''
        #切换窗口
    self.driver.execute_script("window.open('%s')"%url)
        #切换到详情页
    self.driver.switch_to_window(self.driver.window_handles[1])
        #获取详情页的页面资源
    source=self.driver.page_source
        #解析详情页面后关闭页面
    self.driver.close()
        #继续切换到列表页
    self.driver.switch_to_window(self.driver.window_handles[0])
        self.parse_detail_page(source)

    def parse_detail_page(self,source):
        '''对招聘详情页的数据进行解析'''
        #对详情页的数据进行分析
    html=etree.HTML(source)
        #获取公司基本信息
    infos=html.xpath('//div[@class="cn"]')
        for info in infos:
            #获取公司名称
     companyName=info.xpath('//p[@class="cname"]/a/@title')[0].strip()
            #获取招聘title
            title=info.xpath('./h1/text()')[0].strip()
            #获取招聘薪资
     salary=info.xpath('./strong/text()')[0]
        #获取职位信息
    position=html.xpath('//div[@class="bmsg job_msg inbox"]/p/text()')
        position=''.join(position).strip()
        jobInfo={
            '公司名称':companyName,
            '招聘职位':title,
            '薪资范围':salary,
            '职位信息':position
        }
        print(jobInfo)
if __name__ == '__main__':
    job=Job()
    job.run()
```

执行如上的代码后，就会获取到最初设计的数据，这里对这些数据就不显示了，实在是数据太多。后期可以对薪资范围这部分进行数据分析。



