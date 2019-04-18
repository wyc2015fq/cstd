# Python 遍历目录 - wishchinYang的专栏 - CSDN博客
2016年07月25日 18:19:58[wishchin](https://me.csdn.net/wishchin)阅读数：766
代码：
1.递归使用遍历目录
```python
import os  
  
def scanfile(path):  
    filelist = os.listdir(path)  
    allfile = []  
    for filename in filelist:  
        filepath = os.path.join(path,filename)  
        if os.path.isdir(filepath):  
            scanfile(filepath)  
        print filepath
```
2.使用listdir
```python
import os
//s = os.sep
//root = "d:" + s + "ll" + s
root = "E:/DataBase/EyeVive3/070802/f"
 for i in os.listdir(root):
    if os.path.isfile(os.path.join(root,i)):
        print i
```
