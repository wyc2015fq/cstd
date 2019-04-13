
# 活体检测Face Anti-spoofing前世今生：作者（Fisher Yu ） - 小花生的博客 - CSDN博客


2018年09月06日 23:18:54[Peanut_范](https://me.csdn.net/u013841196)阅读数：2805


## 活体检测Face Anti-spoofing前世今生
摘录：[https://zhuanlan.zhihu.com/p/43480539](https://zhuanlan.zhihu.com/p/43480539)
**1. 什么是活体检测？**
–> 判断捕捉到的人脸是真实人脸，还是伪造的人脸攻击（如：彩色纸张打印人脸图，电子设备屏幕中的人脸数字图像 以及 面具 等）
**2. 为什么需要活体检测？**
–> 在金融支付，门禁等应用场景，活体检测一般是嵌套在人脸检测与人脸识别or验证中的模块，用来验证是否用户真实本人
**3. 活体检测对应的计算机视觉问题：**
–> 就是分类问题，可看成二分类（真 or 假）；也可看成多分类（真人，纸张攻击，屏幕攻击，面具攻击）
## Anti-spoofing 1.0
从早期 handcrafted 特征的传统方法说起，目标很明确，就是找到活体与非活体攻击的difference，然后根据这些差异来设计特征，最后送给分类器去决策。
那么问题来了，活体与非活体有哪些差异？
颜色纹理
非刚性运动变形
材料 （皮肤，纸质，镜面）
图像or视频质量
所以这段时期的文章都是很有针对性地设计特征，列举几篇比较重要的：
**[1] Image Distortion Analysis, 2015**
如下图，单帧输入的方法，设计了 镜面反射+图像质量失真+颜色 等统计量特征
![这里写图片描述](https://img-blog.csdn.net/20180906230242884?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[2] Colour Texture, 2016**
Oulu CMVS组的产物，算是传统方法中的战斗机，特别简洁实用，Matlab代码（课题组官网有），很适合搞成C++部署到门禁系统。
原理：活体与非活体，在RGB空间里比较难区分，但在其他颜色空间里的纹理有明显差异
算法：HSV空间人脸多级LBP特征 + YCbCr空间人脸LPQ特征 （后在17年的paper拓展成SURF特征）
![这里写图片描述](https://img-blog.csdn.net/20180906230316383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[3] Motion mag.-HOOF + LBP-TOP, 2014**
![这里写图片描述](https://img-blog.csdn.net/20180906230350841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[4] DMD + LBP, 2015**
前面说的都是单帧方法，这两篇文章输入的是连续多帧人脸图；
主要通过捕获活体与非活体微动作之间的差异来设计特征。
一个是通过 方向光流直方图HOOF + LBP-TOP；一个是通过动态模式分解DMD，得到最大运动能量的子空间图，再分析纹理。
PS：这个 motion magnification 的预处理很差劲，加入了很多其他频段噪声（18年新出了一篇用 Deep learning 来搞 Motion mag. 看起来效果挺好，可以 google 一下）
![这里写图片描述](https://img-blog.csdn.net/20180906230424921?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[5] Pulse + texture, 2016**
第一个将 remote pluse 应用到活体检测中，多帧输入
（交代下背景：在CVPR2014，当时 Xiaobai Li 已经提出了从人脸视频里测量心率的方法）
算法流程：
1. 通过 pluse 在频域上分布不同先区分 活体 or 照片攻击 （因为照片中的人脸提取的心率分布不同）
2. 若判别1结果是活体，再 cascade 一个 纹理LBP 分类器，来区分 活体 or 屏幕攻击（因为屏幕视频中人脸心率分布与活体相近）
![这里写图片描述](https://img-blog.csdn.net/20180906230455506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Anti-spoofing 2.0
其实用 Deep learning 来做活体检测，从15年陆陆续续就有人在研究，但由于公开数据集样本太少，一直性能也超越不了传统方法：
**[6] CNN-LSTM, 2015**
多帧方法，想通过 CNN-LSTM 来模拟传统方法 LBP-TOP，性能堪忧~
**[7] PatchNet pretrain，CNN finetune, 2017**
单帧方法，通过人脸分块，pre-train 网络；然后再在 global 整个人脸图 fine-tune，然并卵
**[8] Patch and Depth-Based CNNs, 2017**
第一个考虑把 人脸深度图 作为活体与非活体的差异特征，因为像屏幕中的人脸一般是平的，而纸张中的人脸就算扭曲，和真人人脸的立体分布也有差异；
就算用了很多 tricks 去 fusion，性能还是超越不了传统方法。。。
![这里写图片描述](https://img-blog.csdn.net/20180906230529633?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[9] Deep Pulse and Depth, 2018**
发表在 CVPR2018 的文章，终于超越了传统方法性能。
文章[8]的同一组人，设计了深度框架 准端到端 地去预测 Pulse统计量 及 Depth map （这里说的“准”，就是最后没接分类器，直接通过样本 feature 的相似距离，阈值决策）
在文章中明确指明：
过去方法把活体检测看成二分类问题，直接让DNN去学习，这样学出来的cues不够general 和 discriminative
将二分类问题换成带目标性地特征监督问题，即 回归出 pulse 统计量 + 回归出 Depth map，保证网络学习的就是这两种特征（哈哈，不排除假设学到了 color texture 在里面，黑箱网络这么聪明）
![这里写图片描述](https://img-blog.csdn.net/20180906230600102?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180906230600102?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
回归 Depth map，跟文章[8]中一致，就是通过 Landmark 然后 3DMMfitting 得到 人脸3D shape，然后再阈值化去背景，得到 depth map 的 groundtruth，最后和网络预测的 estimated depth map 有 L2 loss。
而文章亮点在于设计了 Non-rigid Registration Layer 来对齐各帧人脸的非刚性运动（如姿态，表情等），然后通过RNN更好地学到 temporal pulse 信息。
![这里写图片描述](https://img-blog.csdn.net/20180906230631189?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**[10] Micro-texture + SSD or binocular depth , 2018**
ArXiv 刚挂出来的文章，最大的贡献是把 活体检测 直接放到 人脸检测（SSD，MTCNN等） 模块里作为一个类，即人脸检测出来的 bbox 里有 背景，真人人脸，假人脸 三类的置信度，这样可以在早期就过滤掉一部分非活体。
所以整个系统速度非常地快，很适合工业界部署~
至于后续手工设计的 SPMT feature 和 TFBD feature，分表是表征 micro-texture 和 stereo structure of face，有兴趣的同学可以去细看。
![这里写图片描述](https://img-blog.csdn.net/20180906230706326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
至此，Face anti-spoofing 的简单Survey已完毕~
毫无疑问，对于学术界，后续方向应该是用DL学习更精细的 人脸3D特征 和 人脸微变化 表征；而也可探索活体检测与人脸检测及人脸识别之间更紧密的关系。
对于工业界，最后那篇文章的启发很不错；更可借助近红外，结构光/ToF等硬件做到更精准。
## Reference:
[1] Di Wen, Hu Han, Anil K. Jain. Face Spoof Detection with Image Distortion Analysis. IEEE Transactions on Information Forensics and Security, 2015
[2] Zinelabidine Boulkenafet, Jukka Komulainen, Abdenour Hadid. Face Spoofing Detection Using Colour Texture Analysis. IEEE TRANSACTIONS ON INFORMATION FORENSICS AND SECURITY, 2016
[3] Samarth Bharadwaj. Face Anti-spoofing via Motion Magnification and
Multifeature Videolet Aggregation, 2014
[4] Santosh Tirunagari, Norman Poh. Detection of Face Spoofing Using Visual Dynamics. IEEE TRANS. ON INFORMATION FORENSICS AND SECURIT, 2015
[5] Xiaobai Li, , Guoying Zhao. Generalized face anti-spoofing by detecting pulse
from face videos, 2016 23rd ICPR
[6] Zhenqi Xu. Learning Temporal Features Using LSTM-CNN Architecture for Face Anti-spoofing, 2015 3rd IAPR
[7] Gustavo Botelho de Souza, On the Learning of Deep Local Features for
Robust Face Spoofing Detection, 2017
[8] Yousef Atoum, Xiaoming Liu. Face Anti-Spoofing Using Patch and Depth-Based CNNs, 2017
[9] Yaojie Liu, Amin Jourabloo, Xiaoming Liu, Learning Deep Models for Face Anti-Spoofing: Binary or Auxiliary Supervision ，CVPR2018
[10] Discriminative Representation Combinations for Accurate Face Spoofing Detection ”，2018 PR，上海交大

