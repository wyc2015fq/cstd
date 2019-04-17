# 【python】windows本地用python读写hdfs文件 - zkq_1986的博客 - CSDN博客





2019年01月22日 10:45:36[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：135








pip install hdfs



```
from hdfs.client import Client
HDFSHOST = "http://192.168.1.1:50070;http://192.168.1.2:50070" #HA模式
client = Client(HDFSHOST)
print(client.list('/'))
```







