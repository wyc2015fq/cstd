# sensor的skipping and binning 模式 - maopig的专栏 - CSDN博客
2018年09月07日 14:19:38[maopig](https://me.csdn.net/maopig)阅读数：428

1、第一种理解
关于sensor的缩放存在两种模式 binning mode 和skip mode
skipping mode 就是把想要的数据采集上来 把其余的数据扔掉 如下所示 column skip 2 row skip 2 留下的就是白色的像素 黑色的像素丢掉。
![](https://img-my.csdn.net/uploads/201211/30/1354246141_2499.jpg)
binning mode 就是把相邻的像素合成一个像素，然后再输出 如下所示2 bin to 1 所有的像素都参与了新像素的生成。
![](https://img-my.csdn.net/uploads/201211/30/1354246172_4102.jpg)
2、第二种理解
camera Binning Mode：像素合并模式，将相邻的像素单元电荷通过物理的方法叠加在一起作为一个像素输出信号；
Binning是一种图像读出模式，将相邻的像元中感应的电荷被加在一起，以一个像素的模式读出。Binning分为水平方向Binning和垂直方向Binning，水平方向Binning是将相邻的行的电荷加在一起读出，而垂直方向Binning是将相邻的列的电荷加在一起读出，Binning这一技术的优点是能将几个像素联合起来作为一个像素使用，提高灵敏度，输出速度，降低分辨率，当行和列同时采用Binning时，图像的纵横比并不改变，当采用2:2Binning，图像的解析度将减少75%。
Binning Mode 优势：增加感光面积，提高暗处对光感应的灵敏度；
Binning Mode 劣势：降低输出分辨率；
应用：增加物理感光像素单元，采用像素合并模式，提升暗处对光感应的灵敏度；
![](https://img-blog.csdn.net/20150624154440639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVvX3dkbHM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 微光成像指南
1. 请参照快速使用指南或用户手册，连接相机，使相机处于正常成像模式。
2. 当拍摄环境光线为微光级别时，推荐增大曝光时间和Binning（像素合并功能）。
注：使用binning功能时会降低图像分辨率，但实际微光应用中，Binning功能即使降低了图像分辨率，但拍摄目标物相对较远时，Binning反而会取得更高的成像质量。通过下面两幅图对比发现，微光下Binning成像质量更好。
![](http://www.indigo-imaging.com/Uploads/image/20170210/20170210182956_10807.jpg)
图1.全分辨率模式
![](http://www.indigo-imaging.com/Uploads/image/20170210/20170210183108_99848.jpg)
图2. 2*2bin模式
           ①全分辨率成像模式。
           ②微光下推荐选择高增益。
     ③Binning 2x2 ：像素合并模式。
     ④微光下推荐适用自动色阶功能，也可以手动调节色阶上下限取值，尝试获得更优的成像效果。
     ⑤微光下可手动调节伽马、对比度等提高图像显示效果; 尤其是将对比度的值调高，在弱光下可显著提高成像效果。
3. 微光下推荐曝光时间越大越好，但特殊应用场景大曝光时间会降低帧频及可能会对运动物体产生拖尾现象，因此，实际应用中需综合考虑设置合适的曝光时间。
4. 微光下可选择大光圈增加进光量，弥补曝光时间不能过大的缺点（0.001lux光照强度推荐光圈F1.2或者更大级别光圈）。为使像面有足够的照度，物镜应有足够大的相对孔径。
5. 像元尺寸6.5μm的相机微光下已评测到0.001lux级别的光照强度（物距20m/镜头焦距84mm/光圈数F1.2/曝光时间20ms/50fps），如需要更低级别如0.0001lux, 推荐另一款背照式11μm像元尺寸的相机。
