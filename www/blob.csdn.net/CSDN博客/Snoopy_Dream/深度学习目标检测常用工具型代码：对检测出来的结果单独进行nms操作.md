# 深度学习目标检测常用工具型代码：对检测出来的结果单独进行nms操作 - Snoopy_Dream - CSDN博客





2018年04月25日 21:06:26[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：96
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)









p.s. 这里的例子我说的都是航空影像目标，所以大家考虑的时候都要考虑成俯视图的状态。


多目标检测的时候，有容易发生拥挤的类别，比如小汽车，也有不易拥挤的类别，比如篮球场。所以需要不同的nms阈值，而检测一次有时候消耗的时间太多，所以我的解决办法是，检测的时候nms=1都输出出来，然后在通过单独对检测的txt结果进行nms操作，比如所有文件nms0.3、nms0.4各种nms全都可以重新输出来，这样会比检测的同时进行nms方便快很多。





```python
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  9 21:09:09 2018
对每一类检测生成的prebbox.txt进行nms操作
1.读入文本，读入bbox
@author: ygx
"""
import os
import numpy as np

#nms的阈值设置
nms_thresh = 0.5
#nms之前的路径
srcpath = r'/media/ygx/B4FE-5315/足球场/nms之前'
#nms之后的保存路径
dstpath = r'/media/ygx/B4FE-5315/足球场/nms/nms0.5'

def custombasename(fullname):
    return os.path.basename(os.path.splitext(fullname)[0])

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

def py_cpu_nms(dets, thresh):
    """Pure Python NMS baseline."""
    #print('dets:', dets)
    x1 = dets[:, 0]
    y1 = dets[:, 1]
    x2 = dets[:, 2]
    y2 = dets[:, 3]
    scores = dets[:, 4]

    areas = (x2 - x1 + 1) * (y2 - y1 + 1)
    ## index for dets
    order = scores.argsort()[::-1]

    keep = []
    while order.size > 0:
        i = order[0]
        keep.append(i)
        xx1 = np.maximum(x1[i], x1[order[1:]])
        yy1 = np.maximum(y1[i], y1[order[1:]])
        xx2 = np.minimum(x2[i], x2[order[1:]])
        yy2 = np.minimum(y2[i], y2[order[1:]])

        w = np.maximum(0.0, xx2 - xx1 + 1)
        h = np.maximum(0.0, yy2 - yy1 + 1)
        inter = w * h
        ovr = inter / (areas[i] + areas[order[1:]] - inter)

        inds = np.where(ovr <= thresh)[0]
        #print (areas[i] + areas[order[1:]] - inter)
        order = order[inds + 1]

    return keep


def nmsbynamedict(nameboxdict, nms,thresh):
    nameboxnmsdict = {x: [] for x in nameboxdict}
    for imgname in nameboxdict:
        #print('imgname:', imgname)
        #keep = py_cpu_nms(np.array(nameboxdict[imgname]), thresh)
        #print('type nameboxdict:', type(nameboxnmsdict))
        #print('type imgname:', type(imgname))
        #print('type nms:', type(nms))
        keep = nms(np.array(nameboxdict[imgname]),thresh)
        #print('keep:', keep)
        outdets = []
        #print('nameboxdict[imgname]: ', nameboxnmsdict[imgname])
        for index in keep:
            # print('index:', index)
            outdets.append(nameboxdict[imgname][index])
        nameboxnmsdict[imgname] = outdets
    return nameboxnmsdict



def nmsbase(srcpath, dstpath, nms):
    filelist = GetFileFromThisRootDir(srcpath)
    for fullname in filelist:
        name = custombasename(fullname)
        #print('name:', name)
        dstname = os.path.join(dstpath, name + '.txt')
        with open(fullname, 'r') as f_in:
            nameboxdict = {}
            lines = f_in.readlines()
            splitlines = [x.strip().split(' ') for x in lines]
            for splitline in splitlines:
                 oriname = splitline[0]#获取文件名P0051
                 confidence = splitline[1]
                 poly = list(map(float, splitline[2:]))
                 det = poly             
                 det.append(confidence)
                 det = list(map(float, det))  
                 #det.append(oriname)#变到列表里面 一一对应
                 if (oriname not in nameboxdict):
                     nameboxdict[oriname] = []
                 nameboxdict[oriname].append(det)
            nameboxnmsdict = nmsbynamedict(nameboxdict, nms ,nms_thresh)
            with open(dstname, 'w') as f_out:
                for imgname in nameboxnmsdict:
                    for det in nameboxnmsdict[imgname]:
                        #print('det:', det)
                        confidence = det[-1]
                        bbox = det[0:-1]
                        outline = str(imgname) + ' ' +str(confidence) + ' ' + ' '.join(map(str, bbox))
                        #outline = imgname + ' ' + str(confidence) + ' ' + ' '.join(map(str, bbox))
                        #print('outline:', outline)
                        f_out.write(outline + '\n')

if __name__ == '__main__':
    nmsbase(srcpath,
              dstpath,
              py_cpu_nms)
```




