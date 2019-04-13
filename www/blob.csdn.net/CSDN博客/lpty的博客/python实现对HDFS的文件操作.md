
# python实现对HDFS的文件操作 - lpty的博客 - CSDN博客

2017年01月14日 20:49:12[lpty](https://me.csdn.net/sinat_33741547)阅读数：9832


在HDFS中，要实现对文件的操作，一般可以在shell中发送指令完成，但这样太麻烦了。
当然我们可以调用HDFS的API，这里我们可以使用python的pyHdfs库来实现对HDFS的文件操作。
一、环境准备
1、python3.5
2、pyHdfs
二、pyHdfs安装
1、pip3 install --upgrade pip
先更新pip，防止版本过低
2、pip3 install pyHdfs
三、调用方法
详细的可见[官方文档](http://pyhdfs.readthedocs.io/en/latest/pyhdfs.html)
1、实例化客户端

```python
from pyhdfs import HdfsClient
client = HdfsClient(hosts='localhost:50070')
print(client.list_status('/'))
```
2、各类方法列举(1)copy_from_local(localsrc,
 dest, **kwargs)
(2)copy_to_local(src, localdest, **kwargs)
(3)delete(path, **kwargs)
(4)exists(path, **kwargs)
(5)listdir(path, **kwargs)
(6)mkdirs(path, **kwargs)
...


