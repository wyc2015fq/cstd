# python获取所有图像像素最大值缩放图片到指定大小并填充像素 - 别说话写代码的博客 - CSDN博客





2018年06月13日 15:17:34[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：1688








```python
见函数getmaxpixel()与setpic_pixel()
```

```python

```

```python
#-*- coding:utf-8 -*-
import pandas as pd
import numpy as np
import pdb
from PIL import Image,ImageEnhance
import os
dic  = {}
import os
import shutil
import numpy as np
import pandas as pd
from PIL import Image
import os
def getmaxpixel():   #获取所有图像中最大像素值
    rootdir = '/home/shaoyidu/Program/pythonCnn/baidu/dataset/train0'
    list = os.listdir(rootdir)
    h=0
    w=0
    for i in range(0,len(list)):
        path = os.path.join(rootdir,list[i])
        if os.path.isfile(path):
            img = Image.open(path)
            imgSize = img.size #图片的长和宽
            if(imgSize[0]>w):
                w = imgSize[0]
            if (imgSize[1] > h):
                h = imgSize[1]
    print (w,h)
    return w,h

def setpic_pixel(maxw,maxh):  #缩放图片并填充
    imglst = os.listdir('/home/shaoyidu/Program/pythonCnn/baidu/dataset/train0')

    for name in imglst:

        # 打开一个jpg图像文件，注意路径要改成你自己的:

        im = Image.open('/home/shaoyidu/Program/pythonCnn/baidu/dataset/train0/%s' % name)
        #im = im.resize((1024,48),PIL.Image.ANTIALIAS)
        # 制作最大尺寸 1024*48 背景用白色填充
        newim = Image.new('RGB', (maxw, maxh), 'white')
        w, h = im.size
        scale= min(maxw/w,maxh/h)
        print(w,h)
        out = im.resize((int(w*scale),int(h*scale)))
        newim.paste(out,(0,0))
        #newim.show()
        newim.save('/home/dataset/train_max0/%s' % name)
        '''
        im = Image.open('E:/allcompetition/test_f/%s' % name)
        im.show()
        enhancer = ImageEnhance.Contrast(im)
        out=enhancer.enhance(3)
        out.save('./%s' % name)
        #brightness
        #time.sleep(1000)
        #print(w,h)
        #if w<h:
        #    f.write('%d %d %s\n' % (w,h,name))
        if (w,h) not in dic:
            dic[(w,h)] = [name,]
            if w < h:
    
        else:
            dic[(w,h)].append(name)
        '''



w,h = getmaxpixel()
setpic_pixel(w,h)
#label2picture()
```




