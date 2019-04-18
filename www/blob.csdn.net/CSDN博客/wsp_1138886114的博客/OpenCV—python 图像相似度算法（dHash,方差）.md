# OpenCV—python 图像相似度算法（dHash,方差） - wsp_1138886114的博客 - CSDN博客





2018年12月03日 20:57:02[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：464











### 文章目录
- [一、SIFT综述](#SIFT_1)
- [二、感知哈希算法(perceptual hash algorithm)](#perceptual_hash_algorithm_39)
- [2.1 相似图片检测步骤：](#21__47)
- [2.2 代码实现](#22__60)
- [三、使用方差检测图像相似度](#_127)
- [3.1 实现步骤](#31__128)




### 一、SIFT综述

尺度不变特征转换(Scale-invariant feature transform或SIFT)是一种电脑视觉的算法用来侦测与描述影像中的局部性特征，它在空间尺度中寻找极值点，并提取出其位置、尺度、旋转不变量，此算法由 David Lowe在1999年所发表，2004年完善总结。

其应用范围包含物体辨识、机器人地图感知与导航、影像缝合、3D模型建立、手势辨识、影像追踪和动作比对。

局部影像特征的描述与侦测可以帮助辨识物体，SIFT 特征是基于物体上的一些局部外观的兴趣点而与影像的大小和旋转无关。对于光线、噪声、些微视角改变的容忍度也相当高。基于这些特性，它们是高度显著而且相对容易撷取，在母数庞大的特征数据库中，很容易辨识物体而且鲜有误认。使用 SIFT特征描述对于部分物体遮蔽的侦测率也相当高，甚至只需要3个以上的SIFT物体特征就足以计算出位置与方位。在现今的电脑硬件速度下和小型的特征数据库条件下，辨识速度可接近即时运算。SIFT特征的信息量大，适合在海量数据库中快速准确匹配。

**SIFT算法的特点**：
- SIFT特征是图像的局部特征，其对旋转、尺度缩放、亮度变化保持不变性，对视角变化、仿射变换、噪声也保持一定程度的稳定性；
- 独特性（Distinctiveness）好，信息量丰富，适用于在海量特征数据库中进行快速、准确的匹配；
- 多量性，即使少数的几个物体也可以产生大量的SIFT特征向量；
- 高速性，经优化的SIFT匹配算法甚至可以达到实时的要求；
- 可扩展性，可以很方便的与其他形式的特征向量进行联合。

**SIFT算法可以解决的问题**：

目标的自身状态、场景所处的环境和成像器材的成像特性等因素影响图像配准/目标识别跟踪的性能。而SIFT算法在一定程度上可解决：
- 目标的旋转、缩放、平移（RST）
- 图像仿射/投影变换（视点viewpoint）
- 光照影响（illumination）
- 目标遮挡（occlusion）
- 杂物场景（clutter）
- 噪声

SIFT算法的实质是在**不同的尺度空间上查找关键点(特征点)**，并计算出关键点的方向。SIFT所查找到的关键点是一些十分突出，不会因光照，仿射变换和噪音等因素而变化的点，如角点、边缘点、暗区的亮点及亮区的暗点等。

SIFT算法分解为如下四步：
- 尺度空间极值检测：搜索所有尺度上的图像位置。通过高斯微分函数来识别潜在的对于尺度和旋转不变的兴趣点。
- 关键点定位：在每个候选的位置上，通过一个拟合精细的模型来确定位置和尺度。关键点的选择依据于它们的稳定程度。
- 方向确定：基于图像局部的梯度方向，分配给每个关键点位置一个或多个方向。所有后面的对图像数据的操作都相对于关键点的方向、尺度和位置进行变换，从而提供对于这些变换的不变性。
- 关键点描述：在每个关键点周围的邻域内，在选定的尺度上测量图像局部的梯度。这些梯度被变换成一种表示，这种表示允许比较大的局部形状的变形和光照变化。

**SIFT的缺点**

SIFT在图像的不变特征提取方面拥有无与伦比的优势，但并不完美，仍然存在：
- 实时性不高。
- 有时特征点较少。
- 对边缘光滑的目标无法准确提取特征点。

### 二、感知哈希算法(perceptual hash algorithm)

它的作用是对每张图像生成一个“指纹”(fingerprint)字符串，然后比较不同图像的指纹。结果越接近，就说明图像越相似。

Hash算法
- aHash：平均值哈希。速度比较快，但是常常不太精确。
- pHash：感知哈希。精确度比较高，但是速度方面较差一些。
- **dHash**：差异值哈希。精确度较高，且速度也非常快。

##### 2.1 相似图片检测步骤：
- 分别计算两张图片的dHash值
- 通过dHash值计算两张图片的汉明距离（Hamming Distance），通过汉明距离的大小，判断两张图片的相似程度。

**实现步骤：**
- **缩小尺寸**：将图像缩小到8*8的尺寸，总共64个像素。这一步的作用是去除图像的细节，只保留结构/明暗等基本信息，摒弃不同尺寸/比例带来的图像差异；
- **灰度化**：减少计算量；
- **比较像素的灰度**：如果前一个像素的颜色强度大于第二个像素，那么差异值就设置为True（也就是1），如果不大于第二个像素，就设置为False（也就是0）
- **计算哈希值**：将差异值数组中每一个值看做一个bit，每8个bit组成为一个16进制值，将16进制值连接起来转换为字符串，就得出了最后的dHash值。；
- **计算汉明距离（Hamming Distance）** 对比不同的图像，看看64位中有多少位是不一样的。在理论上，这等同于”**汉明距离**”(Hamming distance,在信息论中，两个等长字符串之间的汉明距离是两个字符串对应位置的不同字符的个数)。如果不相同的数据位数不超过5，就说明两张图像很相似；如果大于10，就说明这是两张不同的图像。

##### 2.2 代码实现

当然你也可以使用这个代码 [https://github.com/hjaurum/DHash/blob/master/dHash.py](https://github.com/hjaurum/DHash/blob/master/dHash.py)

```python
import cv2

#均值哈希算法
def aHash(img):
    img=cv2.resize(img,(8,8),interpolation=cv2.INTER_CUBIC)
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    #s为像素和初值为0，hash_str为hash值初值为''
    s=0
    ahash_str=''
    for i in range(8):                  #遍历累加求像素和
        for j in range(8):
            s=s+gray[i,j]
    avg=s/64                            #求平均灰度
    for i in range(8):                  #灰度大于平均值为1相反为0生成图片的hash值
        for j in range(8):
            if  gray[i,j]>avg:
                ahash_str=ahash_str+'1'
            else:
                ahash_str=ahash_str+'0'
    return ahash_str


#差值感知算法
def dHash(img):
    img=cv2.resize(img,(9,8),interpolation=cv2.INTER_CUBIC)
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    dhash_str=''
    for i in range(8):                #每行前一个像素大于后一个像素为1，相反为0，生成哈希
        for j in range(8):
            if gray[i,j]>gray[i,j+1]:
                dhash_str = dhash_str+'1'
            else:
                dhash_str = dhash_str+'0'
    return dhash_str

def cmpHash(hash1,hash2):              #Hash值对比
    n=0
    if len(hash1)!=len(hash2):         #hash长度不同则返回-1代表传参出错
        return -1
    for i in range(len(hash1)):        #遍历判断
        if hash1[i]!=hash2[i]:         #不相等则n计数+1，n最终为相似度
            n=n+1
    return n


if __name__ == '__main__':
    img1=cv2.imread('lalala.jpg')
    img2=cv2.imread('lalala1.jpg')
    hash1= aHash(img1)
    hash2= aHash(img2)
    print(hash1)
    print(hash2)
    n=cmpHash(hash1,hash2)
    print('均值哈希算法相似度：',n)


    hash1= dHash(img1)
    hash2= dHash(img2)
    print(hash1)
    print(hash2)
    n=cmpHash(hash1,hash2)
    print('差值哈希算法相似度：',n)
```

### 三、使用方差检测图像相似度

##### 3.1 实现步骤
- **缩放图片**

将需要处理的图片所放到指定尺寸，根据自己需求，弹性的缩放。在效率和准确度之间维持平衡。
- **灰度化**

通常对比图像相似度和颜色关系不是很大，所以处理为灰度图，减少后期计算的复杂度。如果有特殊需求则保留图像色彩。
- **计算平均值**

此处开始，与传统的哈希算法不同：分别依次计算图像每行像素点的平均值，记录每行像素点的平均值。每一个平均值对应着一行的特征。
- **计算方差**

对得到的所有平均值进行计算方差，得到的方差就是图像的特征值。方差可以很好的反应每行像素特征的波动，既记录了图片的主要信息。计算两张图每行平均值的差再平方
- **比较方差**

计算之后，每张图都会生成一个特征值（方差）。比较图像相似度就是比较图像生成方差的接近成程度。一组数据方差的大小可以判断稳定性，多组数据方差的接近程度可以反应数据波动的接近程度。我们只关注两个**方差的差值**的大小。方差差值越小图像越相似！

```python
import cv2
import matplotlib.pyplot as plt


def getss(list):				     #计算方差
    avg=sum(list)/len(list)          #计算平均值
    ss=0
    for l in list:                   #计算方差
        ss+=(l-avg)*(l-avg)/len(list)
    return ss


def getdiff(img):                     #获取每行像素平均值
    Sidelength=32                     #定义边长
    img=cv2.resize(img,(Sidelength,Sidelength),interpolation=cv2.INTER_CUBIC)
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    avglist=[]                        #avglist列表保存每行像素平均值
    for i in range(Sidelength):       #计算每行均值，保存到avglist列表
        avg=sum(gray[i])/len(gray[i])
        avglist.append(avg)
    return avglist

#读取测试图片
img1=cv2.imread("./lalala.jpg")
diff1=getdiff(img1)
print('img1:',getss(diff1))

#读取测试图片
img11=cv2.imread("./lalala1.jpg")
diff11=getdiff(img11)
print('img11:',getss(diff11))

x=range(32)

plt.figure("avg")
plt.plot(x,diff1,marker="*",label="$walk01$")
plt.plot(x,diff11,marker="*",label="$walk03$")
plt.title("avg")
plt.legend()
plt.show()
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181203201837350.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181203201952959.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0,size_16,color_FFFFFF,t_70)
```python
import cv2
import os
import time
from time import ctime
import threading



def getdiff(img):                     #获取每行像素平均值
    Sidelength=32                     #定义边长
    img=cv2.resize(img,(Sidelength,Sidelength),interpolation=cv2.INTER_CUBIC)
    gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    avglist=[]                        #avglist列表保存每行像素平均值
    for i in range(Sidelength):       #计算每行均值，保存到avglist列表
        avg=sum(gray[i])/len(gray[i])
        avglist.append(avg)
    return avglist

def read_img(input_dir):             #获取所有图片
    img_paths = []
    for (path,dirname,filenames) in os.walk(input_dir):
        for filename in filenames:
            img_paths.append(path+"/"+filename)
    print("img_paths",img_paths)
    return img_paths

################## 此处函数使用多线程用来加快运算 ####################
def get_img_avg(n,img_paths):
    img_avg_list = []
    for i in range(n,len(img_paths),2):
        img=cv2.imread(img_paths[i])
        img_avg_list.append(getdiff(img))
    return img_avg_list
###################################################################

def double_img_all(diff_list_all,n):
    sum_All = []
    sum_all = []
    for i in range(len(diff_list_all)):
        for j in range(len(diff_list_all[i])):
            for k in range(i + 1, (len(diff_list_all))):
                diffe_squra = (diff_list_all[i][j] - diff_list_all[k][j]) ** 2
                sum_all.append(diffe_squra)
    if n>=0 and n<len(diff_list_all):
        for i in range(len(diff_list_all)):
            sum_All.append(sum_all[(len(diff_list_all)-1)*i+n])
    else:
        print("error number")
    return sum_all,sum_All

class MyThread(threading.Thread):    # 线程类
    def __init__(self, func, args, name=''):
        threading.Thread.__init__(self)
        self.name = name
        self.func = func
        self.args = args
        self.result = self.func(*self.args)

    def get_result(self):
        try:
            return self.result
        except Exception:
            return None


def main():                       #主函数
    print('start at', ctime())
    start_time = time.time()
    input_dir = "./content"
    img_paths = read_img(input_dir)
    diff_list_all = []
    threads = []
    n =[0,1]
    for i in n:
        t = MyThread(get_img_avg, (i,img_paths,), get_img_avg.__name__)
        threads.append(t)
    for i in range(len(threads)):  # 将任务分发到每个线程，完成后再开始执行start方法
        threads[i].setDaemon(True)
        threads[i].start()
    for i in range(len(threads)):  # join()方法等待线程完成
        threads[i].join()
    print("threads[0]:",threads[0].get_result())
    print("threads[1]:",threads[1].get_result())
    img_avg_list1 = threads[0].get_result()
    img_avg_list2 = threads[1].get_result()
    diff_list_all.extend((img_avg_list1,img_avg_list2))

    # 遍历输出： 数组1与数组2的计算值;数组1与数组3的计算值...
    sum_all_data = []
    for n in range(len(diff_list_all[0]) - 1):
        sum_all, sum_All = double_img_all(diff_list_all[0], n)
        sum_all_data.append(sum_all)
        print('sum_All:', sum_All)
    print('sum_all:', sum_all_data)
    end_time = time.time()
    print("done!  Total Spend time：", str((end_time - start_time) / 60)[0:6] + "分钟")


if __name__ == '__main__':
    main()
```

鸣谢
[https://www.jianshu.com/p/193f0089b7a2](https://www.jianshu.com/p/193f0089b7a2)
[https://blog.csdn.net/haofan_/article/details/78806143](https://blog.csdn.net/haofan_/article/details/78806143)




