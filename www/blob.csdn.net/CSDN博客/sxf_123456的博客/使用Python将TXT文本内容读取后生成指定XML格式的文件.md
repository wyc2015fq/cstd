# 使用Python将TXT文本内容读取后生成指定XML格式的文件 - sxf_123456的博客 - CSDN博客
2017年08月13日 16:20:43[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3529
任务说明：
- 解析服务器内存和CPU信息。格式是无后缀的文本格式。将其转换为统一XML格式。
- 源文件名：zmonitor.2017-04-06-14_28
```
172.17.8.64 saptmqas_cpu   431    CPU utilization percentage : 1%
172.17.8.95    saptmprd_cpu   426    CPU utilization percentage : 6%
172.17.8.63    saptmdev_cpu   421    CPU utilization percentage : 2.5%
172.17.8.91    saptm2prd_cpu  487    CPU utilization percentage : 24.75%
172.17.8.66    sappoqas_cpu   394    CPU utilization percentage : 1.75%
172.17.8.93    sappoprd_cpu   382    CPU utilization percentage : 1.125%
172.17.8.62    sappodev_cpu   313    CPU utilization percentage : 2.5%
172.17.8.78    sapmdmqas_cpu  450    CPU utilization percentage : 1.5%
172.17.8.79    sapmdmprd_cpu  445    CPU utilization percentage : 1%
172.17.8.72    sapmdmdev_cpu  440    CPU utilization percentage : 3.5%
172.17.8.74    sapepdev_cpu   462    CPU utilization percentage : 1%
172.17.8.65    sapemqas_cpu   413    CPU utilization percentage : 1%
172.17.8.96    sapemprd_cpu   408    CPU utilization percentage : 1%
172.17.8.61    sapemdev_cpu   403    CPU utilization percentage : 1.5%
172.17.8.83    sapeccqas_cpu  364    CPU utilization percentage : 1.5%
172.17.8.92    sapem2prd_cpu  476    CPU utilization percentage : 1%
172.17.8.89    sapeccprd_2_cpu    376    CPU utilization percentage : 6.75%
172.17.8.88    sapeccprd_1_cpu    372    CPU utilization percentage : 4.625%
172.17.8.87    sapeccprd_0_cpu    368    CPU utilization percentage : 1%
172.17.8.80    sapeccdev_cpu  336    CPU utilization percentage : 2.625%
172.17.8.76    sapbpmprd_cpu  468    CPU utilization percentage : 1.75%
172.17.8.73    sapbpmdev_cpu  457    CPU utilization percentage : 1%
172.17.8.64    saptmqas_memval    519    Disk OK - Physical memory TOTAL: 31.361GB USED: 30.801GB (98%) FREE: 0.560GB (2%)
172.17.8.95    saptmprd_memval    518    Disk OK - Physical memory TOTAL: 47.342GB USED: 46.992GB (99%) FREE: 0.350GB (1%)
172.17.8.63    saptmdev_memval    517    Disk OK - Physical memory TOTAL: 31.361GB USED: 31.088GB (99%) FREE: 0.272GB (1%)
172.17.8.91    saptm2prd_memval   516    Disk OK - Physical memory TOTAL: 47.137GB USED: 46.847GB (99%) FREE: 0.290GB (1%)
172.17.8.66    sappoqas_memval    515    Disk OK - Physical memory TOTAL: 31.361GB USED: 30.074GB (95%) FREE: 1.287GB (5%)
172.17.8.93    sappoprd_memval    514    Disk OK - Physical memory TOTAL: 31.357GB USED: 31.141GB (99%) FREE: 0.216GB (1%)
172.17.8.62    sappodev_memval    513    Disk OK - Physical memory TOTAL: 31.358GB USED: 31.051GB (99%) FREE: 0.306GB (1%)
172.17.8.78    sapmdmqas_memval   512    Disk OK - Physical memory TOTAL: 15.577GB USED: 15.325GB (98%) FREE: 0.253GB (2%)
172.17.8.79    sapmdmprd_memval   511    Disk OK - Physical memory TOTAL: 62.917GB USED: 62.499GB (99%) FREE: 0.418GB (1%)
172.17.8.72    sapmdmdev_memval   510    Disk OK - Physical memory TOTAL: 15.577GB USED: 15.421GB (98%) FREE: 0.156GB (2%)
172.17.8.74    sapepdev_memval    509    Disk OK - Physical memory TOTAL: 31.343GB USED: 28.224GB (90%) FREE: 3.119GB (10%)
172.17.8.65    sapemqas_memval    508    Disk OK - Physical memory TOTAL: 15.579GB USED: 15.364GB (98%) FREE: 0.215GB (2%)
172.17.8.96    sapemprd_memval    507    Disk OK - Physical memory TOTAL: 31.342GB USED: 30.852GB (98%) FREE: 0.490GB (2%)
172.17.8.61    sapemdev_memval    506    Disk OK - Physical memory TOTAL: 31.486GB USED: 31.166GB (98%) FREE: 0.320GB (2%)
172.17.8.92    sapem2prd_memval   505    Disk OK - Physical memory TOTAL: 35.302GB USED: 34.755GB (98%) FREE: 0.547GB (2%)
172.17.8.89    sapeccprd_2_memval 503    Disk OK - Physical memory TOTAL: 47.137GB USED: 41.424GB (87%) FREE: 5.712GB (13%)
172.17.8.83    sapeccqas_memval   504    Disk OK - Physical memory TOTAL: 15.579GB USED: 15.381GB (98%) FREE: 0.198GB (2%)
172.17.8.88    sapeccprd_1_memval 502    Disk OK - Physical memory TOTAL: 47.122GB USED: 46.764GB (99%) FREE: 0.359GB (1%)
172.17.8.87    sapeccprd_0_memval 501    Disk OK - Physical memory TOTAL: 7.688GB USED: 7.552GB (98%) FREE: 0.136GB (2%)
172.17.8.80    sapeccdev_memval   500    Disk OK - Physical memory TOTAL: 15.579GB USED: 15.283GB (98%) FREE: 0.297GB (2%)
172.17.8.76    sapbpmprd_memval   499    Disk OK - Physical memory TOTAL: 31.358GB USED: 31.117GB (99%) FREE: 0.241GB (1%)
172.17.8.73    sapbpmdev_memval   498    Disk OK - Physical memory TOTAL: 31.673GB USED: 31.031GB (97%) FREE: 0.642GB (3%)
```
- 统一的XML文件格式（示例）：
<?xml version="1.0"encoding="UTF-8"?><root><Platformdate="2017-02-28 12:00:00"><PlatformNumdescription="数据中心监控平台编号，以便识别****************************">1872_ZABBIX
      </PlatformNum><!-- 扩展属性，没有就不写，主要目的是同一个平台传送多种格式时区分 --><Moduledescription="模块编号">M1
      </Module></Platform><datasdescription="数据"><!-- 一条数据一个data节点，并告知关联的CI流水号
      ciNum 被监控服务器的ID，预留字段，未来和CMDB建立关联，目前可以空着。
      datetime 监控时间点
      host  被监控的服务器名称
      ip  ip地址
      itemName 被监控的服务器资源的属性  首期为 CPU空闲率(CPU idle time)大于等于0小于等于1，用于计算CPU利用率=(1-CPU idle time)*100%；空余内存数量(Free real memory)单位MB  和 总内存数量(Total memory)单位MB  用于计算内存利用率= (Free real memory/ Total memory) *100%
      itemValue 监控值，单位cpu为百分比，0.32实为32%； 内存是GB；实际各家的监控信息也可了解一下？
      --><dataciNum="CI201702230001"datetime="2017-02-28 12:00:00"host="lxbdb01"ip="172.32.224.73"itemName="CPU idle time"itemValue="0.3233"></data><dataciNum="CI201702230001"datetime="2017-02-28 12:00:00"host="lxbdb01"ip="172.32.224.73"itemName="Total memory"itemValue="128000000"></data><dataciNum="CI201702230001"datetime="2017-02-28 12:00:00"host="lxbdb01"ip="172.32.224.73"itemName="Free real memory"itemValue="30666000"></data><dataciNum="CI201701260002"datetime="2017-02-28 12:00:00"host="lxbdb02"ip="172.32.224.78"itemName="CPU idle time"itemValue="0.3233"></data><dataciNum="CI201701260002"datetime="2017-02-28 12:00:00"host="lxbdb02"ip="172.32.224.78"itemName="Total memory"itemValue="96000000"></data><dataciNum="CI201701260002"datetime="2017-02-28 12:00:00"host="lxbdb02"ip="172.32.224.78"itemName="Free real memory"itemValue="60222000"></data></datas></root>
需求：
- 将文本格式转换成统一的XML格式，并形成文件。
- 文件名上的日期，是监控时间点，读取后写入统一XML格式中。
思路：
- 因为统一的XML格式是一个服务器3段标签，其中重复信息比较多。所以先生成JSON格式，以后方便读取。
- 根本原因是领导还在协商使用XML还是JSON。
- 再将JSON格式的依次插入到XML中。
- 生成XML文件。
代码简述：
- 声明CLASS readFileToXML
- 初始化类（要求一个参数：文件完整路径）
- 第一个方法：getTime，根据文件名获取监控时间点。
- “2017-04-06-14_28”需要注意的地方是，最后一个“-”需要替换为空格。因为字符串不能单独修改，则转换成LIST修改后，再转格式为string
- 第二个方法：readFileToDict，解析源文件并生成JSON。
- 这里有个坑要说一下，生成XML的时候，要求值为字符串。因为牵扯计算，所以其中一个值为FLOAT格式存入JSON。但当这个float的值存入XML文件时报错。
- 所以这个坑是在XML模块中，并不是JSON的。当时好找一顿。
- 第三个方法：json2XML，将JSON格式数据按照XML统一格式转换。
- 简单xml模块使用。
- doc = Document() #创建DOM文档对象
- root = doc.createElement('root')  # 创建根元素
- doc.appendChild(root)   # 将根标签root加载到doc结构下。
- 以我的XML为例节点顺序：doc>root>Platform>PlatformNum
                                                         |               >Module
                                                         >datas>data
                                                                    > .......
- 这里的坑是如果你要在root标签下添加Platform标签则需要：root.appendChild(Platform)
- 创建节点和文本等，都是doc.createXXXX。但添加到指定节点时，则是节点元素Platform.appendChild(Module)
- 呵呵，有点啰嗦。
- 第四个方法：makeXML，生成XML文件保存。
- 没什么要说的，doc.toprettyxml(indent='')就是之前所有操作后的结果。
代码如下：
```python
#coding=utf-8  2017/4/7
__author__ = 'Asktao'
import os
from xml.dom.minidom import Document
class readFileToXML(object):
    def __init__(self, filepath):
        self.filepath = filepath                        # 完整路径
        self._path = os.path.split(filepath)[0]         # 文件的路径
        self._filename = os.path.split(filepath)[1]     # 文件的名称
        self.DATA_JSON = readFileToXML.readFileToDict(self) # JSON格式数据
        self.DATA_XML = readFileToXML.json2XML(self)        # XML格式数据
    def getTime(self):
        '''
        文件名格式：zmonitor.2017-04-06-14_28
        把文件名的中的时间信息截取出来。格式：2017-04-06 14:28:00
        '''
        getTime_str = self._filename.partition(".")[2].replace("_", ":")
        # name.partition(".")的结果为("zmonitor", ".", "2017-04-06-14_28")
        getTime_list = list(getTime_str)
        getTime_list[10] = " "      # 字符串不可以单独修改，则修改格式为列表格式，修改后再整体修改为字符串格式。
        getTime_str = "".join(getTime_list)
        return getTime_str+":00"
    def readFileToDict(self):
        '''
        解析原始文件，提取信息存为字典格式（JSON）
        '''
        JSON_dict = {"hosts": {}}      # 用于这里原始文件的数据，编排成JSON格式。
        fileUpdataTime = readFileToXML.getTime(self)     # 取文件名中包含的时间。
        JSON_dict["datetime"] = fileUpdataTime
        with open(self.filepath, "r", encoding="utf-8") as file:
            for line in file:
                line = line.replace("\n", "")
                line_item = line.split("\t")
                if line_item[1].endswith("_cpu"):
                    ip = line_item[0]                           # IP地址
                    name = line_item[1].partition('_')[0]       # 机器名
                    num3 = line_item[2]                         # 不知道做什么的三位数
                    CPU_utilization_percentage = line_item[3].partition(":")[2]     # CPU占用率 **%
                    CPU_utilization_percentage = CPU_utilization_percentage.replace("%", "")     # CPU占用率 去掉%号
                    CPU_utilization_percentage = str(float(100) - float(CPU_utilization_percentage))          # 求得CPU空闲时间
                    if not ip in JSON_dict["hosts"]:     #判断JSON_dict中索引是否存在，如不在则创建
                        JSON_dict["hosts"][ip] = {}               # 用IP做索引（用IP做一级目录）
                    JSON_dict["hosts"][ip]["ip"] = ip     # 添加机器名到JSON_dict
                    JSON_dict["hosts"][ip]["name"] = name     # 添加机器名到JSON_dict
                    JSON_dict["hosts"][ip]["num3"] = num3        # 添加不知道什么的3位数，到JSON_dict
                    # CPU占用率
                    JSON_dict["hosts"][ip]["CPU_utilization_percentage"] = CPU_utilization_percentage
                elif line_item[1].endswith("_memval"):
                    ip = line_item[0]                           # IP地址
                    num3_2 = line_item[2]                         # 不知道做什么的三位数+
                    temp_info = line_item[3].split(" ")
                    #print(temp_info)
                    Physical_memory_TOTAL = temp_info[6]    #.replace("GB", "")        # 物理内存总量
                    Physical_memory_USED = temp_info[8]     #.replace("GB", "")        # 物理内存使用量
                    Physical_memory_FREE = temp_info[11]    #.replace("GB", "")       # 物理内存剩余量
                    if not ip in JSON_dict["hosts"]:     # 判断JSON_dict中索引是否存在，如不在则创建
                        JSON_dict["hosts"][ip] = {}               # 用IP做索引（用IP做一级目录）
                    JSON_dict["hosts"][ip]["ciNum"] = "(被监控服务器的ID，预留字段)"     # 添加被监控服务器ID到JSON_dict
                    JSON_dict["hosts"][ip]["num3"] = num3_2        # 添加不知道什么的3位数，到JSON_dict
                    JSON_dict["hosts"][ip]["Physical_memory_TOTAL"] = Physical_memory_TOTAL         # 添加内存总量
                    JSON_dict["hosts"][ip]["Physical_memory_USED"] = Physical_memory_USED           # 添加使用量
                    JSON_dict["hosts"][ip]["Physical_memory_FREE"] = Physical_memory_FREE           # 添加剩余空间量
                else:
                    print("文件内部格式错误，请检查第二列，*****_cup 和 ******_memval,是否合规。")
                    print("错误信息如下：%s" % line_item[1])
        return JSON_dict
    def json2XML(self):
        '''
        生成XML文档。
        '''
        doc = Document()  #创建DOM文档对象
        root = doc.createElement('root')                        # 创建根元素
        doc.appendChild(root)
        Platform = doc.createElement('Platform')                # 创建root下第一节点Platform
        Platform.setAttribute("date", self.DATA_JSON["datetime"])        # **赋值时间
        root.appendChild(Platform)
        PlatformNum = doc.createElement('PlatformNum')          # 创建Platform第一节点PlatformNum
        PlatformNum.setAttribute("description", "数据中心监控平台编号，以便识别，*****************************")
        PlatformNum_title = doc.createTextNode("1872_ZABBIX")
        PlatformNum.appendChild(PlatformNum_title)
        Platform.appendChild(PlatformNum)
        Module = doc.createElement('Module')                    # 创建Platform第一节点Module
        Module.setAttribute("description", "模块编号")
        Module_title = doc.createTextNode("M1")
        Module.appendChild(Module_title)
        Platform.appendChild(Module)
        datas = doc.createElement('datas')                      # 创建root下第一节点datas
        root.appendChild(datas)
        for ip in self.DATA_JSON["hosts"]:
            data = doc.createElement('data')
            data.setAttribute("ciNum", "CI201702230001(被监控服务器的ID，预留字段)")
            data.setAttribute("datetime", self.DATA_JSON["datetime"])
            data.setAttribute("host", self.DATA_JSON["hosts"][ip]["name"])
            data.setAttribute("ip", self.DATA_JSON["hosts"][ip]["ip"])
            data.setAttribute("itemName", "CPU idle time")      # CPU空闲时间
            data.setAttribute("itemValue", self.DATA_JSON["hosts"][ip]["CPU_utilization_percentage"])
            data_title = doc.createTextNode("")     # 为了解决自闭合标签。
            data.appendChild(data_title)            # 为了解决自闭合标签。
            datas.appendChild(data)
            data = doc.createElement('data')
            data.setAttribute("ciNum", "CI201702230001(被监控服务器的ID，预留字段)")
            data.setAttribute("datetime", self.DATA_JSON["datetime"])
            data.setAttribute("host", self.DATA_JSON["hosts"][ip]["name"])
            data.setAttribute("ip", self.DATA_JSON["hosts"][ip]["ip"])
            data.setAttribute("itemName", "Total memory")      # 总内存
            data.setAttribute("itemValue", self.DATA_JSON["hosts"][ip]["Physical_memory_TOTAL"])     # 总内存
            data_title = doc.createTextNode("")     # 为了解决自闭合标签。
            data.appendChild(data_title)            # 为了解决自闭合标签。
            datas.appendChild(data)
            data = doc.createElement('data')
            data.setAttribute("ciNum", "CI201702230001(被监控服务器的ID，预留字段)")
            data.setAttribute("datetime", self.DATA_JSON["datetime"])
            data.setAttribute("host", self.DATA_JSON["hosts"][ip]["name"])
            data.setAttribute("ip", self.DATA_JSON["hosts"][ip]["ip"])
            data.setAttribute("itemName", "Free real memory")      # 空闲内存
            data.setAttribute("itemValue", self.DATA_JSON["hosts"][ip]["Physical_memory_FREE"])     # 空闲
            data_title = doc.createTextNode("")     # 为了解决自闭合标签。
            data.appendChild(data_title)            # 为了解决自闭合标签。
            datas.appendChild(data)
        return doc.toprettyxml(indent='')
    def makeXML(self):
        f = open(self._filename+'.xml', 'w', encoding="UTF-8")
        f.write(self.DATA_XML)
        f.close()
```
调试代码：（调试代码和代码在同级目录下）
```python
# -*- coding: utf-8 -*-
import readFileToXML as RFTX
# 实例化
TEST = RFTX.readFileToXML("zmonitor.2017-04-06-14_28")
#TEST = RFTX.readFileToXML("C:\\Users\\Asktao\\PycharmProjects\\CSV_to_XML\\zmonitor.2017-04-06-14_28")
# 在当前目录下生成XML文件
TEST.makeXML()
# 打印JSON格式数据
print("JSON格式：%s" % TEST.DATA_JSON)
# 打印XML格式数据
print("XML格式：\n %s " % TEST.DATA_XML)
print(TEST._path)       # 路径
print(TEST._filename)   # 文件名
```
后记：
    领导4月7号周五上午布置任务，要求月底之前交。结果开开心心干了半天就看小说了。周六日开开心心睡大觉。结果今天上午10点就说这两天就要。哎，亏着勤劳，一上午就弄完了。要不然就被领导坑了。大家共勉吧，勤劳共勉。
领导改需求了，要在linux当脚本跑，于是改了一下调用方式。
