# 基于rPPG的人脸活体检测综述

#### 

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104432_36199.jpg)

## 什么是rPPG?

远程光电体积描记术 Remote Photoplethysmography (rPPG) 利用反射的周围光来测量皮肤的细微亮度变化。皮肤的细微亮度变化是由于心脏跳动导致的血液流动导致的。一般通过rPPG 我们可以得到类似 BVP 的信号，通过此信号可以预测心率，呼吸率等各种vital sign，在 healthcare 领域应用广泛。

## 为什么 rPPG 能用在人脸活体检测？

如下图所示，如果是 live face，会有部分周围光穿过皮层到达血管，然后反射回camera，故相机是能监测到血液流动导致的亮度变化；而对于 fake face，由于材料不同，吸收及反射到相机的信息就很不同。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104458_50148.jpg)

rPPG用于活体检测的原理

------

## 1. Generalized face anti-spoofing [1], ICPR2016

正如之前 活体检测综述 谈到，这是第一篇把 rPPG 用于活体检测的文章。如下图所示，由于3D打印制造的人脸面具具有与真人及其相似的纹理及深度，故用传统的局部纹理（LBP）等难以区分，但使用 rPPG 特征能更好地区分。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104515_36356.jpg)

3D人脸面具

## 算法流程如下：

1）对人脸ROI区域，提取RGB三个channel 对应的 rPPG信号，进行滤波转换到频域。

2）提取频域统计量（频域响应最大值，伪信噪比）共 6维特征。

3） 使用SVM进行分类。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104544_84485.jpg)

文中通过实验发现，rPPG对于面具攻击和纸张攻击效果很好，但是对于视频翻拍攻击效果一般，原因很可能是录制的视频人脸上也带有类似rPPG信息，故不好区分。最后文中提出了一个cascading system，即用rPPG来先拒掉纸张攻击和面具攻击，再用传统纹理特征来拒视频翻拍攻击：

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104602_79694.jpg)

## 算法缺点：

rPPG信号提取算法不够鲁棒；rPPG可判别性特征提取得过于简单。

------

## 2. PPGSecure[2]， FG2017

由于上文提取的rPPG特征过于粗糙，故本文提出了更discriminative的特征：
即对下图所示的5个ROI区域（3个人脸区域，2个背景区域）提取rPPG信号并转换到频域；接着把跟rPPG分布相关的 physiological range, [0.5 Hz, 5 Hz] 当成特征，并把五个区域对应的特征 concat 一起。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403104713_42038.jpg)

该特征设计的原理是：对于 live 样本，人脸上不同区域和背景区域，提取出来的rPPG特征分布差异很大；而对于 fake 样本，人脸上不同区域和背景区域，提取出来的rPPG特征分布差异很小。

### 算法缺点：

头部额头ROI (c) 很容易被头发遮挡；头部有motion或者在outdoor光照变化的场景下，容易false accepted。

------

## 3. Pulse-based Features [3], BTAS2018

启发于语音防伪中的特征设计speaker presentation attack detection，本文对rPPG信号进行 long-term spectral statistics (LTSS) 特征提取：
对 overlapping temporal windows 的信号进行N-point DCT 变换；然后提取DCT系数向量的一阶及二阶统计量 concat 一起；最后把不同窗口对应的统计量也合并一起。
$$
\mu (k)=\frac{1}{W}\sum_{i=1}^{W}\log\left | X_{i}(k) \right |\qquad \qquad \qquad(1)\\\sigma ^{2}(k)=\frac{1}{W}\sum_{i=1}^{W}(\log\left | X_{i}(k)-\mu (k) \right |)\qquad(2)
$$
特征设计的原理依据：live face 的rPPG信号在不同的时间窗口都会更具periodicity；而fake face 则不然。

实验结果比较可得：基于时间窗口的频域内容分析方法（LTSS），会比上述两文中的”频域统计量（最大值，伪信噪比）” 和 “[0.5 Hz, 5 Hz]频域的所有集合 ”要更精细合理，效果也更好。

------

## 4. Time Analysis rPPG [4], CVPRW 2018

该文致力于研究两个问题：1) 时间长短对rPPG活体检测性能的影响 2)在不同模态下，RGB or NIR 下的rPPG活体检测性能。

文中使用的算法pipeline及特征都和 [1] 中的大同小异，在这里主要贴几个重要的实验图及结果分析，以帮助后续更深入的算法设计：

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105023_49287.jpg)

面具攻击及高清纸张攻击下的性能

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105041_85263.jpg)

高清纸张攻击下的不同模态性能

结论：

1.对于rPPG信号，7秒左右的特征已很discriminative
2.对于高清纸张攻击，NIR提取的rPPG效果更佳（当然，对于屏幕翻拍，NIR更是牛逼）。

------

## 5. Local rPPG correlation model [5], ECCV2016

本文只致力于解决与真实人脸具有极其相似纹理及深度的3D面具攻击。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105202_15781.jpg)

算法框架图

我们主要来看看 Local rPPG correlation model 是如何设计特征：
1）先根据人脸landmarks构造出15个local ROI并提取对应的Local signal $[s\_{1}, s\_{2}, . . . , s\_{a}]^{T}$

2）对上述信号两两配对，计算它们的similarity (对信号i 和 信号j 进行cross-correlation增强 heartbeat component，接着傅里叶变换，最后求频域响应最大值):
$$
\rho (s_{i},s_{j})=max\left | F\left \{ s_{i}\star s_{j} \right \} \right |
$$
最后把生成的120种配对结果的similarity合并起来当成总特征。

算法原理依据：真实人脸中的不同局部区域间rPPG信号会有细微的延时（血液流动导致），但是局部区域内部基本一致；但 fake 人脸中的不同局部区域rPPG信号的频域相关性很低，因为其主要包含的是环境噪声而不是vital signals。

