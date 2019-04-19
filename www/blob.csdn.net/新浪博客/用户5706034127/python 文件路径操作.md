# python 文件路径操作_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
转载：http://blog.csdn.net/lsq2902101015/article/details/51305825
# 一、os.walk()
模块os中的walk()函数可以遍历文件夹下所有的文件。
**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1670567/fork)
- 
os.walk(top, topdown=Ture, onerror=None, followlinks=False) 
该函数可以得到一个三元tupple(dirpath, dirnames, filenames).
参数含义：
- 
dirpath：string，代表目录的路径；
- 
dirnames：list，包含了当前dirpath路径下所有的子目录名字（不包含目录路径）；
- 
filenames：list，包含了当前dirpath路径下所有的非目录子文件的名字（不包含目录路径）。
注意，dirnames和filenames均不包含路径信息，如需完整路径，可使用os.path.join(dirpath,
dirnames)
下面给出代码;
**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1670567/fork)
- 
# -*- coding: utf-8 -*- 
- 
- 
importos 
- 
- 
deffile_name(file_dir): 
- forroot, dirs, files inos.walk(file_dir): 
- print(root) #当前目录路径
- print(dirs) #当前路径下所有子目录
- print(files) #当前路径下所有非目录子文件
当需要特定类型的文件时，代码如下：
**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1670567/fork)
- 
# -*- coding: utf-8 -*- 
- 
- 
importos 
- 
- 
deffile_name(file_dir): 
- L=[] 
- forroot, dirs, files inos.walk(file_dir): 
- forfile infiles: 
- ifos.path.splitext(file)[1] == '.jpeg': 
- L.append(os.path.join(root, file)) 
- returnL 
其中os.path.splitext()函数将路径拆分为文件名+扩展名，例如os.path.splitext(“E:/lena.jpg”)将得到”E:/lena“+".jpg"。
# 二、os.listdir()
os.listdir()函数得到的是仅当前路径下的文件名，不包括子目录中的文件，所有需要使用递归的方法得到全部文件名。
直接给出代码，函数将返回类型为‘.jpeg’个文件名：
**[python]**[view plain](http://blog.csdn.net/lsq2902101015/article/details/51305825#)[copy](http://blog.csdn.net/lsq2902101015/article/details/51305825#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1670567/fork)
- 
# -*- coding: utf-8 -*-
- 
importos 
- 
- 
deflistdir(path, list_name): 
- forfile inos.listdir(path): 
- file_path = os.path.join(path, file) 
- ifos.path.isdir(file_path): 
- listdir(file_path, list_name) 
- elifos.path.splitext(file_path)[1]=='.jpeg': 
- list_name.append(file_path) 
