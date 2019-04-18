# OpenCV—python 手写数字字体识别（使用HOG特征） - wsp_1138886114的博客 - CSDN博客





2018年10月15日 21:53:11[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1181











### 文章目录
- [一、训练分类器](#_1)
- [二、导入训练权重，使用图片检测](#_64)




### 一、训练分类器

步骤如下：
- 计算数据库中每个样本的HOG特征。
- 使用每个样本的HOG特征以及相应的标签训练多类线性SVM。
- 将分类器保存在文件中

由于是灰度图（二值化图）不能用图像的直方图来作为图像特征，这里用到了HOG方向梯度直方图，可以说，正合适。

数据集下载地址：[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)

当然，你也可以在线下载

```python
from sklearn.externals import joblib
from sklearn import datasets
from skimage.feature import hog
from sklearn.svm import LinearSVC
import numpy as np
import os
import struct
```

```python
# 在线下载特征获取代码
dataset = datasets.fetch_mldata("MNIST Original")
features = np.array(dataset.data, 'int16') 
labels = np.array(dataset.target, 'int')
```

离线下载数据集读取并训练

```python
def load_mnist(path):            #读取数据函数
    #Load MNIST data from path
    labels_path = os.path.join(path, 'train-labels.idx1-ubyte')
    images_path = os.path.join(path, 'train-images.idx3-ubyte')

    with open(labels_path, 'rb') as lbpath:
        magic, n = struct.unpack('>II',lbpath.read(8))
        labels = np.fromfile(lbpath, dtype=np.uint8)

    with open(images_path, 'rb') as imgpath:
        magic, num, rows, cols = struct.unpack(">IIII",imgpath.read(16))
        images = np.fromfile(imgpath, dtype=np.uint8).reshape(len(labels), 784)
    return images, labels

features,labels = load_mnist("./")
print('Rows: %d, columns: %d' % (features.shape[0], labels.shape[0]))

list_hog_fd = [] 
for feature in features:
    fd = hog(feature.reshape((28, 28)),     # hog 特征
             orientations=9, 
             pixels_per_cell=(14, 14), 
             cells_per_block=(1, 1), 
             visualise=False)
    list_hog_fd.append(fd)
hog_features = np.array(list_hog_fd, 'float64')

clf = LinearSVC()                                # 定义分类器
clf.fit(hog_features, labels)                    # 训练
joblib.dump(clf, "digits_cls.pkl", compress=3)   # 模型保存

# 压缩：0到9的整数可选
# 压缩级别：0没有压缩。越高意味着更多的压缩，而且读取和写入越慢。使用3的值通常是一个很好的折衷。
```

### 二、导入训练权重，使用图片检测

```python
import cv2
from sklearn.externals import joblib
from skimage.feature import hog
import numpy as np

clf = joblib.load("digits_cls.pkl")                 # 读取分类器
im = cv2.imread("./num.png")                        # 读取输入图片 
im_gray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)      # 灰度图化
im_gray = cv2.GaussianBlur(im_gray, (5, 5), 0)      # 高斯模糊（去噪）

ret, im_th = cv2.threshold(im_gray, 90, 255, cv2.THRESH_BINARY_INV)              # 阈值：二值化
ctrs, hier = cv2.findContours(im_th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE) # 查询图像轮廓 
rects = [cv2.boundingRect(ctr) for ctr in ctrs]    # 框出目标

# 对查询的目标识别：计算HOG特征图并且使用SVM预测数字
for rect in rects:
    cv2.rectangle(im, (rect[0], rect[1]), (rect[0] + rect[2], rect[1] + rect[3]), (0, 255, 0), 3) 
    leng = int(rect[3] * 1.6)
    pt1 = int(rect[1] + rect[3] // 2 - leng // 2)
    pt2 = int(rect[0] + rect[2] // 2 - leng // 2)
    roi = im_th[pt1:pt1+leng, pt2:pt2+leng]
    
    # resize 图片
    roi = cv2.resize(roi, (28, 28), interpolation=cv2.INTER_AREA)
    roi = cv2.dilate(roi, (3, 3))
    
    # 计算 HOG features
    roi_hog_fd = hog(roi, orientations=9, pixels_per_cell=(14, 14), cells_per_block=(1, 1), visualise=False)
    nbr = clf.predict(np.array([roi_hog_fd], 'float64'))
    cv2.putText(im, str(int(nbr[0])), (rect[0], rect[1]),cv2.FONT_HERSHEY_DUPLEX, 2, (0, 255, 255), 3)
cv2.imshow("Resulting Image with Rectangular ROIs", im)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

数字应该相互分开。否则，如果数字太接近，则会干扰每个数字周围的正方形区域。在这种情况下，我们需要创建一个新的方形图像，然后我们需要复制该方形图像中的轮廓。

对于我们在测试中使用的图像，固定阈值工作得很好。在大多数现实世界的图像中，固定阈值不会产生好的结果。在这种情况下，我们需要使用自适应阈值。

在预处理步骤中，我们只做了高斯模糊处理。在大多数情况下，在二进制图像上，我们需要打开和关闭图像以去除小的噪点像素并填充小孔。

特别鸣谢
[http://hanzratech.in/2015/02/24/handwritten-digit-recognition-using-opencv-sklearn-and-python.html](http://hanzratech.in/2015/02/24/handwritten-digit-recognition-using-opencv-sklearn-and-python.html)



