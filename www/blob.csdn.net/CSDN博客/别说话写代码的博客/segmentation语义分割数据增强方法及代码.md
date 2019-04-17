# segmentation语义分割数据增强方法及代码 - 别说话写代码的博客 - CSDN博客





2019年01月19日 20:50:35[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：331标签：[语义分割数据增强																[segmentation data augmentation](https://so.csdn.net/so/search/s.do?q=segmentation data augmentation&t=blog)](https://so.csdn.net/so/search/s.do?q=语义分割数据增强&t=blog)
个人分类：[计算机视觉](https://blog.csdn.net/qq_21997625/article/category/7782165)








```python
# -*- coding:utf-8 -*-
"""数据增强
   1. 翻转变换 flip
   2. 随机修剪 random crop
   3. 色彩抖动 color jittering
   4. 平移变换 shift
   5. 尺度变换 scale
   6. 对比度变换 contrast
   7. 噪声扰动 noise
   8. 旋转变换/反射变换 Rotation/reflection
"""
 
from PIL import Image, ImageEnhance, ImageOps, ImageFile
import numpy as np
import random
import threading, os, time
import logging
 
logger = logging.getLogger(__name__)
ImageFile.LOAD_TRUNCATED_IMAGES = True
 
 
class DataAugmentation:
    """
    包含数据增强的八种方式
    """
 
 
    def __init__(self):
        pass
 
    @staticmethod
    def openImage(image):
        return Image.open(image, mode="r")
 
    @staticmethod
    def randomRotation(image, label, mode=Image.BICUBIC):
        """
         对图像进行随机任意角度(0~360度)旋转
        :param mode 邻近插值,双线性插值,双三次B样条插值(default)
        :param image PIL的图像image
        :return: 旋转转之后的图像
        """
        random_angle = np.random.randint(1, 360)
        return image.rotate(random_angle, mode) , label.rotate(random_angle, Image.NEAREST)
 
    #暂时未使用这个函数
    @staticmethod
    def randomCrop(image, label):
        """
        对图像随意剪切,考虑到图像大小范围(68,68),使用一个一个大于(36*36)的窗口进行截图
        :param image: PIL的图像image
        :return: 剪切之后的图像
        """
        image_width = image.size[0]
        image_height = image.size[1]
        crop_win_size = np.random.randint(40, 68)
        random_region = (
            (image_width - crop_win_size) >> 1, (image_height - crop_win_size) >> 1, (image_width + crop_win_size) >> 1,
            (image_height + crop_win_size) >> 1)
        return image.crop(random_region), label
 
    @staticmethod
    def randomColor(image, label):
        """
        对图像进行颜色抖动
        :param image: PIL的图像image
        :return: 有颜色色差的图像image
        """
        random_factor = np.random.randint(0, 31) / 10.  # 随机因子
        color_image = ImageEnhance.Color(image).enhance(random_factor)  # 调整图像的饱和度
        random_factor = np.random.randint(10, 21) / 10.  # 随机因子
        brightness_image = ImageEnhance.Brightness(color_image).enhance(random_factor)  # 调整图像的亮度
        random_factor = np.random.randint(10, 21) / 10.  # 随机因1子
        contrast_image = ImageEnhance.Contrast(brightness_image).enhance(random_factor)  # 调整图像对比度
        random_factor = np.random.randint(0, 31) / 10.  # 随机因子
        return ImageEnhance.Sharpness(contrast_image).enhance(random_factor) ,label # 调整图像锐度
 
    @staticmethod
    def randomGaussian(image, label, mean=0.2, sigma=0.3):
        """
         对图像进行高斯噪声处理
        :param image:
        :return:
        """
 
        def gaussianNoisy(im, mean=0.2, sigma=0.3):
            """
            对图像做高斯噪音处理
            :param im: 单通道图像
            :param mean: 偏移量
            :param sigma: 标准差
            :return:
            """
            for _i in range(len(im)):
                im[_i] += random.gauss(mean, sigma)
            return im
 
        # 将图像转化成数组
        img = np.asarray(image)
        img.flags.writeable = True  # 将数组改为读写模式
        width, height = img.shape[:2]
        img_r = gaussianNoisy(img[:, :, 0].flatten(), mean, sigma)
        img_g = gaussianNoisy(img[:, :, 1].flatten(), mean, sigma)
        img_b = gaussianNoisy(img[:, :, 2].flatten(), mean, sigma)
        img[:, :, 0] = img_r.reshape([width, height])
        img[:, :, 1] = img_g.reshape([width, height])
        img[:, :, 2] = img_b.reshape([width, height])
        return Image.fromarray(np.uint8(img)), label
 
    @staticmethod
    def saveImage(image, path):
        image.save(path)
 
 
def makeDir(path):
    try:
        if not os.path.exists(path):
            if not os.path.isfile(path):
                # os.mkdir(path)
                os.makedirs(path)
            return 0
        else:
            return 1
    except Exception, e:
        print str(e)
        return -2
 
 
def imageOps(func_name, image, label, img_des_path, label_des_path , img_file_name, label_file_name, times=5):
    funcMap = {"randomRotation": DataAugmentation.randomRotation,
               "randomCrop": DataAugmentation.randomCrop,
               "randomColor": DataAugmentation.randomColor,
               "randomGaussian": DataAugmentation.randomGaussian
               }
    if funcMap.get(func_name) is None:
        logger.error("%s is not exist", func_name)
        return -1
 
    for _i in range(0, times, 1):
        new_image , new_label = funcMap[func_name](image,label)
        DataAugmentation.saveImage(new_image, os.path.join(img_des_path, func_name + str(_i) + img_file_name))
        DataAugmentation.saveImage(new_label, os.path.join(label_des_path, func_name + str(_i) + label_file_name))
 
 
opsList = {"randomRotation",  "randomColor", "randomGaussian"}
 
 
def threadOPS(img_path, new_img_path, label_path, new_label_path):
    """
    多线程处理事务
    :param src_path: 资源文件
    :param des_path: 目的地文件
    :return:
    """
    #img path 
    if os.path.isdir(img_path):
        img_names = os.listdir(img_path)
    else:
        img_names = [img_path]
 
    #label path 
    if os.path.isdir(label_path):
        label_names = os.listdir(label_path)
    else:
        label_names = [label_path]
 
    img_num = 0
    label_num = 0
 
    #img num
    for img_name in img_names:
        tmp_img_name = os.path.join(img_path, img_name)
        if os.path.isdir(tmp_img_name):
            print('contain file folder')
            exit()
        else:
            img_num = img_num + 1;
    #label num
    for label_name in label_names:
        tmp_label_name = os.path.join(label_path, label_name)
        if os.path.isdir(tmp_label_name):
            print('contain file folder')
            exit()
        else:
            label_num = label_num + 1
 
    if img_num != label_num:
        print('the num of img and label is not equl')
        exit()
    else: 
        num = img_num
 
 
    for i in range(num):
        img_name = img_names[i]
        print img_name
        label_name = label_names[i]
        print label_name
 
        tmp_img_name = os.path.join(img_path, img_name)
        tmp_label_name = os.path.join(label_path, label_name)
 
        # 读取文件并进行操作
        image = DataAugmentation.openImage(tmp_img_name)
        label = DataAugmentation.openImage(tmp_label_name)
 
        threadImage = [0] * 5
        _index = 0
        for ops_name in opsList:
            threadImage[_index] = threading.Thread(target=imageOps,
                                                    args=(ops_name, image, label, new_img_path, new_label_path, img_name, label_name))
            threadImage[_index].start()
            _index += 1
            time.sleep(0.2)
 
 
if __name__ == '__main__':
    threadOPS("/data1/qixinyuan/data/datasets/little/img",
              "/data1/qixinyuan/data/datasets/little/new_img",
              "/data1/qixinyuan/data/datasets/little/label",
              "/data1/qixinyuan/data/datasets/little/new_label")
```

还有一种

## 安装Augmentor

```python
pip install Augmentor
```

### 对图片进行随机旋转

```python
import Augmentor
p = Augmentor.Pipeline("/path/to/images")
p.rotate(probability=1, max_left_rotation=5, max_right_rotation=5) #probability表示以一定概率随机处理图片
p.sample(500) #产生500张图片
```

### image and ground truth data can be identically augmented

```python
p = Augmentor.Pipeline("/path/to/images")
# Point to a directory containing ground truth data.
# Images with the same file names will be added as ground truth data
# and augmented in parallel to the original data.
p.ground_truth("/path/to/ground_truth_images")
# Add operations to the pipeline as normal:
p.rotate(probability=1, max_left_rotation=5, max_right_rotation=5)
p.flip_left_right(probability=0.5)
p.zoom_random(probability=0.5, percentage_area=0.8)
p.flip_top_bottom(probability=0.5)
p.sample(50)
```

![这里写图片描述](https://img-blog.csdn.net/20180309154932407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaml1bGlhbmcxOTE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在旋转图片时,常常会在图片周围产生空白填充,如图 

![这里写图片描述](https://img-blog.csdn.net/20180309160021054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaml1bGlhbmcxOTE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20180309155949257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaml1bGlhbmcxOTE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

遇到这种情况,Augmentor会在旋转的时候同时缩放图片,不致在四周出现黑色填充 

![这里写图片描述](https://img-blog.csdn.net/20180309160251095?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaml1bGlhbmcxOTE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 问题

在使用 

ground_truth()函数时,如果路径中有多张图片,将会导致augment之后的mask和image不对应,因此只能在路径中存放一张图片,如果有很多组数据需要augment则需要将他们单个存放在文件夹中

下面是我的代码:

```python
# -*- coding: utf-8 -*-
import Augmentor
import glob
import os
import random
from keras.preprocessing.image import ImageDataGenerator, array_to_img, img_to_array, load_img

train_path = 'train'
groud_truth_path = 'mask'
img_type = 'jpg'
train_tmp_path = 'tmp/train'
mask_tmp_path = 'tmp/mask'

def start(train_path,groud_truth_path):
    train_img = glob.glob(train_path+'/*.'+img_type)
    masks = glob.glob(groud_truth_path+'/*.'+img_type)

    if len(train_img) != len(masks):
        print ("trains can't match masks")
        return 0
    for i in range(len(train_img)):
        train_img_tmp_path = train_tmp_path + '/'+str(i)
        if not os.path.lexists(train_img_tmp_path):
            os.mkdir(train_img_tmp_path)
        img = load_img(train_path+'/'+str(i)+'.'+img_type)
        x_t = img_to_array(img)
        img_tmp = array_to_img(x_t)
        img_tmp.save(train_img_tmp_path+'/'+str(i)+'.'+img_type)

        mask_img_tmp_path =mask_tmp_path +'/'+str(i)
        if not os.path.lexists(mask_img_tmp_path):
            os.mkdir(mask_img_tmp_path)
        mask = load_img(groud_truth_path+'/'+str(i)+'.'+img_type)
        x_l = img_to_array(mask)
        mask_tmp = array_to_img(x_l)
        mask_tmp.save(mask_img_tmp_path+'/'+str(i)+'.'+img_type)
        print ("%s folder has been created!"%str(i))
    return i+1


def doAugment(num):
    sum = 0
    for i in range(num):
        p = Augmentor.Pipeline(train_tmp_path+'/'+str(i))
        p.ground_truth(mask_tmp_path+'/'+str(i))
        p.rotate(probability=0.5, max_left_rotation=5, max_right_rotation=5)#旋转
        p.flip_left_right(probability=0.5)#按概率左右翻转
        p.zoom_random(probability=0.6, percentage_area=0.99)#随即将一定比例面积的图形放大至全图
        p.flip_top_bottom(probability=0.6)#按概率随即上下翻转
        p.random_distortion(probability=0.8,grid_width=10,grid_height=10, magnitude=20)#小块变形
        count = random.randint(40, 60)
        print("\nNo.%s data is being augmented and %s data will be created"%(i,count))
        sum = sum + count
        p.sample(count)
        print("Done")
    print("%s pairs of data has been created totally"%sum)


a = start(train_path, groud_truth_path)
doAugment(a)
```

参考：

[https://blog.csdn.net/qq_20852429/article/details/79137777](https://blog.csdn.net/qq_20852429/article/details/79137777)

[https://blog.csdn.net/jiuliang1916/article/details/79498885](https://blog.csdn.net/jiuliang1916/article/details/79498885)

[https://github.com/mdbloice/Augmentor](https://github.com/mdbloice/Augmentor)