文中的另一个贡献是 Learning Confidence Map，设计了学习机制来学习不同区域local signal的重要性，使得某些更discriminative的区域贡献更大，类似attention机制。在这个模块中，confidence map 的依据是，rPPG signal的SNR高低程度。

### 算法缺点：

若整体人脸都带有global的motion或者illumination变换，所有的 local signal 也会受到影响，故在手持相机运动等情况下，效果不好。

------

## 6. rPPG Correspondence Feature, ECCV2018

本文是基于上文[5]的一个拓展版本，对motion及光照变化更鲁棒，计算量更低。
本文致力于探索 what is the real heartbeat component，希望能设计方法来整合 Local rPPG signal 的共性，来提取心跳。文中对心跳建模为template，故下面主要来说说如何学习得到这个template.

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105439_89628.jpg)

如上图上半部分所示，频谱模板的学习流程：

1）首先对人脸分成3块大的局部区域，然后把背景分出4块矩形的局部区域，对每块区域提取rPPG信号，然后对人脸和背景各自进行cross-correlation并进行傅里叶变换，得到人脸频谱 $s_{i}^{t}$ 和背景频谱 $s_{k}^{n}$

2）问题就变成了最小化下面的目标函数:
$$
min_{\omega }\sum_{i=1}^{M}\left \| S_{i}^{t}\omega -y \right \|_{2}^{2}+\lambda \left \| \omega  \right \|_{2}^{2}+\gamma \sum_{k=1}^{K}\left \| S_{k}^{n}\omega  \right \|_{2}^{2}
$$
这里 分别是人脸频谱和背景频谱的循环偏移矩阵， 目标 是带1D高斯分布的vector， 是需要学习的template系数。

在这里，是自监督学习的，不需要label。左边项约束使得人脸频谱尽量符合高斯分布，中间是正则项，右边约束使得背景噪声频谱尽量为0.

再来看上图的下半部分，一旦模板学习完成，就可以开始进行真正的特征提取过程了：

1）把人脸分成更小的9块局部区域，提取rPPG信号，cross-correlation，频谱

2）把得到的频谱与学习到的模板参数进行卷积，得到filtered response；最后对其进行peak-to-sidelobe ratio (来衡量 peak sharpness) 特征提取。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105702_22419.jpg)

实验结果来说，该方法在只有高仿面具攻击的数据集 intra-dataset 和 cross-dataset 测试，性能都比所有的rPPG方法，及普通 LBP及CNN(VGG）都好很多；在带纸张打印及视频翻拍攻击的数据集下，也超越了所有rPPG方法，但对比其他 color texture/deep learning based 方法还是有差距。

------

## 7.rPPG+Depth [7], CVPR2018

这也在之前的 活体检测综述 里说过，在这里，主要对比下 rPPG 的部分。
这是第一个用端到端的 deep learning 来学习 rPPG 频谱分布的文章。

![img](http://www.tensorinfinity.com/upload/mdfiles/20190403105614_70357.jpg)

框架图后半部分

如上图所示，训练流程：当提取好了每帧的deep feature后，便进行LSTM时序建模，后接FC假定输出的是rPPG信号，FFT提取频谱，然后与ground truth 计算L1距离作为Loss。这里整个过程模拟的特征提取，可以看成是文章[2]中的直接对比 [0.5Hz,5Hz]频谱启发的。

rPPG频谱的伪ground truth是这样生成的：

1）对于Live face：选择同一个identity的 no PIE variation video，用传统方法提取rPPG信号，然后计算频谱。

2）对于fake face：频谱全部=0
至于Non-rigid注册层的作用就是把人脸正脸化，即帧间的rPPG特征对齐好，服务于LSTM。

比较可惜的是文中没有单独使用 non-rigid registration + rPPG 的实验结果，都是加了Depth特征的，想知道它单独使用时的性能是否稳定。

------

## 总结与展望：

至于与rPPG活体检测相关联的 rPPG Living-Skin Classification[8]/Subject Detection[9] 有兴趣的也可以看看。总结一下，上述文献更多是focus在提取rPPG信号后，如何设计频域的feature 来更具判别性。后续发展方向，无疑是stronger的基于deep learning的方法，个人觉得空间还是很大，毕竟MSU文章[7]只是开了个头。

Reference:
[1] Xiaobai Li, , Guoying Zhao. Generalized face anti-spoofing by detecting pulse
from face videos, 2016 23rd ICPR
[2] Ewa Magdalena Nowara, PPGSecure: Biometric Presentation Attack Detection Using Photopletysmograms, FG2017
[3] Guillaume Heusch, Pulse-based Features for Face Presentation Attack Detection, BTAS2018
[4] Javier Hernandez-Ortega, Time Analysis of Pulse-based Face Anti-Spoofing in Visible and NIR, CVPRW2018
[5] Si-Qi Liu, 3D Mask Face Anti-spoofing with Remote Photoplethysmography, ECCV2016
[6] Si-Qi Liu, Remote Photoplethysmography Correspondence Feature for 3D Mask Face Presentation Attack Detection, ECCV2018
[7] Yaojie Liu, Amin Jourabloo, Xiaoming Liu, Learning Deep Models for Face Anti-Spoofing: Binary or Auxiliary Supervision ，CVPR2018
[8] Wenjin Wang, Living-Skin Classification via Remote-PPG, IEEE TRANSACTIONS ON BIOMEDICAL ENGINEERING, VOL. 64, NO. 12, DECEMBER 2017
[9]Wenjin Wang, Unsupervised Subject Detection via Remote PPG, IEEE TRANSACTIONS ON BIOMEDICAL ENGINEERING, VOL. 62, NO. 11, NOVEMBER 2015