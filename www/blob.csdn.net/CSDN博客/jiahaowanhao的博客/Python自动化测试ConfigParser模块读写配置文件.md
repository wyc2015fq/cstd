
# Python自动化测试ConfigParser模块读写配置文件 - jiahaowanhao的博客 - CSDN博客


2018年04月07日 20:15:25[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：97


Python自动化测试ConfigParser模块读写配置文件
ConfigParser 是Python自带的模块， 用来读写配置文件， 用法及其简单。 直接上代码，不解释，不多说。
配置文件的格式是： []包含的叫section,    section 下有option=value这样的键值
配置文件   test.conf
[section1]
name = tank
age = 28
[section2]
ip = 192.168.1.1
port = 8080
Python代码
\# -* - coding: UTF-8 -* -
import ConfigParser
conf = ConfigParser.ConfigParser()
conf.read("c:\\test.conf")
\# 获取指定的section， 指定的option的值
name = conf.get("section1", "name")
print(name)
age = conf.get("section1", "age")
print age
\#获取所有的section
sections = conf.sections()
print sections
\#写配置文件
\# 更新指定section, option的值
conf.set("section2", "port", "8081")
\# 写入指定section, 增加新option的值
conf.set("section2", "IEPort", "80")
\# 添加新的 section
conf.add_section("new_section")
conf.set("new_section", "new_option", "http://www.cnblogs.com/tankxiao")
\# 写回配置文件
conf.write(open("c:\\test.conf","w"))
以上就是对Python ConfigParser模块读写配置文件的资料整理，后续继续补充相关资料

