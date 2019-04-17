# fater-rcnn绘制loss图 - Snoopy_Dream - CSDN博客





2018年06月13日 10:59:22[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：273
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)









py-fater-rcnn根据生成的log文件绘制loss图。

一般输出是下图中的第一幅图（对应位置为2,5,1），但本身并没有多大的意义，只有将其平均之后如(对应位置为2,5,6)，才能看出loss是否还在下降。




![](https://img-blog.csdn.net/20180613105241888?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```python
# -*- coding: utf-8 -*-
"""
Created on Wed Jun 13 09:58:00 2018

@author: ygx
"""
# -*- coding:utf-8 -*-
# !/usr/bin/env python
import matplotlib.pyplot as plt
f1=open('faster_rcnn_end2end_VGGcp_.txt.2018-06-12_17-51-16','r')
lines=f1.readlines()
i=0
Iteration=[]
loss=[]
loss_bbox=[]
loss_cls=[]
rpn_cls_loss=[]
rpn_loss_bbox=[]
for line in lines:  
    if line.find('solver.cpp:229] Iteration')>0:       
        Iteration.append(int(line.split('Iteration ')[1].split()[0].split(',')[0]))
        loss.append(float(line.split(' ')[-1]))
        i=i+1
        print '***',i,'***'     
    if line.find('Train net output #0:')>0:       
        loss_bbox.append(float(line.split(' ')[-6]))
    if line.find('Train net output #1:')>0:       
        loss_cls.append(float(line.split(' ')[-6]))
    if line.find('Train net output #2:')>0:       
        rpn_cls_loss.append(float(line.split(' ')[-6]))
    if line.find('Train net output #3:')>0:       
        rpn_loss_bbox.append(float(line.split(' ')[-6]))          
##总的loss图，我自己sovel文件中设置的是log文件中每20次显示一次       
plt.subplot(251)
plt.title('loss') 
plt.plot(Iteration,loss,c='green')

plt.subplot(252)
plt.title('loss_bbox') 
plt.plot(Iteration,loss_bbox,c='green')

plt.subplot(253)
plt.title('loss_cls') 
plt.plot(Iteration,loss_cls,c='green')

plt.subplot(254)
plt.title('rpn_cls_loss') 
plt.plot(Iteration,rpn_cls_loss,c='green')

plt.subplot(255)
plt.title('rpn_loss_bbox') 
plt.plot(Iteration,rpn_loss_bbox,c='green')

##平均的loss图
n=50 #（每20次显示一次loss平均一次loss）20*n表示真正的,这里1000次平均
k=len(Iteration)/n
loss_avg_=[]
loss_bbox_=[]
loss_cls_=[]
rpn_cls_loss_=[]
rpn_loss_bbox_=[]

for i in range(k):
    loss_avg_.append(sum(loss[i*n:(i+1)*n])/n)
plt.subplot(256)
plt.title('loss_avg') 
plt.plot(range(k),loss_avg_,c='red')

for i in range(k):
    loss_bbox_.append(sum(loss_bbox[i*n:(i+1)*n])/n)
plt.subplot(257)
plt.title('loss_bbox_avg')
plt.plot(range(k),loss_bbox_,c='red')

for i in range(k):
    loss_cls_.append(sum(loss_cls[i*n:(i+1)*n])/n)
plt.subplot(258)
plt.title('loss_cls_avg') 
plt.plot(range(k),loss_cls_,c='red')

for i in range(k):
    rpn_cls_loss_.append(sum(rpn_cls_loss[i*n:(i+1)*n])/n)
plt.subplot(259)
plt.title('rpn_cls_loss_avg') 
plt.plot(range(k),rpn_cls_loss_,c='red')

for i in range(k):
    rpn_loss_bbox_.append(sum(rpn_loss_bbox[i*n:(i+1)*n])/n) 
plt.subplot(2,5,10 )
plt.title('rpn_loss_bbox_avg')
plt.plot(range(k),rpn_loss_bbox_,c='red')

plt.show()
f1.close()
```




