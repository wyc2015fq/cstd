# 深度学习目标检测常用工具型代码：对检测出来的结果单独进行softnms操作 - Snoopy_Dream - CSDN博客





2018年04月25日 21:37:27[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：153
所属专栏：[数据生成器+数据增广+常用工具](https://blog.csdn.net/column/details/29905.html)












```python
# -*- coding: utf-8 -*-
"""
Created on Mon Apr  9 21:09:09 2018
对每一类生成的prebbox.txt进行softnms操作
1.读入文本，读入bbox
@author: ygx
"""
import os
import numpy as np

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
def cpu_soft_nms(boxes,sigma=0.5, Nt=0.3,threshold=0.001, method=0):
    N = boxes.shape[0]
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
            ts = boxes[pos, 4]

            area = (x2 - x1 + 1) * (y2 - y1 + 1)
            #iw = (min(tx2, x2) - max(tx1, x1) + 1)
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
        #print('keep:', keep)
        outdets = []
        #print('nameboxdict[imgname]: ', nameboxnmsdict[imgname])
        for index in keep:
            # print('index:', index)
            outdets.append(nameboxdict[imgname][index])
        nameboxnmsdict[imgname] = outdets
    return nameboxnmsdict




#我要将他改成softnms，对15类最终的txt进行softnms
def softnmsbase(srcpath, dstpath, nms):
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
                        outline = str(imgname) + ' ' +str(confidence) + ' ' + ' '.join(map(str, bbox))
                        #outline = imgname + ' ' + str(confidence) + ' ' + ' '.join(map(str, bbox))
                        #print('outline:', outline)
                        f_out.write(outline + '\n')



if __name__ == '__main__':
    #mergebypoly()
    srcpath = r'/media/ygx/参赛/最后提交的zip文件/港口、油罐、环形交叉路、桥//nms之前'
    dstpath = r'/media/ygx/参赛/最后提交的zip文件/港口、油罐、环形交叉路、桥/softnms'
    softnmsbase(srcpath,
              dstpath,
              cpu_soft_nms)
```




