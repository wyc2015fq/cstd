# 【Python】python实现将文件夹内所有txt文件合并成一个文件 - zkq_1986的博客 - CSDN博客





2018年08月31日 22:22:07[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：174








# [python实现将文件夹内所有txt文件合并成一个文件](https://www.cnblogs.com/zhangtianyuan/p/6875150.html)

      新建一个文件夹命名为yuliao，把所有txt文件放进去就ok啦！注意路径中‘/’，windows下路径不是这样。

```
#coding=utf-8 
import os
#获取目标文件夹的路径
filedir = os.getcwd()+'/yuliao'
#获取当前文件夹中的文件名称列表  
filenames=os.listdir(filedir)
#打开当前目录下的result.txt文件，如果没有则创建
f=open('result.txt','w')
#先遍历文件名
for filename in filenames:
    filepath = filedir+'/'+filename
    #遍历单个文件，读取行数
    for line in open(filepath):
        f.writelines(line)
    // f.write('\n')
#关闭文件
f.close()
```

转载自：[https://www.cnblogs.com/zhangtianyuan/p/6875150.html](https://www.cnblogs.com/zhangtianyuan/p/6875150.html)



