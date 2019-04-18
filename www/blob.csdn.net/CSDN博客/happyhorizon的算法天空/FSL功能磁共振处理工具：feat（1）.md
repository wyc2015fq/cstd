# FSL功能磁共振处理工具：feat（1） - happyhorizon的算法天空 - CSDN博客
2018年12月19日 09:36:22[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：229
# 基本介绍
## 功能
FSL的FEAT工具主要用在功能磁共振影像fmri的分析中。FEAT提供了fmri 的预处理和对实验设计简单的统计分析功能，同时也具有足够的灵活性以允许对最复杂的实验进行复杂分析。
## 调用方式
和FSL的诸多命令一样, feat提供了GUI界面和命令行调用两种方式. 启动feat的GUI界面, 可以在shell控制台中输入fsl, 点击feat按钮, 弹出界面如下图所示:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218184129794.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
GUI界面的最上面有两个按钮, 是基本任务选择:  first-level analysis 和 high level analysis , 在first level analysis 中, 还有Full analysis, preprocessing 和 statistics 三种选择. 对于high level analysis, 就只有statistics这一种.
对于命令行调用方式, 只需要配置好需要的fsf文件即可. fsf文件中记录了所有的设置选择. 在GUI界面上设置好后, 可以点击最下面的 Save 按钮进行保存设置到.fsf文件中. 英国biobank在批处理pipeline中就采用了设置fsf配置文件的方法。
### FMRI的预处理过程：
- DICOM to NifTI : 压缩成NifTI格式
- remove first images: 去掉开始的几张
- slice timing：扫描时间修正
- realign：头动修正
- temporally detrend： 去时间漂移
### 1) DICOM to NifTI（压缩NifTI）:
首先选择输入的NifTI文件. 在Data标签页中,设置输入Data和输出路径:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218230942912.jpeg)
如果输出路径留空的话, 默认会在输入数据路径下新建一个以"数据名称.feat"命名的文件夹作为输出路径。
每运行一次，默认的结果文件夹名字后面会自动添加一个“+”号。例如第二次运行后的结果会保存在名为“数据名称.feat+”的文件夹中。
### 2) remove first images（去掉开始的几张）：
当设置好输入文件后，feat会自动识别总的Volume数量，如果需要刚开始的几个volume，可以在这里设置：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218231234889.jpeg)
### 3) slice timing： 扫描时间误差修正
slice timing， 扫描时间误差修正。需要做这一步的原因是在MRI机器扫描的时候，这一个slice和下一个slice扫描的时间点是不同的（显然，即便是同一个slice， 扫描的数据也不可能在同一时刻），实际上得到的MRI图像是用了“临近”的时间点上的状态去“近似”得到“当前”时间的大脑状态。
在MRI采集中，这个时间段就称为TR， 也就是说为了采集到整个大脑的状态数据，扫描脉冲会在TR时刻内扫描到>=30个slice。过去扫描序列是从上到下或者从下到上的顺序序列，目前通常是间隔序列，例如先奇数slice，之后偶数slice。
假设slice9比slice10采集的时间晚了1.5s，如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206145528762.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
slice timing所要做的修正就是对slice9的数据进行偏移和重采样（shift+interpolate）。修正后结果如下:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206145904329.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
fsl – feat中, 这个设置分在两个标签页上: Data标签页上, 可以设置TR, 如下图:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218233636515.jpeg)
在Pre_stats标签页上, 在slice timing的regressing correction右侧的下拉菜单中选择具体的设置.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218233649797.jpeg)
### 4) realign 头动修正/线性修正:
realign指的是头动线性修正。fsl-feat用了MCFLIRT做头动的刚性矫正. MCFLIRT工具基于FLIRT命令实现,  MCFLIRT工具没有任何的设置, 官方解释是因为这种是比较初级的修正方法, 还需要更多的研究.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218233810541.jpeg)
Alternative reference image 可以设置刚性修正的参考图像.
### 5) Spatially Smooth 空间平滑
为什么要做空间平滑/空间滤波？
当模糊的区域比激活的区域大的时候，空间滤波可以提高信号的信噪比，同时数据具备最小光滑度以满足后续高斯随机场理论设置阈值。不过，空间滤波操作也将平滑掉一些非常小的激活区域。
空间滤波算法在具体实施的过程中，会利用一个3D的卷积核，对每个voxel逐一做卷积，对于特定的voxel，将其周围voxel强度的加权平均作为该voxel的强度。
加权系数的选择如图，FWHM=全宽半高高斯核，中心voxel的系数为1，周围voxel 上的系数随距离减小。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206155642546.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
FWHM平滑核宽度不同，结果也会不同。如果需要看比较精细的结构, 可以选择平滑核宽度为5mm, 如果只是在看比较粗的\大致的范围, 可以设置为10mm甚至是15mm. 如果要关闭平滑, 可以将FWHM为0.  如下图第一排最左侧图，FWHM的宽度从0逐渐增加。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206160041276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
fsl-feat中的相应设置在pre-stats里:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181218234757878.jpeg)
intensity归一化, 使得fmri每个volume有一样的平均强度. 通常用不到, 默认也是取消的状态.
Fsl – feat 还有一个perfusion subtraction操作, 适用于专门的perfusion fmri数据处理, 不同于通常的BOLD fmri。
## 时间漂移滤波
每个voxel强度的时间序列都包含着与扫描设备相关的、医学上的，以及一些高频噪声信号。与扫描设备、生理相关的其它信号（例如血液循环信号，呼吸信号等等）都有高频和低频的成分。任务激活信号就被淹没在这些噪声信号中。
那么什么是时间漂移滤波？时间漂移滤波（Temporal Filtering）就是指将这些高频、低频噪声去掉的过程。经过时间漂移滤波后，可以将信号有效地提取出来，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206172709162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
假设原始信号如左图，经过高通滤波器后，低频（线性漂移）被去掉了，只保留了高频信号，如右图。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206161329718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
如果用低通滤波，得到信号如下面右图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190206173032896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
因此，在选择滤波器的时候，一定要充分考虑到滤波器的特点。大多数情况下，特别是Dense Single Event 模型，模型本身会引入很多高频信号。这时必须要谨慎选择低通滤波器，尽可能保证有用的信号不丢失。
![Dense Single-Event Model](https://img-blog.csdnimg.cn/20190206173501728.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
## 参考
T1norm与EPInorm配准方法对fMRI数据分析的影响
[http://blog.sina.com.cn/s/blog_18712c10d0102x9zi.html](http://blog.sina.com.cn/s/blog_18712c10d0102x9zi.html)
**EPI 扫描序列和EPI图像**, DTI和fmri都是EPI序列采集的, 所以这里说的EPI模板也就是用这种这种序列扫描得到的一个平均模板. T1图像一般是用3D MPRAG序列扫描的. 所以用EPI序列扫描得到的图像就经常被称为EPI图像.
![EPI图像](https://img-blog.csdnimg.cn/20181218233959652.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hhcHB5aG9yaXppb24=,size_16,color_FFFFFF,t_70)
