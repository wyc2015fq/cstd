# 用python对minist数据集进行读取并保存成图片 - sinat_31425585的博客 - CSDN博客
2016年09月27日 10:35:20[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：3960
  记忆力差的孩纸得勤做笔记！
参考博客[http://www.cnblogs.com/x1957/archive/2012/06/02/2531503.html](http://www.cnblogs.com/x1957/archive/2012/06/02/2531503.html)，最近想要拿minist数据集练练手，想要将minsit数据集转换成图片格式，参考了上面博客后，有了下面这个demo，给大家分享下！好了，多的不说直接上代码！
```python
#coding:utf-8
import numpy as np
import struct
import matplotlib.pyplot as plt
from scipy.misc import imsave
 
filename = 'train-images.idx3-ubyte'
binfile = open(filename , 'rb')
buf = binfile.read()
index = 0
#'>IIII'使用大端法读取四个unsigned int32
magic, numImages , numRows , numColumns = struct.unpack_from('>IIII' , buf , index)
index += struct.calcsize('>IIII')
# 输出大端数
print magic
print numImages
print numRows
print numColumns
for i in range(60000):
	name = str(i) + ".jpg"
	# upack_from从流中截取784位数据（图片像素值） 
	im = struct.unpack_from('>784B' ,buf, index)
	index += struct.calcsize('>784B')
	im = np.array(im)
	im = im.reshape(28,28)
	imsave(name, im)
binfile.close()
 
# fig = plt.figure()
# plotwindow = fig.add_subplot(111)
# plt.imshow(im , cmap='gray')
# plt.show()
```
下面是读取label的demo，最后label会在label.txt文件夹中以列的形式存在。
```python
#coding:utf-8
import numpy as np
import struct
# import matplotlib.pyplot as plt
# from scipy.misc import imsave
 
#filename = 'dat//train-images.idx3-ubyte'
filename =  'dat//train-labels.idx1-ubyte'
binfile = open(filename , 'rb')
buf = binfile.read()
f1 = open('label.txt', 'w')
index = 0
#'>IIII'使用大端法读取两个unsigned int32
magic, numLabels = struct.unpack_from('>II' , buf , index)
index += struct.calcsize('>II')
# 输出大端数
print magic
print numLabels
# for i in range(numLabels):
for i in range(numLabels):
	numtemp = struct.unpack_from('1B' ,buf, index)
	# numtemp 为tuple类型，读取其数值
	num = numtemp[0]
	# 存入label.txt文件中
	f1.write(str(num))
	
	f1.write('\n')
	index += struct.calcsize('1B')
	print num
	
binfile.close()
 
# fig = plt.figure()
# plotwindow = fig.add_subplot(111)
# plt.imshow(im , cmap='gray')
# plt.show()
```
打完收工！demo下载地址：[点击打开链接](http://download.csdn.net/detail/sinat_31425585/9641121)
