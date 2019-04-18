# ML二：python批量修改文件名-测试KDTree - wishchinYang的专栏 - CSDN博客
2014年01月18日 16:17:07[wishchin](https://me.csdn.net/wishchin)阅读数：671
个人分类：[MLandPy																[PythonLG](https://blog.csdn.net/wishchin/article/category/1548081)](https://blog.csdn.net/wishchin/article/category/1508471)
（1）：#批量修改文件名
```python
import os
import numpy as np
import string
import shutil
prefix =''#单引号，前缀！
sufix  ='txt'
renameFiles(srcdir,prefix,sufix)
def renameFiles(srcdir, prefix,sufix):     
    os.chdir(srcdir)
    srcfiles = os.listdir(srcdir)  #遍历文件   
    for srcfile in srcfiles:         
        sufixsrc = os.path.splitext( srcfile )[1][1:]
        destfile = srcfile.replace(sufixsrc,sufix)
        #destfile =  prefix + srcfile + sufix  
        #srcfile = os.path.join(srcdir, srcfile)  
        os.rename(srcfile, destfile)
```
（2）：#遍历特征，生成特征矩阵
```python
import os
import fileinput
import numpy as np
import re   
#遍历特征，写入向量矩阵！   
#用于构建决策树
#无法建立索引，只能遍历查找K近邻！
path ='D:/Develope/MyVsWorks/Test2/X64_VS10_Test/pillow/ESF_TXT'    
filelist= os.listdir(path)
os.chdir(path)
vectorESF =[]
for filename in filelist:
    file = open(filename)
    linef = file.readlines(11)
    flist =linef[11].split()
    list =np.zeros((1,640))
    idx =0    
    for fvalue in flist:
        list[0,idx] =float(fvalue)
        ++idx       
    vectorESF.append(list[0])
```
（3）：列表转化为矩阵
```python
#列表无shape ，转化为矩阵！
lineNum = vectorESF.__len__()
dataset = np.mat(np.random.rand(lineNum,640))
for x in range(0,lineNum-1):
    for y in range(0,640 -1):
        dataset[x,y] =vectorESF[x][y]
```
（4）：载入测试特征
```python
#Test
testvec =np.zeros((1,640))
filename ='D:/Develope/MyVsWorks/Test2/X64_VS10_Test/pillow/ESF_TXT/ViewX_5.235987 ViewY_0.000000 ViewZ_6.141592..txt'
file = open(filename)
linef = file.readlines(11)
flist =linef[11].split()
idx =0
for fvalue in flist:
    testvec[0,idx] =float(fvalue)#载入到向量testvec[0]
    ++idx
```
（5）：由矩阵生成特征树 进行分类测试
..............
