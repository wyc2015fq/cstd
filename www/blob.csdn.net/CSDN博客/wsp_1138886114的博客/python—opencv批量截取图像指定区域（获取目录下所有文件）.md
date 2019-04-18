# python—opencv批量截取图像指定区域（获取目录下所有文件） - wsp_1138886114的博客 - CSDN博客





2018年09月19日 21:23:59[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：4364








### 一、指定图像位置的裁剪处理

```python
import os   
import cv2 
 
# 遍历指定目录，显示目录下的所有文件名
def CropImage4File(filepath,destpath):
    pathDir =  os.listdir(filepath)    # 列出文件路径中的所有路径或文件
    for allDir in pathDir:
        child = os.path.join(filepath, allDir)
        dest = os.path.join(destpath,allDir)
        if os.path.isfile(child):
        	image = cv2.imread(child) 
            sp = image.shape            #获取图像形状：返回【行数值，列数值】列表
            sz1 = sp[0]                 #图像的高度（行 范围）
            sz2 = sp[1]                 #图像的宽度（列 范围）
            #sz3 = sp[2]                #像素值由【RGB】三原色组成
            
            #你想对文件的操作
            a=int(sz1/2-64) # x start
            b=int(sz1/2+64) # x end
            c=int(sz2/2-64) # y start
            d=int(sz2/2+64) # y end
            cropImg = image[a:b,c:d]   #裁剪图像
            cv2.imwrite(dest,cropImg)  #写入图像路径
           
if __name__ == '__main__':
    filepath ='F:\\\maomi'             #源图像
    destpath='F:\\maomi_resize'        # resized images saved here
    CropImage4File(filepath,destpath)
```

### 二、批量处理—指定图像位置的裁剪

我这个是用来截取发票的印章区域，用于图像分割（公司的数据集保密）

各位可以用自己的增值发票裁剪。适当的更改截取区域

```python
"""
处理数据集 和 标签数据集的代码：（主要是对原始数据集裁剪）
    处理方式：分别处理
    注意修改 输入 输出目录 和 生成的文件名
    output_dir = "./label_temp"
    input_dir = "./label"
"""

import cv2
import os
import sys

output_dir = "./temp_dir"           # 保存截取的图像目录
input_dir = "./original_image"      # 读取图片目录表
index = 1
for (path, dirnames, filenames) in os.walk(input_dir):
    for filename in filenames:
        print('正在处理图像： %s' % index)
        img_path = path+'/'+filename
        img = cv2.imread(img_path)
        gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)# 转为灰度图片
        weight_01 = gray_img.shape[1]
        print("图像宽度：",weight_01)
        if weight_01>1600:                           # 正常发票
            cropImg = gray_img[50:1050, 720:1720]    # 裁剪【行数据范围：列数据范围】
            cropImg = cv2.resize(cropImg, None, fx=0.5, fy=0.5,
                                 interpolation=cv2.INTER_CUBIC) #缩小图像
            cv2.imwrite(output_dir + '/' + str(index) + '.png', cropImg)
        else:                                        # 卷帘发票
            cropImg_01 = gray_img[40:540, 100:600]
            cropImg_02 = gray_img[600:1100, 80:580]
            cv2.imwrite(output_dir + '/cropImg_01_' + str(index) + '.png', cropImg_01)
            cv2.imwrite(output_dir + '/cropImg_02_' + str(index) + '.png', cropImg_02)

        index += 1
    key = cv2.waitKey(30) & 0xff                 # 不断刷新图像，频率时间为 30ms
    if key == 27:
        sys.exit(0)
```

```python
import cv2
import os

output_dir = "./face_dataset/test_faces"           # 保存截取的图像目录
input_dir = "./face_dataset/test_faces"      # 读取图片目录表
index = 1
for (path, dirnames, filenames) in os.walk(input_dir):
    for filename in filenames:
        print('正在处理图像： %s' % index)
        img_path = path+'/'+filename
        img = cv2.imread(img_path)
        weight = img.shape[1]
        print("图像宽度：",weight)
        if weight>1200:
            cropImg = img[:, 200:int(weight-200)]    # 裁剪【行数据范围：列数据范围】
            cropImg = cv2.resize(cropImg, None, fx=0.5, fy=0.5,
                                 interpolation=cv2.INTER_CUBIC) #缩小图像
            cv2.imwrite(output_dir + '/' +str(filename[:-4])+ str(index) + '.png', cropImg)
        else:
            pass
        index += 1
```



