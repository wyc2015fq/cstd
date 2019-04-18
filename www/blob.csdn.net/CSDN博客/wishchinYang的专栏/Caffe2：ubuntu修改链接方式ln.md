# Caffe2：ubuntu修改链接方式ln - wishchinYang的专栏 - CSDN博客
2018年01月30日 10:55:24[wishchin](https://me.csdn.net/wishchin)阅读数：984
         参考：[文件和目录命令-文件重定向](http://man.linuxde.net/ln)
ln  
使用caffe2，产生了此种情况：
```python
from caffe2.python import workspace
>>WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode.
>>WARNING:root:Debug message: libcudart.so.8.0: cannot open shared object file: No such file or directory
```
不小心升级了CUDA，升级到cuda9.1，/usr/local/cuda 快捷方式 链接则指向cuda9.1，需要再次重定向cuda快捷方式，使其指向cuda-8.0。
使用命令：
删除掉原来的链接
       cd  /usr/local
       sudo rm -rf cuda
注意：不要使用 sudo rm -rf cuda/ 
重新建立链接
     sudo ln -s /usr/local/cuda-8.0  cuda
则重新建立指向cuda-8.0的cuda文件件超链接
测试成功：
