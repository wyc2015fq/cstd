# 【Python】skimage模块 - YZXnuaa的博客 - CSDN博客
2018年03月19日 15:38:09[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：775

可以用来做图像做处理的模块有很多，不过对于使用python不熟悉，或者刚使用python做图像处理的时候，经常不知道应该选择使用哪些模块。比如，scipy中misc和ndimage中都有相应的图像处理函数，现在就介绍一下scikit-image模块吧：
1)**例图**： 
fromskimageimportdatadata里面会有样图，如data.camera()是一张（512， 512）的图；
2）**滤波**：
fromskimageimportfilters比如高斯滤波，filters.gaussian(camera, 1)
比如边缘检测：filters.sobel(camera)
3）**文件读取**：
fromskimageimportio
io.imread(filename)，其中，filename也可以是URL地址；
io.imsave(path, img).
4)**数据类型转换**：
fromskimageimportimg_as_float
camera_f = img_as_float(camera)将原图uint8类型转变成[-1, 1]之间的范围；

5）**颜色空间转换**：
fromskimageimport color
color.rgb2hsvcolor.lab2rgb, etc.
**关于图像预处理和增强**
6）**局部滤波**，如filters.sobel_h(img)
7）**非局部滤波**，如
romskimageimportexposure
中exposure.equalize_hist(img)做直方图均衡；

8）**数学形态变换**
fromskimageimportmorphology**具体图形生成**：比如morphology.diamond(1)生成菱形；
**腐蚀操作**：morphology.binary_erosion(img, selem=None)
selem表示结构元素，用于设定局部区域的形状和大小。将0值扩充到邻近像素。扩大黑色部分，减小白色部分。可用来提取骨干信息，去掉毛刺，去掉孤立的像素。
**膨胀操作**：morphology.binary)dilation(img, selem = None)
一般对二值图像进行操作。找到像素值为1的点，将它的邻近像素点都设置成这个值。1值表示白，0值表示黑，因此膨胀操作可以扩大白色值范围，压缩黑色值范围。一般用来扩充边缘或填充小的孔洞。
**Opening**: erosion + dilation在
注：该操作也适用于非二值图，即灰度图，对应的是最小滤波
（scipy.ndimage.morphology同样也有该操作）

9）**图像分割**
**基于直方图分析的二值分割**：（Otsu阈值法）如：val = filters.threshold(img)找到阈值，然后通过mask = img < val得到二值图
**图像打标签**：对于分割后的图像，可以给每一个区域打标签，如
fromskimageimportmeasureall_labels=measure.label(blobs)
另外，scipy中也有相应操作；
**水域分割算法（也叫分水岭算法）**： 
fromskimage.morphologyimportwatershedfromskimage.featureimportpeak_local_maxfromscipyimportndimage>>> distance=ndimage.distance_transform_edt(image)>>> local_maxi=peak_local_max(distance,indices=False,footprint=np.ones((,)),labels=image)>>> markers=morphology.label(local_maxi)>>> labels_ws=watershed(-distance,markers,mask=image)另一个**随机walker分割**：fromskimageimportsegmentation>>> # Transform markers image so that 0-valued pixels are to>>> # be labelled, and -1-valued pixels represent background>>> markers[~image]=->>> labels_rw=segmentation.random_walker(image,markers)
10）**测量区域属性，如面积和周长**
properties=measure.regionprops(labels_rw)>>> [prop.areaforpropinproperties][770, 1168]>>> [prop.perimeterforpropinproperties][100.91..., 126.81...]
11）**数据可视化和交互**
matplotlib中提供了plt.imshow等函数，结合以上滤波、分割(segmentation.clear_border(mask))等使用
另外，skimage也提供显示图像的函数，不过也是基于matplotlib，貌似功能更强大：
fromskimageimportviewer>>> new_viewer=viewer.ImageViewer(coins)>>> new_viewer.show()
For more interaction, plugins can be added to the viewer:
>>>>>> new_viewer=viewer.ImageViewer(coins)>>> fromskimage.viewer.pluginsimportlineprofile>>> new_viewer+=lineprofile.LineProfile()>>> new_viewer.show()
12）**CV中的特征提取**
fromskimageimportfeature
不同角点特征：
fromskimage.featureimportcorner_harris,corner_subpix,corner_peaks
//补充角点特征知识点
另外skimage.transform提供扭曲和仿射变换操作wrap和AffineTransform
**//待补充其它特征提取函数**
