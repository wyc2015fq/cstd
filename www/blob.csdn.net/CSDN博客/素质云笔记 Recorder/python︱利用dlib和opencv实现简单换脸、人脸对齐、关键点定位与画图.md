
# python︱利用dlib和opencv实现简单换脸、人脸对齐、关键点定位与画图 - 素质云笔记-Recorder... - CSDN博客

2017年11月17日 18:48:12[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7658


这是一个利用dlib进行关键点定位 + opencv处理的人脸对齐、换脸、关键点识别的小demo。原文来自于[《Switching Eds: Face swapping with Python, dlib, and OpenCV》](https://matthewearl.github.io/2015/07/28/switching-eds-with-python/)
该博文的[github](https://github.com/matthewearl/faceswap/blob/master/faceswap.py)地址中有所有的code。这边我将我抽取的code放在自己的github之中，可以来这下载:
[https://github.com/mattzheng/Face_Swapping](https://github.com/mattzheng/Face_Swapping)
有人将其进行[中文翻译](http://python.jobbole.com/82546/)也有将其进行一定改编有以下两个案例：
1.[《川普撞脸希拉里(基于 OpenCV 的面部特征交换)-2》](http://blog.csdn.net/oxuzhenyi/article/details/54982632)
[变脸](http://messcode.github.io/2016/04/17/switch-faces-using-python/)
### 变脸贴图：
从这几张：
![这里写图片描述](https://img-blog.csdn.net/20171117190501758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117190501758?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171117190521804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117190521804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171117190537959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
组合变成这几张：
![这里写图片描述](https://img-blog.csdn.net/20171117190546474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117190546474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171117190554152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117190554152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171117190601451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171117190601451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
因为原文里面内容丰富，我觉得可以提取出很多有用的小模块，于是乎：
.
# 提取一：关键点定位与画图
```python
import
```
```python
cv2
```
```python
import
```
```python
dlib
```
```python
import
```
```python
numpy
```
```python
import
```
```python
sys
```
```python
import
```
```python
matplotlib.pyplot
```
```python
as
```
```python
plt
SCALE_FACTOR =
```
```python
1
```
```python
# 图像的放缩比
```
```python
def
```
```python
read_im_and_landmarks
```
```python
(fname)
```
```python
:
```
```python
im = cv2.imread(fname, cv2.IMREAD_COLOR)
    im = cv2.resize(im, (im.shape[
```
```python
1
```
```python
] * SCALE_FACTOR,
                         im.shape[
```
```python
0
```
```python
] * SCALE_FACTOR))
    s = get_landmarks(im)
```
```python
return
```
```python
im, s
```
```python
def
```
```python
annotate_landmarks
```
```python
(im, landmarks)
```
```python
:
```
```python
'''
    人脸关键点，画图函数
    '''
```
```python
im = im.copy()
```
```python
for
```
```python
idx, point
```
```python
in
```
```python
enumerate(landmarks):
        pos = (point[
```
```python
0
```
```python
,
```
```python
0
```
```python
], point[
```
```python
0
```
```python
,
```
```python
1
```
```python
])
        cv2.putText(im, str(idx), pos,
                    fontFace=cv2.FONT_HERSHEY_SCRIPT_SIMPLEX,
                    fontScale=
```
```python
0.4
```
```python
,
                    color=(
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
255
```
```python
))
        cv2.circle(im, pos,
```
```python
3
```
```python
, color=(
```
```python
0
```
```python
,
```
```python
255
```
```python
,
```
```python
255
```
```python
))
```
```python
return
```
```python
im
```
然后实践就是载入原图：
`im1, landmarks1 =``read``_im_and_landmarks(``'02.jpg'``)``# 底图``im1 = annotate_landmarks(im1, landmarks1)
%matplotlib inline
plt.subplot(``111``)
plt.imshow(im1)`.
# 提取二：人脸对齐
需要一张模板图来作为靠拢的对象图。
```python
# 人脸对齐函数
```
```python
def
```
```python
face_Align
```
```python
(Base_path,cover_path)
```
```python
:
```
```python
im1, landmarks1 = read_im_and_landmarks(Base_path)
```
```python
# 底图
```
```python
im2, landmarks2 = read_im_and_landmarks(cover_path)
```
```python
# 贴上来的图
```
```python
if
```
```python
len(landmarks1) ==
```
```python
0
```
```python
& len(landmarks2) ==
```
```python
0
```
```python
:
```
```python
raise
```
```python
ImproperNumber(
```
```python
"Faces detected is no face!"
```
```python
)
```
```python
if
```
```python
len(landmarks1) >
```
```python
1
```
```python
& len(landmarks2) >
```
```python
1
```
```python
:
```
```python
raise
```
```python
ImproperNumber(
```
```python
"Faces detected is more than 1!"
```
```python
)
    M = transformation_from_points(landmarks1[ALIGN_POINTS],
                                   landmarks2[ALIGN_POINTS])
    warped_im2 = warp_im(im2, M, im1.shape)
```
```python
return
```
```python
warped_im2
```
#### 这里的步骤是：
提取模板图、对齐图的landmarks;
通过transformation_from_points计算对齐图向模板图的转移矩阵M，变换矩阵是根据以下公式计算出来的;
warp_im，将 im2 的掩码进行变化，使之与 im1 相符
实践的话就是：
`FEATHER_AMOUNT =``19``# 匹配的时候，特征数量，现在是以``11``个点为基准点``11``15``17``Base_path =``'``01``.jpg'``cover_path =``'``02``.jpg'``warped_mask =``face_Align(Base_path,cover_path)`![这里写图片描述](https://img-blog.csdn.net/20171117190752424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
.
# 提取三：换脸
主要函数：
```python
def
```
```python
Switch_face
```
```python
(Base_path,cover_path)
```
```python
:
```
```python
im1, landmarks1 = read_im_and_landmarks(Base_path)
```
```python
# 底图
```
```python
im2, landmarks2 = read_im_and_landmarks(cover_path)
```
```python
# 贴上来的图
```
```python
if
```
```python
len(landmarks1) ==
```
```python
0
```
```python
& len(landmarks2) ==
```
```python
0
```
```python
:
```
```python
raise
```
```python
ImproperNumber(
```
```python
"Faces detected is no face!"
```
```python
)
```
```python
if
```
```python
len(landmarks1) >
```
```python
1
```
```python
& len(landmarks2) >
```
```python
1
```
```python
:
```
```python
raise
```
```python
ImproperNumber(
```
```python
"Faces detected is more than 1!"
```
```python
)
    M = transformation_from_points(landmarks1[ALIGN_POINTS],
                                   landmarks2[ALIGN_POINTS])
    mask = get_face_mask(im2, landmarks2)
    warped_mask = warp_im(mask, M, im1.shape)
    combined_mask = numpy.max([get_face_mask(im1, landmarks1), warped_mask],
                              axis=
```
```python
0
```
```python
)
    warped_im2 = warp_im(im2, M, im1.shape)
    warped_corrected_im2 = correct_colours(im1, warped_im2, landmarks1)
    output_im = im1 * (
```
```python
1.0
```
```python
- combined_mask) + warped_corrected_im2 * combined_mask
```
```python
return
```
```python
output_im
```
#### 主要步骤：
提取模板图、对齐图的landmarks;
M，通过transformation_from_points计算对齐图向模板图的转移矩阵M;
`matrix(``[[   0.62876962,    0.20978991, -101.32973923],
        [  -0.20978991,    0.62876962,   79.11235991],
        [   0.        ,    0.        ,    1.        ]]``)`mask,得到基于对齐图的掩膜，get_face_mask函数，获取 im2 的面部掩码，mask长成这样：
![这里写图片描述](https://matthewearl.github.io/assets/switching-eds/mask.png)
warped_mask ,warp_im函数，将 im2 的掩码进行变化，使之与 im1 相符,跟上面的mask张一样（一个鼻子）
combined_mask ，将二者的掩码进行连通（跟warped_mask 长一样）
warped_im2 ，warp_im函数，第二次，将第二幅图像调整到与第一幅图像相符（对齐图片,斜了点）
![这里写图片描述](https://img-blog.csdn.net/20171117184429306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
warped_corrected_im2 ，correct_colours函数，将 im2 的皮肤颜色进行修正，使其和 im1 的颜色尽量协调（类似下图）
![这里写图片描述](https://img-blog.csdn.net/20171117190821673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
output_im  组合图像，获得结果
实践：
FEATHER_AMOUNT =23Base_path ='03.jpg'cover_path ='02.jpg'output_im =Switch_face(Base_path,cover_path)
公众号“素质云笔记”定期更新博客内容：
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

