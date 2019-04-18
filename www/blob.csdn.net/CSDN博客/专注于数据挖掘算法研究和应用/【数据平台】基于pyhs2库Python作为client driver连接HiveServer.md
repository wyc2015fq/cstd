# 【数据平台】基于pyhs2库Python作为client driver连接HiveServer - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月13日 17:36:36[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：371
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、安装pyhs2

A Python client driver for HiveServer2 is available at https://github.com/BradRuderman/pyhs2 (thanks, Brad). It includes all the required packages such as SASL and Thrift wrappers.The driver has been certified for use with Python 2.6 and newer.




```python
pip install pyhs2
```


我的环境还需要提前安装VCForPython27，下载地址：https://www.microsoft.com/en-us/download/details.aspx?id=44266






2、参考代码：



```python
import pyhs2
 
with pyhs2.connect(host='localhost',
                   port=10000,
                   authMechanism="PLAIN",
                   user='root',
                   password='test',
                   database='default') as conn:
    with conn.cursor() as cur:
        #Show databases
        print cur.getDatabases()
 
        #Execute query
        cur.execute("select * from table")
 
        #Return column info from query
        print cur.getSchema()
 
        #Fetch table results
        for i in cur.fetch():
            print i
```


参考地址：https://cwiki.apache.org/confluence/display/Hive/Setting+Up+HiveServer2#SettingUpHiveServer2-PythonClientDriver

也可以考虑pyhive，github地址：https://github.com/dropbox/PyHive





