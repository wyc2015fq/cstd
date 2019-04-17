# Python获取指定文件夹下的文件名 - 别说话写代码的博客 - CSDN博客





2018年03月02日 10:00:38[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：12036








转自：http://blog.csdn.net/lsq2902101015/article/details/51305825



本文采用os.walk()和os.listdir()两种方法，获取指定文件夹下的文件名。




# 一、os.walk()




模块os中的walk()函数可以遍历文件夹下所有的文件。



**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)



- os.walk(top, topdown=Ture, onerror=None, followlinks=False)  


该函数可以得到一个三元tupple(dirpath, dirnames, filenames).





参数含义：


- dirpath：string，代表目录的路径；
- dirnames：list，包含了当前dirpath路径下所有的子目录名字（不包含目录路径）；
- filenames：list，包含了当前dirpath路径下所有的非目录子文件的名字（不包含目录路径）。



注意，dirnames和filenames均不包含路径信息，如需完整路径，可使用os.path.join(dirpath, dirnames)




下面给出代码;





**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)



- # -*- coding: utf-8 -*- 
- 
- import os  
- 
- def file_name(file_dir):   
- for root, dirs, files in os.walk(file_dir):  
- print(root) #当前目录路径
- print(dirs) #当前路径下所有子目录
- print(files) #当前路径下所有非目录子文件





当需要特定类型的文件时，代码如下：





**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)



- # -*- coding: utf-8 -*- 
- 
- import os  
- 
- def file_name(file_dir):   
-     L=[]   
- for root, dirs, files in os.walk(file_dir):  
- for file in files:  
- if os.path.splitext(file)[1] == '.jpeg':  
-                 L.append(os.path.join(root, file))  
- return L  


其中os.path.splitext()函数将路径拆分为文件名+扩展名，例如os.path.splitext(“E:/lena.jpg”)将得到”E:/lena“+".jpg"。





# 二、os.listdir()




os.listdir()函数得到的是仅当前路径下的文件名，不包括子目录中的文件，所有需要使用递归的方法得到全部文件名。




直接给出代码，函数将返回类型为‘.jpeg’个文件名：





**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)



- # -*- coding: utf-8 -*-
- import os  
- 
- def listdir(path, list_name):  
- for file in os.listdir(path):  
-         file_path = os.path.join(path, file)  
- if os.path.isdir(file_path):  
-             listdir(file_path, list_name)  
- elif os.path.splitext(file_path)[1]=='.jpeg':  
-             list_name.append(file_path)  
- 





