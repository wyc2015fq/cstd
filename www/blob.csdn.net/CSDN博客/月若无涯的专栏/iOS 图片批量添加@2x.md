# iOS 图片批量添加@2x - 月若无涯的专栏 - CSDN博客
2013年09月26日 20:11:56[月若无涯](https://me.csdn.net/u010124617)阅读数：1324标签：[iOS																[图片																[添加2x后缀](https://so.csdn.net/so/search/s.do?q=添加2x后缀&t=blog)](https://so.csdn.net/so/search/s.do?q=图片&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/u010124617/article/category/1393991)
对于iOS开发者来说，常常会有设计部给到一大批素材，然后需要自己去一个个命名，添加@2x等等操作。
这里我使用python写了一个批量给图片添加@2x的脚本。分享出来。
```python
#!\usr\bin\env python
# -*- coding: utf-8 -*-
# Author: YueRuo
# Function: 批量命名图片的名字，给定一个目录，会把此目录下的所有文件(含子目录的文件)添加为@2x
import sys
import os
import os.path
import pickle
def UsePrompt():
    #如果省略path，则path为当前路径
    print 'Useage: add@2x.py [path]'
    sys.exit()
def BatchRename(path):
    if (os.path.isfile(path)) :
        return
    os.chdir(path)
    fileList = os.listdir(path)
    print ('--->>check path='+path )
    fileNameDict = {}
    for fileItem in fileList:
        print ('--->>fileItem='+fileItem )
        fileItemPath= os.path.join(path,fileItem)
        if (os.path.isdir(fileItemPath)) :
            BatchRename(fileItemPath)
            continue
        if (fileItem.find('@2x')>=0):
            continue
        dotIndex = fileItem.find('.')
        if(dotIndex<=0):
            continue
        fileName = fileItem[ : dotIndex]
        fileExt = fileItem[dotIndex : ]
        fileFullName = fileName + '@2x' + fileExt
        print ('----->>try rename file '+fileItem + ' to '+ fileFullName)
        os.chdir(path)
        os.rename(fileItem, fileFullName)
        print (fileName + ': ' + fileExt + ' => ' + fileFullName)
def main():
    cancelling = False
    if len(sys.argv) == 2:
        path = sys.argv[1]
    else:
        path = os.getcwd()
    confirm = raw_input('Confirm(y|n): ')
    if confirm == 'n':
        sys.exit()
    else:
        BatchRename(path)
if __name__ == '__main__':
    main()
```
使用方法，把此部分的代码保存为把此部分的代码保存为add@2x.py
打开命令终端，使用命令  
```python
python add@2x.py  你要处理的图片文件夹目录
```
附件是这个脚本文件。
把此部分的代码保存为
