# 对检测出来的结果单独进行softnms操作 - Snoopy_Dream - CSDN博客





2018年06月25日 12:16:52[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：632
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)












![](https://img-blog.csdn.net/2018062511593416?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[1.从官方文档https://github.com/bharatsingh430/soft-nms/blob/master/lib/nms/cpu_nms.pyx，下载cython实现的softnms的代码](https://github.com/bharatsingh430/soft-nms/blob/master/lib/nms/cpu_nms.pyx)  记住最后一行的把return keep改为return boxes[keep,:]，具体如下

```python
# --------------------------------------------------------
# Deformable Convolutional Networks
# Copyright (c) 2015 Microsoft
# Licensed under The MIT License [see LICENSE for details]
# Modified from py-faster-rcnn (https://github.com/rbgirshick/py-faster-rcnn)
# --------------------------------------------------------

import numpy as np
cimport numpy as np

cdef inline np.float32_t max(np.float32_t a, np.float32_t b):
    return a if a >= b else b

cdef inline np.float32_t min(np.float32_t a, np.float32_t b):
    return a if a <= b else b

def cpu_soft_nms(np.ndarray[float, ndim=2] boxes, float sigma=0.5, float Nt=0.3, float threshold=0.001, unsigned int method=0):
    cdef unsigned int N = boxes.shape[0]
    cdef float iw, ih, box_area
    cdef float ua
    cdef int pos = 0
    cdef float maxscore = 0
    cdef int maxpos = 0
    cdef float x1,x2,y1,y2,tx1,tx2,ty1,ty2,ts,area,weight,ov

    for i in range(N):
        maxscore = boxes[i, 4]
        maxpos = i

        tx1 = boxes[i,0]
        ty1 = boxes[i,1]
        tx2 = boxes[i,2]
        ty2 = boxes[i,3]
        ts = boxes[i,4]

        pos = i + 1
	# get max box
        while pos < N:
            if maxscore < boxes[pos, 4]:
                maxscore = boxes[pos, 4]
                maxpos = pos
            pos = pos + 1

	# add max box as a detection 
        boxes[i,0] = boxes[maxpos,0]
        boxes[i,1] = boxes[maxpos,1]
        boxes[i,2] = boxes[maxpos,2]
        boxes[i,3] = boxes[maxpos,3]
        boxes[i,4] = boxes[maxpos,4]

	# swap ith box with position of max box
        boxes[maxpos,0] = tx1
        boxes[maxpos,1] = ty1
        boxes[maxpos,2] = tx2
        boxes[maxpos,3] = ty2
        boxes[maxpos,4] = ts

        tx1 = boxes[i,0]
        ty1 = boxes[i,1]
        tx2 = boxes[i,2]
        ty2 = boxes[i,3]
        ts = boxes[i,4]

        pos = i + 1
	# NMS iterations, note that N changes if detection boxes fall below threshold
        while pos < N:
            x1 = boxes[pos, 0]
            y1 = boxes[pos, 1]
            x2 = boxes[pos, 2]
            y2 = boxes[pos, 3]
            s = boxes[pos, 4]

            area = (x2 - x1 + 1) * (y2 - y1 + 1)
            iw = (min(tx2, x2) - max(tx1, x1) + 1)
            if iw > 0:
                ih = (min(ty2, y2) - max(ty1, y1) + 1)
                if ih > 0:
                    ua = float((tx2 - tx1 + 1) * (ty2 - ty1 + 1) + area - iw * ih)
                    ov = iw * ih / ua #iou between max box and detection box

                    if method == 1: # linear
                        if ov > Nt: 
                            weight = 1 - ov
                        else:
                            weight = 1
                    elif method == 2: # gaussian
                        weight = np.exp(-(ov * ov)/sigma)
                    else: # original NMS
                        if ov > Nt: 
                            weight = 0
                        else:
                            weight = 1

                    boxes[pos, 4] = weight*boxes[pos, 4]
		    
		    # if box score falls below threshold, discard the box by swapping with last box
		    # update N
                    if boxes[pos, 4] < threshold:
                        boxes[pos,0] = boxes[N-1, 0]
                        boxes[pos,1] = boxes[N-1, 1]
                        boxes[pos,2] = boxes[N-1, 2]
                        boxes[pos,3] = boxes[N-1, 3]
                        boxes[pos,4] = boxes[N-1, 4]
                        N = N - 1
                        pos = pos - 1

            pos = pos + 1

    keep = [i for i in range(N)]
    return boxes[keep,:]
```


2.创建setup.py文件

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(
      name = 'softnms_module',
      ext_modules = cythonize('cpu_nms.pyx'),
      )
```

3.开始生成动态链接库so文件和o文件，以及c源代码：

```python
python setup.py build
```

我在这里出现了错误，提示numpy/arrayobject.h: No such file or directory，usr/include/python2.7下面的numpy的软连接，出现错误，然后顺势找到/usr/lib/python2.7/dist-packages发现没有numpy文件夹，然后按照网上的提示 sudo apt-get install python-numpy，提示我已经安装了，然后我在终端下运行cython，import numpy也没有问题。后来pip install numpy，然后发下/usr/lib/python2.7/dist-packages还是发现没有numpy文件夹，之后想起了我是多用户的，在/home/ygx/.local/lib/python2.7/site-packages/numpy，把这个文件夹复制到/usr/lib/python2.7/dist-packages，问题解决。

4.将生成的build文件夹下面的so文件，拷贝到主文件夹下面，删除build文件夹，即可。


5.之后下面是softnms.py的文件，该文件作用是单独执行softnms操作，可调参数有sigma和method，method如果是0就和普通的nms没差。注意我这里的的nms_thresh其实就是置信度的阈值，和普通的不一样。你把它设置成和原先检测时置信度的阈值就可以了，因为重叠区域高的置信度不会变，会将相对低的变的更低，比如0.4变成0.1这样。

![](https://img-blog.csdn.net/20180625121610487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180625121634348?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  9 21:09:09 2018
对每一类生成的prebbox.txt进行softnms操作
1.读入文本，读入bbox
@author: ygx
"""
import sys  
defaultencoding = 'utf-8'  
if sys.getdefaultencoding() != defaultencoding:  
    reload(sys)  
    sys.setdefaultencoding(defaultencoding)
    
import os
import numpy as np
from cpu_nms import cpu_soft_nms

nms_thresh = 0.45 #这个和你之前设置的置信度的阈值一样就好了

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


def cpu_soft_nms_ygx(dets, sigma=0.5, Nt=0.3, threshold=0.001, method=1):
    keep = cpu_soft_nms(np.ascontiguousarray(dets, dtype=np.float32),
                        np.float32(sigma), np.float32(Nt),
                        np.float32(threshold),
                        np.uint8(method))
    return keep


def nmsbynamedict(nameboxdict, nms):
    nameboxnmsdict = {x: [] for x in nameboxdict}
    for imgname in nameboxdict:
        #print('imgname:', imgname)
        #keep = py_cpu_nms(np.array(nameboxdict[imgname]), thresh)
        #print('type nameboxdict:', type(nameboxnmsdict))
        #print('type imgname:', type(imgname))
        #print('type nms:', type(nms))
        keep = nms(np.array(nameboxdict[imgname]))
    #======================ygx=====================
        #keep = nms(np.array(keep))#又多进行了一层的softnms
    ##############################################
        #print('keep:', keep)
        outdets = []
        #print('nameboxdict[imgname]: ', nameboxnmsdict[imgname])
    #======================ygx=====================
        all_num = len(keep)
        keep_soft = [] 
        for i in range(all_num):
            #可以调试的时候看一下
            if keep[i][4]>nms_thresh:
                keep_soft.append(i)
            else:
                continue
        for index in keep_soft:
            # print('index:', index)
            outdets.append(keep[index])
        #千万 注意在softnms之后索引已经发生了变化。。。
            #outdets.append(nameboxdict[imgname][index])
        nameboxnmsdict[imgname] = outdets
    return nameboxnmsdict


#我要将他改成softnms，对15类最终的txt进行softnms
def softnmsbase(srcpath, dstpath, nms):
    filelist = GetFileFromThisRootDir(srcpath)
    for fullname in filelist:
        name = custombasename(fullname)
        #print('name:', name)
        dstname = os.path.join(dstpath, name + '_softnms_'+str(nms_thresh)+'.txt')
        with open(fullname, 'r') as f_in:
            nameboxdict = {}
            lines = f_in.readlines()
            splitlines = [x.strip().split(' ') for x in lines]
            for splitline in splitlines:
                 oriname = splitline[0]
                 confidence = splitline[1]
                 poly = list(map(float, splitline[2:]))
                 det = poly
                
                #det.append(classname)
                 det.append(confidence)
                 det = list(map(float, det))
                 if (oriname not in nameboxdict):
                     nameboxdict[oriname] = []
                 nameboxdict[oriname].append(det)
            nameboxnmsdict = nmsbynamedict(nameboxdict, nms)
            with open(dstname, 'w') as f_out:
                for imgname in nameboxnmsdict:
                    for det in nameboxnmsdict[imgname]:
                        #print('det:', det)
                        confidence = det[-1]
                        bbox = det[0:-1]
                        outline = str(imgname) + ' '  + ' '.join(map(str, bbox))
                        #outline = imgname + ' ' + str(confidence) + ' ' + ' '.join(map(str, bbox))
                        #print('outline:', outline)
                        f_out.write(outline + '\n')



if __name__ == '__main__':
    #mergebypoly()
    srcpath = r'/home/ygx/666/nms之前'
    dstpath = r'/home/ygx/666/nms之后'
    softnmsbase(srcpath,
              dstpath,
              cpu_soft_nms_ygx)
```


参考博客：http://www.cnblogs.com/king-lps/p/9031568.html




