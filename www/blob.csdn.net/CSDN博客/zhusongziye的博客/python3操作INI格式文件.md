# python3操作INI格式文件 - zhusongziye的博客 - CSDN博客





2017年08月30日 20:24:51[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1786







**什么是INI格式**


INI文件格式是某些平台或软件上的配置文件的非正式标准，以节(section)和键(key)构成，常用于微软Windows操作系统中。这种配置文件的文件扩展名多为INI，故名。



INI是英文“初始化”（initialization）的缩写。正如该术语所表示的，INI文件被用来对操作系统或特定程序初始化或进行参数设置。


**INI文件格式**

节(section) 节用方括号括起来，单独占一行，例如：



[section]



键(key)



键(key)又名属性(property)，单独占一行用等号连接键名和键值，例如：



name=value



注释(comment)



注释使用英文分号（;）开头，单独占一行。在分号后面的文字，直到该行结尾都全部为注释，例如：



; comment text



下面看一个用于MySQL数据库链接参数配置实例



```python
[mysql]
;数据库地址
host=127.0.0.1
;数据库链接端口
port=3306
;数据库用户名
user=root
;数据库密码
password=123456
;默认链接的数据库名
db=autotest
```







**Python ConfigParser类**



在python里由标准模块ConfigParser模块提供的ConfigParser类实现对INI格式的文件进行读写，下面我们看看其主要的函数，也是大家必须熟悉的。


```python
# 初始化  
cf = ConfigParser.ConfigParser()  # 读取ini文件,path为要读取的ini文件的路径 
cf.read(path) 


# 获取所有sections。即将配置文件中所有“[ ]”读取到列表中
s = cf.sections() 


# 获取指定section的options。


# 即将配置文件某个section内key 读取到列表中
o = cf.options("mysql")# 获取指定section 的配置信息v = cf.items("msyql")# 按照类型读取指定section 的option 信息# 同样的还有getfloat、getbooleandb_host = cf.get("mysql", "host")


db_port = cf.getint("mysqldb", "port")


db_user = cf.get("mysql", "user")


db_pass = cf.get("mysql", "password") 


# 设置某个option 的值。（记得最后要保存）
cf.set("mysql", "password", "654321")


# 添加一个section。（同样要保存）
cf.add_section('oracle')


cf.set('oracle', 'host', '127.0.0.1')
cf.set('oracle', 'port', '5555') 


#  移除section 或者option (同样要保存)
cf.remove_option('oracle','port')
cf.remove_section('oracle')
```




**实例**

下面我们对ConfigParser进行下简单的封装，形成我们自己的ini解析类，并演示如何使用。

```python
#-*- coding:utf-8 -*-

from configparser import ConfigParser
import os


class LYMINIParser:
    def __init__(self, path):
        self.path = path
        self.ini = ConfigParser()
        self.ini.read(self.path)    


    # 获取sections列表
    def get_sections(self):
        if self.ini:        
            return self.ini.sections() 
    
    # 获取指定的section的options列表
    def get_options_by_section(self, section):
        if self.ini:     
            return self.ini.options(section)    
    
    # 获取指定section的配置信息列表
    def get_section_items(self, section):
        if self.ini:            
            return self.ini.items(section)    




    # 按类型读取配置信息


    # 返回字符串类型
    def get_string(self, section, option):
        if self.ini:            
            return self.ini.get(section, option) 


    # 返回int类型
    def get_int(self, section, option):
        if self.ini:         
            return self.ini.getint(section, option)    
            
    # 返回float类型
    def get_float(self, section, option):
        if self.ini:            
            return self.ini.getfloat(section, option)    


    # 返回bool类型    
    def get_boolean(self, section, option):
        if self.ini:            
            return self.ini.getboolean(section, option)    


    # 新增section
    def add_section(self, section):
        if self.ini:
            self.ini.add_section(section)
            self.ini.write(open(self.path, "w")) 


    # 设置指定option值
    def set_option(self, section, option, value):
        if self.ini:
            self.ini.set(section, option, value)
            self.ini.write(open(self.path, "w")) 
    
    # 删除指定section
    def remove_section(self, section):
        if self.ini:
            self.ini.remove_section(section)
            self.ini.write(open(self.path, "w")) 


    # 删除指定option
    def remove_option(self, section, option):
        if self.ini:
            self.ini.remove_option(section, option)
            self.ini.write(open(self.path, "w"))         


if __name__ == "__main__":
    print("python ini标准库解析实例")    


    # 如果存在mysql.ini先删除，方便下列代码的运行
    if os.path.exists("mysql.ini"):
        os.remove("mysql.ini")    


    # 我们先写一些数据到mysql.ini中
    ini = LYMINIParser("mysql.ini")    


    # 先加一个mysql section
    mysql_section = "mysql"
    ini.add_section(mysql_section)    


    # 在mysql section下写入一些配置信息
    ini.set_option(mysql_section, "host", "127.0.0.1")
    ini.set_option(mysql_section, "port", "3306")
    ini.set_option(mysql_section, "db", "autotesting")
    ini.set_option(mysql_section, "user", "root")
    ini.set_option(mysql_section, "password", "123456")    
    
    
    # 再添加一个oracle section
    oracle_section = "oracle"
    ini.add_section(oracle_section)    


    # oracle section下写入一些配置信息
    ini.set_option(oracle_section, "host", "127.0.0.1")
    ini.set_option(oracle_section, "port", "1520")
    ini.set_option(oracle_section, "db", "auto_ui")
    ini.set_option(oracle_section, "user", "sa")
    ini.set_option(oracle_section, "password", "123456")    
    
    # 获取下所有的section，并在console输出
    sections = ini.get_sections()
    print(sections)    


    # 遍历各个section下的options，并在console中输出
    print("---" * 20)    for sec in sections:
        print(sec, " 中的options为： ")
        options = ini.get_options_by_section(sec)
        print(options)
        print("---" * 20)    


    # 获取各个section下的配置信息
    for sec in sections:
        print(sec, " 中的配置信息为： ")
        items = ini.get_section_items(sec)
        print(items)
        print("---" * 20)    


    # 获取指定的option值这里演示读取host和port
    host = ini.get_string("mysql", "host")
    port = ini.get_int("mysql", "port")
    print("类型： ", type(host), " ", type(port))
    print(host, " ", port)    


    # 删除mysql中的host配置
    ini.remove_option("mysql", "host")    


    # 删除oracle section
    ini.remove_section("oracle")    


    # 修改mysql port的值为4000
    ini.set_option("mysql", "port", "4000")    
    
    # 最终mysql.ini中的文件内容如下
    #[mysql]
    #port = 4000
    #db = autotesting
    #user = root
    #password = 123456
```



**小结**


本文对ini格式的解析进行了较为完整的演示，大家可以根据需要对示例中的进一步封装优化掌握。




来源于网络，如若侵犯您的权益，请留言联系我，我会第一时间处理！







