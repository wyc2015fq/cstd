# 深度学习目标检测常用工具型代码：对训练数据的trainval.txt进行数据清洗 - Snoopy_Dream - CSDN博客





2018年04月25日 22:31:09[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：221
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









这个用于ODAI（航空数据目标检测）比赛当中，这个数据集像素大小从800*800-4000*4000不等，所以需要进行原始数据的裁剪，在裁剪过后有一部分数据里面什么目标都没有，要把这一部分去除，使得高效训练！





```python
#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# --------------------------------------------------------
# --------------------------------------------------------

"""
如果你的训练图片太大，需要对图片进行裁剪，那么就会有很大一部分裁剪的图片里没有标签，
已经制作好了trainval.txt，将里面对应的没有标签的索引拿掉，不用做训练，即
所以需要进行清洗数据的操作，即把null的标签对应的裁剪后数据和nul的label删掉。

"""

import os
#rootdir = os.getcwd()               #获取当前路径
#文件结构说明：注意我的rootdir下面不是标签的txt，而是在rootdir下一个文件夹里面存放着txt
#这里的文件结构是/media/ygx/Elements/VOCdevkit2007/trainsplit/1下面只有一个名为1的文件夹，里面放着txt
rootdir = '/media/ygx/Elements/VOCdevkit2007/trainsplit/1'
#顺便把所有文件的大小写入到一个csv文件中
rootdir = rootdir.decode('gbk')
x  = u'统计文件大小.csv'
f = open(os.path.join(rootdir,x), "w+")

#找到标签文件夹下，0kb的txt文档，并且将其记录在null列表里面
null=[]
for dirname in  os.listdir(rootdir):  #获取二级目录所有文件夹与文件
    Dir = os.path.join(rootdir, dirname)    #路径补齐
    count = 0
    if (os.path.isdir(Dir)):  #判断是否为目录
        for r, ds, files in os.walk(Dir): #遍历目录下所有文件根，目录下的每一个文件夹(包含它自己), 产生3-元组 (dirpath, dirnames, filenames)【文件夹路径, 文件夹名字, 文件名称】
            for file in files:      #遍历所有文件
                size = os.path.getsize(os.path.join(r, file)) #获取文件大小
                count += size
                #print size
                (filename,extension) = os.path.splitext(file)
                if size == 0:
                    null.append(filename)
        if ((count/1024.0/1024.0) < 1024):
            #print  Dir +'\t' + '%.2f'% (count/1024.0/1024.0)+'MB'
            f.write(Dir.encode("gbk") +','+  '%.2f'% (count/1024.0/1024.0)+'MB' + '\n')
        else:
            #print  Dir + '\t' + '%.2f' % (count / 1024.0 / 1024.0/1024.0) + 'GB'
            f.write(Dir.encode("gbk") + ',' + '%.2f' % (count / 1024.0 / 1024.0/1024.0) + 'GB' + '\n')
    else:
        continue
f.close()


def GetFileFromThisRootDir(dir,ext = None):
  allfiles = []
  needExtFilter = (ext != None)
  for root,dirs,files in os.walk(dir):
    for filespath in files:
      filepath = os.path.join(root, filespath)
      extension = os.path.splitext(filepath)[1][1:]
      if needExtFilter and extension in ext:
        allfiles.append(filepath)
      elif not needExtFilter:
        allfiles.append(filepath)
  return allfiles

#已经制作好了trainval.txt，这里就是trainval.txt的路径。
allfiles = GetFileFromThisRootDir('/media/ygx/Elements/VOCdevkit2007/VOC2007/ImageSets/Main/train_and_test_子类')
          
for files in allfiles:
    filename = os.path.basename(files)
    #新的trainval存放的路径
    newfile = '/media/ygx/Elements/VOCdevkit2007/VOC2007/ImageSets/train_and_test_子类_clear/'+filename
    with open(files,'r') as r:
        lines = r.readlines()    
    with open(newfile,'w') as w:
        #如果line不再null列表当中，则保留
        for line in lines:
            if (line[:-1] not in null):
                w.write(line)
            else:
                print '1'
```




