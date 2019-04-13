
# python获取指定目录下所有文件名列表的方法 - jiahaowanhao的博客 - CSDN博客


2018年02月06日 08:34:02[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1438


[python获取指定目录下所有文件名列表的方法](http://cda.pinggu.org/view/24667.html)
本文实例讲述了python获取指定目录下所有文件名列表的方法。分享给大家供大家参考。具体实现方法如下：
这里python代码实现获取文件名列表的功能，可以指定文件中包含的字符，方便提取特定类型的文件名列表：
\# -*- coding: utf-8 -*-
\#~ \#------------------------------------------------------------------
\#~ module:wlab
\#~ Filename:wgetfilelist.py
\#~ Function :
\#~ def IsSubString(SubStrList,Str)
\#~ def GetFileList(FindPath,FlagStr=[]):
\#~ 功能:读取指定目录下特定类型的文件名列表
\#~ Data: 2013-08-08,星期四
\#~ Author:吴徐平
\#~ Email:wxp07@qq.com
\#~ \#------------------------------------------------------------------
\#~ \#------------------------------------------------------------------
def IsSubString(SubStrList,Str):
'''''
\#判断字符串Str是否包含序列SubStrList中的每一个子字符串
\#>>>SubStrList=['F','EMS','txt']
\#>>>Str='F06925EMS91.txt'
\#>>>IsSubString(SubStrList,Str)\#return True (or False)
'''
flag=True
for substr in SubStrList:
if not(substr in Str):
flag=False
return flag
\#~ \#----------------------------------------------------------------------
def GetFileList(FindPath,FlagStr=[]):
'''''
\#获取目录中指定的文件名
\#>>>FlagStr=['F','EMS','txt'] \#要求文件名称中包含这些字符
\#>>>FileList=GetFileList(FindPath,FlagStr) \#
'''
import os
FileList=[]
FileNames=os.listdir(FindPath)
if (len(FileNames)>0):
for fn in FileNames:
if (len(FlagStr)>0):
\#返回指定类型的文件名
if (IsSubString(FlagStr,fn)):
fullfilename=os.path.join(FindPath,fn)
FileList.append(fullfilename)
else:
\#默认直接返回所有文件名
fullfilename=os.path.join(FindPath,fn)
FileList.append(fullfilename)
\#对文件名排序
if (len(FileList)>0):
FileList.sort()
return FileList
可以使用pip在线安装wlab
?
1
pip install wlab
还是给个图吧：
![](http://cda.pinggu.org/uploadfile/image/20180206/20180206071644_79420.png)
希望本文所述对大家的Python程序设计有所帮助。

