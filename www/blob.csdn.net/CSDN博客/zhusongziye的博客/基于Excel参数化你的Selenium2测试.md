# 基于Excel参数化你的Selenium2测试 - zhusongziye的博客 - CSDN博客





2018年04月26日 21:42:30[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：197








## 前言

今天我们就如何使用xlrd模块来进行python selenium2 + excel自动化测试过程中的参数化进行演示说明，以解决大家在自动化测试实践过程中参数化的疑问。

## 环境安装

xlrd是python用于读取excel的第三方扩展包，因此在使用xlrd前，需要使用以下命令来安装xlrd。
`pip install xlrd`
## xlrd基本用法
- 
导入扩展包

`import xlrd`- 
打开excel文件

`excel = xlrd.open_workbook(u'excelFile.xls')`- 
获取工作表


```
# 通过索引顺序获取
table = excel.sheets()[0]
table = excel.sheet_by_index(0)

# 通过工作表名获取
table = excel.sheet_by_name(u'Sheet1')
```
- 
获取行数和列数


```
# 获取行数
nrows = table.nrows

# 获取列数
ncols = table.ncols
```
- 
获取整行或整列的值


```
# 其中i为行号， j为列号# 行号、列号索引从0开始

row_values = table.row_values(i)

col_values = table.col_values(j)
```
- 
获取指定单元格数据


```
# i-行号， j-列号
value = table.cell(i, j).value

# 例如获取第一行、第一列的数据
value = table.cell(0, 0).value
```
- 
循环行遍历列表数据


```
# 先获取行数
nrows = table.nrows

# 遍历打印所有行数据
for i in range(0, nrows):
    print table.row_values(i)
```

至此我们将xlrd基本常用的技巧和方法都一一列举完毕，下面我们一起看一下如何利用xlrd来实现python selenium2自动化测试参数化。

## 代码示例

我们以上一章我们的第一个python selenium2测试代码为蓝本，进行改造，从excel中读取以下格式的数据来进行测试， 请将下列表格数据存入名为baidu_search.xlsx的excel文件。
|序号|搜索词|期望结果|
|----|----|----|
|1|开源优测|开源优测_百度搜索|
|2|别啊|别啊_百度搜索|
|3|尼玛，能不能动手分享下？|尼玛，能不能动手分享下？_百度搜索|

```
# 将以下代码保存到first_webdriver.py中

#-*- coding:utf-8 -*-

from selenium import webdriver
import unittest
import HTMLTestRunner  import sys
from time import sleep
import xlrd

reload(sys)
sys.setdefaultencoding("utf-8")


class LoadBaiduSearchTestData:
    def __init__(self, path):
        self.path = path    

    def load_data(self):
        # 打开excel文件
        excel = xlrd.open_workbook(self.path)        
        # 获取第一个工作表
        table = excel.sheets()[0]        
        # 获取行数
        nrows = table.nrows   
         
        # 从第二行开始遍历数据
        # 存入一个list中
        test_data = []       
        for i in range(1, nrows):
            test_data.append(table.row_values(i))      
  
        # 返回读取的数据列表    
        return test_data


class BaiduTest(unittest.TestCase):
    """百度首页搜索测试用例"""

    def setUp(self):
        self.driver = webdriver.Firefox()
        self.driver.implicitly_wait(30)
        self.base_url = u"http://www.baidu.com"
        self.path = u"baidu_search.xlsx"
        
    def test_baidu_search(self):
        driver = self.driver        
        print u"开始[case_0001]百度搜索"        
        
        # 加载测试数据
        test_excel = LoadBaiduSearchTestData(self.path)
        data = test_excel.load_data()        
        print data 
  
              
        # 循环参数化
        for d in data:            
            # 打开百度首页
            driver.get(self.base_url)            
            # 验证标题
            self.assertEqual(driver.title, u"百度一下，你就知道")
            sleep(1)
            
            driver.find_element_by_id("kw").clear()            
            # 参数化 搜索词
            driver.find_element_by_id("kw").send_keys(d[1])
            sleep(1)
            driver.find_element_by_id("su").click()
            sleep(1)            
            # 参数化 验证搜索结果标题
            self.assertEqual(driver.title, d[2])
            sleep(2)            
        
    def tearDown(self):
        self.driver.quit()        if __name__ == '__main__':
    testunit = unittest.TestSuite()
    testunit.addTest(BaiduTest('test_baidu_search'))    
    # 定义报告输出路径
    htmlPath = u"testReport.html"
    fp = file(htmlPath, "wb")
    
    runner = HTMLTestRunner.HTMLTestRunner(stream=fp, 
           title=u"百度测试", 
           description=u"测试用例结果")
    
    runner.run(testunit)
    
    fp.close()
```

## 总结

在上文中，我们详细的描述了xlrd操作excel的各种方法和技巧，以及封装xlrd读取excel实现在python selenium自动化测试过程参数化相应的输入数据和期望结果。

最重要的还是需要大家自己多练习相关的代码，并能做相应的扩展， 同时要去有针对性的学习对应的库，深入了解其使用方法和技巧，甚至原理。



