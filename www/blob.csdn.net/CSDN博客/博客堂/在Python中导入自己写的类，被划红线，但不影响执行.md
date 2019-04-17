# 在Python中导入自己写的类，被划红线，但不影响执行 - 博客堂 - CSDN博客





2019年03月02日 16:32:59[最小森林](https://me.csdn.net/u012052268)阅读数：86











### 文章目录
- [1. 错误描述](#1__2)
- [2. 原因分析](#2__9)
- [3. 解决方案](#3__19)
- [3.1 加一个“点”](#31__21)
- [3.2 添加“当前目录”](#32__32)




### 1. 错误描述

之前在学习Python的过程中，导入自己写的包文件时，与之相关的方法等都会被划红线，但并不影响代码执行，如图：

![在这里插入图片描述](https://img-blog.csdn.net/20180816142947571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1RheHVzX3NoYW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看着红线确实有点强迫症，并且在这个过程当时，当使用该文件里的方法时不会自动提示方法名，只能靠手全部输入，这种容易造成手误，对于小白特别容易降低编写效率

### 2. 原因分析

pycharm中，**source root**概念非常重要，当你在代码中写相对路径的时候，就是以**source root**为起点进行查询。

而pycharm中，**当前的项目文件夹** 是默认的**source root**，当你使用import语句导入模块时，Pycharm默认在project folder中寻找；

所以当你在project folder下的某个subfolder导入某个同样定义在这个subfolder中的模块时，会出现导入错误。

### 3. 解决方案

问题已经找到了，pycharm不会将当前文件目录自动加入自己的sourse_path。所以我们要告诉pycharm就行了。下面有两种方法（3.1和3.2），都能比较方便的解决问题：

#### 3.1 加一个“点”

第一种方法非常简单，只需要在models前加一个点。表示，告诉pycharm我要引用的模块位于同级目录。

例如：

```
from models import className
```

修改为：

```
from .models import className
```

就可以不报错了

#### 3.2 添加“当前目录”

第二种方法：

既然，pycharm不会将当前文件目录自动加入自己的sourse_path。那么我们就手动告诉他。

右键目录名 点击make_directory as  -->  sources path   将当前工作的文件夹加入source_path就可以了。步骤如下图：
![在这里插入图片描述](https://pic2.zhimg.com/80/v2-795833362af1e0193b147b8e36c123c5_hd.png)



