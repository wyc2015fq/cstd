# python3+requests+unittest接口自动化实例讲解 - zhusongziye的博客 - CSDN博客





2018年12月08日 14:18:08[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：198








**前言**

这个季度初的时候，我们公司管理层提出了全面提升部门技术水准的要求，而作为项目开发流程中最重要的一环——测试，我们也是有自己的技术追求的，所以为了，由原来的业务功能测试转变成自动化测试。之前大多都是手工进行，利用postman和fiddler进行抓包和截取数据，后来，经过大家的讨论，决定基于python学习，开始搞接口自动化，经过大家的努力，这套自动化框架目前已经基本完成了，于是进行一些总结，便于以后回顾温习，有许多不完善的地方，大家可以留言一起讨论研究，共同进步。

**首先，我们可以捋一捋思路，想一想接口测试的流程**



![](https://user-gold-cdn.xitu.io/2018/12/3/16773c00fc4651dd?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)



**其次，选择合适的框架**

流程清晰之后，我们需要选择一个合适的框架，于是选择了Python3+requests+unittest框架

Requests模块发送http的网络请求，请求类型主要包含了post，get， PUT，DELETE，HEAD

python+unittest单元测试框架构成，和测试报告生成（HTMLTestRunner）

框架详解不在此赘述，requests可参考 [点击前往](https://link.juejin.im/?target=http%3A%2F%2Fdocs.python-requests.org%2Fzh_CN%2Flatest%2Fuser%2Fquickstart.html)

unittest参考 [点击前往](https://link.juejin.im/?target=https%3A%2F%2Fwww.jianshu.com%2Fp%2Fa73a9b68abbf)

基于以上，我们来一步步搭建我们的框架。在这个过程中，我们需要做到业务和数据的分离，这样才能灵活，达到我们写框架的目的。接下来，我们来进行结构的划分。

我们的结构是这样的

![](https://user-gold-cdn.xitu.io/2018/12/3/16773c0cecf21101?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

data：存放自动化测试所用到的数据文档

log：存放生成的日志文件

base：存放公共的方法

report：存放生成的自动化测试报告

testcase：存放测试脚本

**接下来，公共方法的开发**

整体结构有了划分，接下来就该一步步的填充整个框架了，我们先来看看Base文件中的公共类或函数，这主要用于后续测试case的调用，所有公共的、一成不变的数据都可以放在这里，维护也方便

配置文档如下

```
[DATABASE]
data_address = ./data/
report_address = ./report/

[HTTP]
base_url = http://xxx.xx

复制代码复制代码
```

想知道怎样从配置文档中得到或写入相应的数据吗？那继续看吧

```
import os
import configparser

# 获取当前py文件地址
proDir = os.path.split(os.path.realpath(__file__))[0]
# 组合config文件地址
configPath = os.path.join(proDir,"config.ini")

class ReadConfig:
    def __init__(self):
        #获取当前路径下的配置文件
        self.cf = configparser.ConfigParser()
        self.cf.read(configPath)

    def get_config(self,field,key):
        #获取配置文件中的key值
        result = self.cf.get(field,key)
        return result

    def set_config(self,field,key,value):
        #向配置文件中写入配置信息
        fb = open(configPath,'w')
        self.cf.set(field,key,value)
        self.cf.write(fb)

复制代码复制代码
```

那问题又来了，我们的测试数据放在哪里？怎么取值？怎么写入？怎么保存？。。。

别急，接着往下看

测试数据优先考虑放在excel或database中，此处以excel为例做个简单介绍

这里需要用到两个操作表格的库，xlrd数据驱动的读取，作用于excel文档，但xlrd不能写入数据，所以引入xlutils数据驱动的读取和写入

安装方法可以用pip3 install xlrd和pip3 install xlutils 来安装。

```
import xlrd
import xlutils.copy
from Base.readConfig import ReadConfig
import time

class ReadExcel:

    def __init__(self,section,field,sheet):
        # 打开工作表，并定位到sheet
        data_address = ReadConfig().get_config(section,field)
        workbook = xlrd.open_workbook(data_address)
        self.table = workbook.sheets()[sheet]


    def get_rows(self):
        # 获取excel行数
        rows = self.table.nrows
        return rows

    def get_cell(self,row,col):
        # 获取单元格数据
        cell_data = self.table.cell(row,col).value
        return cell_data

    def get_col(self,col):
        # 获取整列数据
        col_data = self.table.col_value(col)
        return col_data

class WriteExcel:
    def __init__(self,section,field,sheet):
        # 打开工作表
        self.address = ReadConfig().get_config(section,field)
        self.workbook = xlrd.open_workbook(self.address)
        self.wf = xlutils.copy.copy(self.workbook)
        self.ws = self.wf.get_sheet(sheet)

    def set_cell(self,row,col,value):
        #设置单元格数据
        self.ws.write(row,col,value)

    def save_excel(self,filename,format):
        #获取当前时间
        self.time = time.strftime("%Y%m%d%H%M%S", time.localtime())
        #生成文件的文件名及格式
        self.report = filename + '_' +self.time + format
        #保存文件
        self.wf.save(self.report)


复制代码复制代码
```

**然后，测试脚本的编辑**

一切准备就绪，下面通过一个简单的、完整的代码进行演示公共函数的调用，框架的使用及报告的生成

```
import unittest
import requests
from Base.readConfig import ReadConfig
from Base.readExcel import ReadExcel
from Base.readExcel import WriteExcel
#实例化
readexcel = ReadExcel('DATABASE','data_address',0)
writeexcel = WriteExcel('DATABASE','data_address',0)

class testcase(unittest.TestCase):
    #初始化
    def setUp(self):
        #获取url
        self.base_url = ReadConfig().get_config('HTTP', 'base_url')
        self.url = self.base_url + readexcel.get_cell(1,1)
        #获取请求头
        self.headers = readexcel.get_cell(1,4)

    def test_case(self):
        nok = 0
        ner = 0
        # 循环读取excel中的测试数据，进行结果验证，并生成excel形式的测试报告
        for i in range(3,readexcel.get_rows()):
            #发送网络请求，得到响应值
            response = requests.post(self.url, headers=self.headers,   data=readexcel.get_cell(i,4).encode('utf-8'))
            actualresult = response.json()
            #获取excel中的预期结果
            expectresult = eval(readexcel.get_cell(i,6))
            # 获取需验证的数据
            key = eval(readexcel.get_cell(i, 5))
            keylen = len(key)
            j = 0
            for k in range(keylen):
                aresult = 'actualresult' + key[k]
                eresult = 'expectresult' + key[k]
                if eval(aresult) == eval(eresult):
                    #预期结果和实际结果一致
                    j = j + 1
            if j == keylen:
                #测试数据执行通过
                nok = nok + 1
                writeexcel.set_cell(i, 8, 'SUCCESS')
            else:
                # 测试数据执行失败，并将实际结果写入excel
                ner = ner + 1
                writeexcel.set_cell(i, 8, 'FAILURE')
                writeexcel.set_cell(i, 7, str(actualresult))
                print('第', i + 1, '行用例执行失败：预期结果是', expectresult, '实际结果是', actualresult)
            # 保存测试报告
            writeexcel.save_excel('testreport', '.xls')
            print('测试数据中总共', nok, '条用例执行通过', ner, '条用例执行失败')

    #释放资源
    def tearDown(self):
        pass

if __name__ == '__main__':
    #构造测试集合
    suite = unittest.TestSuite()
    suite.addTest(testcase('test_case'))
    #创建html文件
    filename = ReadConfig().get_config('DATABASE', 'report_address') + 'testreport.html'
    fb = open(filename,"wb")
    #执行测试并生成html测试报告
    runner = HTMLTestRunner.HTMLTestRunner(stream = fb,description = '针对接口的描述信息',title = '某某的自动化测试报告')
    runner.run(suite)
    #关闭文件
    fb.close()

复制代码复制代码
```

**最后，生成的html报告**



![](https://user-gold-cdn.xitu.io/2018/12/3/16773c1536b08d46?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)



以上是最近总结的部分成果，希望对大家有帮助，后续会陆续更新，也请大家多多留言，互相交流互相成长





作者：年糕妈妈技术团队

链接：https://juejin.im/post/5c050dffe51d45636350f1a0

来源：掘金

著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



