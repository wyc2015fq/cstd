
# OCR身份证号码识别 - intflojx的博客 - CSDN博客


2019年01月01日 14:51:03[paulpanwang](https://me.csdn.net/intflojx)阅读数：274



# 一、项目概述
如今互联网的迅猛发展让网络实名制再次引起社会的重视，因此进一步加强对网络秩序的安全管理，对推进社会治安综合治理工作显得尤为突出和重要。这时候*OCR*身份证识别技术的优势就逐渐显现出来了。
互联网的开放性、交互性、匿名性和信息传播的快速性，使普通民众拥有了大量免费资源和更多话语权的同时，也衍生出了许多暴力犯罪问题。为了维护社会的和谐稳定，实行网络实名认证从源头上进行约束和监管也是一项重大的工作。
身份证识别技术不仅可以实现拍照输入身份证信息，还能对身份证真伪进行鉴别。如此就能有效防范不法分子盗用他人信息进行违法犯罪。此外，它还具备较高的灵活性和可塑性，支持多平台多终端，因此可以嵌入不同平台的软件系统中运行。图像已经很广泛的应用到公安领域的警务通，其中就运用到了云*/*脉*OCR*身份证识别技术。同样的身份证识别技术也为扫除手机实名制盲区立下汗马功劳。
如今移动互联网大潮来临，消费市场逐渐向移动端转移，摄像头已经成为数据采集最主要的入口，更多的非结构化数据需要转化为前后台可检索的数据，这个转化的过程就需要*OCR*技术。
身份证识别技术的出现，是社会各行业信息化自我革新的机会，也是实
现网络实名制改革升级的途径之一。
# 二、运行环境
软件：*Win7 + MatlabR2012b + Tesseract3.04*硬件：*Inter-i3-2350+6GB*内存

# 三、测试效果
## 1.
## 测试流程
### 1)
### 图像光照处理
首先对图片进行灰度化处理，然后进行光照处理，因为有些图片的亮度不够均匀，在转二值化的过程中会产生误差。如果图像某部分光线太强，文字在转二值图像的过程中就会变成空白；如果某部分光线太弱，文字在转二值图像的过程中就会变成黑色背景。为了消除这种误差，就要先对身份证的图像进行光照处理。效果图如下：
|原始图像
![](https://img-blog.csdnimg.cn/20190101144333914.png)
|
|背景估计
![](https://img-blog.csdnimg.cn/20190101144352170.png)
|
|删除背景
![](https://img-blog.csdnimg.cn/20190101144358185.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
|

### 2)
### 图像校正
由于输入的身份证图像可能会有倾斜，如果倾斜程度过大，会影响匹配识
别。所以在识别文字之前，要先利用*hough*变换对图像进行校正。校正的原理就是先对图像的边框进行边缘检测，根据检测到的直线角度来校正整张身份证
![](https://img-blog.csdnimg.cn/20190101144526718.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)

### 3)
### 模板匹配
对于校正后的身份证图片，想要识别上面的信息，就要先确定信息所在的
范围。确定范围的过程就是模板匹配的过程，将图片和已经存在的模板进行匹配，就能提取出信息用于下一步的二值化。效果图如下：
![](https://img-blog.csdnimg.cn/20190101144542608.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190101144602685.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190101144615281.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
ROI_Image1=roicolor(ROI_Image1(:,:,1),Y_MIN,Y_MAX)&roicol or(ROI_Image1(:,:,2),Cb_MIN,Cb_MAX)&roicolor(ROI_Image1(:,:,3)
**2.****测试结果**
**1)**数字测试结果
数字识别对输入的身份证图片进行光照处理，图片校正和模板匹配后，就可以进行数字识别。利用神经网络学习一定数量的训练样本*,*一共需要训练*0*，*1,2,3,4,5,6,7,8*，*9*，*X*共*11*个对象，每个对象训练*8-9*个样本，得到
最后训练过的网络。用训练过的神经网络对图片上的数字进行匹配识别。识别效果如下：
![](https://img-blog.csdnimg.cn/20190101144634907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
### 2)
### 文字识别
*1.*在数字识别的基础上，我们还添加了一些文字识别的功能，利用
![](https://img-blog.csdnimg.cn/20190101144729105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190101144736629.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190101144748220.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)

# 四、算法说明
## 1.
## 思路说明及分析
基本处理流程如下：
![](https://img-blog.csdnimg.cn/20190101144759829.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
## 2.
## 算法原理
### 1)
### 灰度化
用*f(i,j)=0.30R(i,j)+0.59G(i,j)+0.11B(i,j))*公式对图片进行灰度化处理，
得到的图像方便后面进行光照矫正；
### 2)
### 光照矫正
在灰度化处理后的图片的基础上使用MATLAB的blkproc函数提取图像的背景，再用imsubtract函数对图像进行减法运算。得到除去光照背景的图像。
[Xsize,Ysize]=size(img); thr=3;
blocks=blkproc(img,[round(Xsize/2),round(Ysize/2)],@esti
background,thr); background=imresize(blocks,[Xsize,Ysize] );
%获得背景图像
figure;imshow(background,[]);title('提取背景' );
%通过图像减运算进行光照不均匀校正
img=imsubtract(img,background); img=mat2gray(img);
functionbackgray=estibackground(x,thr) meanx=mean(x(:)); stdx=std(double(x(:))); minx=min(x(:)); minx2=meanx- thr*stdx; backgray=max(minx2,minx);end

### 3)
### 二值化
图像的二值化，就是将图像上的像素点的灰度值设置为 0 或 255，也就是将整个图像呈现出明显的只有黑和白的视觉效果。对光照矫正之后的图像进行二值化处理，得到黑白的图像，此步骤主要是为了方便后面进行hough变换检测直线。
%快速转变成二值图像
i1=rgb2gray(i);
i2=im2bw(i1);
%感兴趣阈值进行二值化
,Cr_MIN,Cr_MAX);

### 4)
### Hough变换
霍夫变换是图像变换中的经典手段之一，主要用来从图像中分离出具有
某种相同特征的几何形状（如，直线，圆等）。霍夫变换寻找直线与圆的方法相比与其它方法可以更好的减少噪声干扰。 我们这里运用的霍夫变换来检测直线，首先检测出图像的边缘性，得到
一副边缘图像；再对图像中的每一个边缘点，在k-b空间中画出一条直线；然后在各直线的焦点，我们采取vote的方法，即累加n条直线的交点，该点的值为n1；最后遍历k-b空间，寻出先局部最大值（极值）的点，这些点的坐标（k，b）就是图像中可能出现的直线的斜率和截距。
![](https://img-blog.csdnimg.cn/20190101144824157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
如图中的极坐标系中，小框标出来的就是交点，说明该坐标系下的ρθ
处检测出了一条直线。
### 5)
### 倾斜校正
此模块主要分为以下几个步骤:
找到最长直线à得到倾斜度à用imrotate函数矫正图片角度
通过这个霍夫变换检测出图像中最长的直线后，可以根据这个直线的倾斜程度（斜率）来对整张图片进行图像旋转校正。一般来说，身份证图片上最长的一条线是它的长，将这条线根据斜率校正可以将整张身份证都调整成到和模板一样正，方便下一步的模板切割和识别。图像
旋转可以用matlab自带的imrotate函数实现。
### 6)
### 模板切割
此模块主要分为以下几个步骤:
人眼识别à人脸识别à设计模板à模板匹配à切割
模板切割是在对身份证倾斜调整的基础的上进行的。首先利用 MATLAB自带的人眼识别函数，识别出图像中人的眼睛。然后根据眼睛的位置确定出整张身份证的位置。在此基础上制作一份身份证的模板，将需要的信息的模板切割出来。然后根据这张切割过的模板对我们的需要检测的身份证进行对比，将这张身份证中需要的信息切割出来。最终设计出来的身份证模板如下：
![](https://img-blog.csdnimg.cn/2019010114484335.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
str = [PathName,FileName]; Org_Image=imread(str); videoFrame=Image_correct( Org_Image );
%%人眼检测
videoFrame2=imresize(videoFrame,0.25); BBOX = step(Face_Detect, videoFrame2); Face_Image=imcrop(videoFrame,4*BBOX); bbox = step(Eye_Detect, Face_Image); bbox(1)=4*BBOX(1)+bbox(1); bbox(2)=4*BBOX(2)+bbox(2);
%原图像与模板的大小比例
rate=(bbox(4)/(117-99)+bbox(3)/(340-279))/2;
BW_Templet=imread('templates.bmp');
%对模板进行剪切
BW_Templet=imresize(BW_Templet,rate);                                 stats = regionprops(BW_Templet, 'basic');
[m,n]=size(BW_Templet); rect=round([bbox(1)-279*rate,bbox(2)-99*rate,n,m]);%对原图像进行裁剪
RGB_Templet=imcrop(videoFrame,rect);
### 7)
### BP神经网络识别（数字）
用匹配之后得到的数字图片进行神经网络识别。
![](https://img-blog.csdnimg.cn/20190101144858743.png)
神经网络如图所示，输入层与输出层之间有一个隐藏层。我们通
过给一些样本（一个对象大约给8-9个样本），然后经过输出值与真实值的比较来反馈给图中的线，适当改变线的权值从而得到较为准确的库。
训练过程结束之后，图中各条线的权值已经确定。只需要输入切割好的一个个数字图片，就可以得到最近似于这张图片内容的数字。

### 8)
### tesseract库识别（汉字）
网络上下载*tesseract*库，将匹配所得的汉字图片切割成只含一个
汉字的图片，然后与*tesseract*库中的文字进行比对。得到最终结果，输出。
imwrite(ROI_Image1,'GUI\name.tif');
system('tesseract\tesseract GUI\name.tif GUI\name -l chi_sim');
fid=fopen('GUI\name.txt','r+','n','utf-8');     nametxt=fgetl(fid);

## 3.
## 时间、空间复杂度分析
由于项目中多数使用的是MATLAB库函数，故空间复杂度并不容易
获得。时间的数量方面可以通过MATLAB自带的计时功能得出： 原图像à倾斜矫正后的图像：
![](https://img-blog.csdnimg.cn/20190101144915641.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)
倾斜矫正后的图像à模板匹配切割后的图像：
![](https://img-blog.csdnimg.cn/20190101144933484.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4,size_16,color_FFFFFF,t_70)

