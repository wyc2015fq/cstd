# from caffe2.proto import caffe2_pb2 ImportError: cannot import name caffe2_pb2 - Snoopy_Dream - CSDN博客





2018年06月09日 22:24:59[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：2668








检查caffe2的gpu版本是否安装成功的时候，终端下运行python后输出



`from caffe2.python import workspace`
出现from caffe2.proto import caffe2_pb2 ImportError: cannot import name caffe2_pb2的错误。

但是我



```
import caffe2
```

没有出现问题

**解决方案：**在home路径下，按住ctrl+H，找到.bashrc文件，打开。

1. 在末尾fi后面第118行，写入

```
export PYTHONPATH=/home/zj/pytorch/build:/usr/local:$PYTHONPATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```
主要是看第一行，中间build后面的：类似于分号；的意思，就是`/usr/local和/home/zj/pytorch/build中python相关的包都作为系统python路径导入`



```python
from caffe2.proto import caffe2_pb2
```

**用到的其实就是 /usr/local/lib/python2.7/dist-packages/caffe2/proto/caffe2_pb2.py**

2.source .bashrc

然后重新进入python写入
`from caffe2.python import workspace`不会报错。
















![](https://img-blog.csdn.net/2018060922173019?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




