# 时序分析：HMM模型（状态空间） - wishchinYang的专栏 - CSDN博客
2015年12月18日 16:11:57[wishchin](https://me.csdn.net/wishchin)阅读数：5367
        关于HMM模型：[时序分析：隐马尔科夫模型](http://blog.csdn.net/wishchin/article/details/13159943)
        HMM用于手势识别： 训练时每一种手势对应一个HMM-Model，识别率取最大的一个HMM即可。  类似于一个封装的完成多类识别器功能单层网络。
        优点： 尤其适用于时间序列的建模，对复杂度高的动作也具有较高的识别精度，易于添加和修改手势库。
        缺点：训练的难度大（需要多的样本来完成图的优化）。
        以下引自于一篇论文：基于马尔可夫模型的手势识别算法。修改为了与我自己的项目相关的东西。
**引言：**
       基于隐马尔可夫模型的动态手势识别一般是应用手势的时间特性进行识别。单一的手势可被认为是不同手形序列组成的，多个手势可由手形及其运动轨迹加以区分。可同时
应用手形及其运动轨迹两大特征进行手势识别（把静态手型作为特征向量的一个维度），在轮廓跟踪过程中，获得当前的手部轮廓，获取多自由度（六自由度（欧式位置x,y,x和欧式位姿alpha，beta，theta） + 一个静态手势码）的手势的运动轨迹，这样就可从图像序列中得到动态手势的特征向量，即观察值序列，记作Ol , l = 1,..., L，其中L为序列总数。
**HMM的特征向量：**
      获取手势的观察值序列：
      手势轨迹的基本特征是六自由度（位置、方位角）。由于同一个用户，手势速率的变化也是比较大的，因此，本文采用六自由度（位置和角度）相结合的方法获取运动轨迹。通过基于卡尔曼滤波器[2]与自适应形状模型的轮廓跟踪算法[3]，我们可得滤波器最优估计值：
          X*k* =  (X*c*^t, Y*c*^t, Z*c*^t，X*c* ^t - X*c* ^(t-1), Y*c*^t-Y*c*^(t-1) ，Z*c*^t- Z*c*^(t-1)   ) ^T          （2）
 其中(Xc^t, Yc^t, Zc^t )为t 时刻的位置坐标，( X*c* ^(t-1), Y*c*^(t-1) ，Z*c*^(t-1)   )为t −1时刻的轮廓质点，由此可得手势轨迹的角度，如图2（a）所示。
      根据所得ϕ^t ，我们采用12 方向链码进行离散化，如图2（b）所示[4]。
![](https://img-blog.csdn.net/20151222154354169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)(图错了，懒得修改！)
        在手势跟踪过程中，跟踪器根据上述方法按**固定时间间隔**（有些问题）从图像序列中提取手势的特征向量，可得到手势的特征观察值序列Ol , l = 1,..., L。
**HMM模型训练：**
        在获取手势的特征观察值序列后，则可对HMM 模型进行训练。首先要对HMM模型λ = (A, B,π )初始化，由于我们采用左右结构的离散HMM 模型，因此我们设定状态i 只能返回到本身或转移到i +1状态，状态转移矩阵A可初始化为[4]：
![](https://img-blog.csdn.net/20151222120511309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对于初始概率分布，设定：
                           π1 =1，
                           πi =0（ i ≠ 1）；
即假设HMM 是从第一个状态开始的。对于观察概率矩阵B， 设
![](https://img-blog.csdn.net/20151222122311126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       初始模型确定以后，利用**前后向递推法**计算观察值序列O在这个模型下的P(O/λ )；      然后利用 Baum-Welch 算法对初始 HMM 模型λ = (A, B,π )进行重新计算，HMM 的各个参数在这一过程得到重新估计，即求得新的HMM 模型λˆ = (Aˆ, Bˆ,πˆ)；       再利用 Viterbi 算法计算出观察序列O在这个模型下的P(O/λˆ)。
       为了估计出最接近观察值序列O的HMM模型，我们设定阈值ε ，当 P(O /λˆ) − P(O /λ ) <ε 时即得到训练出的HMM模型；否则令 λ =λˆ。重复此步骤，直到P(O/λ )收敛，得到最接近观察值的HMM 模型。
**基于HMM的识别过程:**
         基于HMM 模型进行手势识别研究，大多采用计算某测试样本与参考模型集的似然度，通过判断最大值以确定测试样本的类别[5]。基于这一原理，采用HMM 实现动态手势识别，HMM的输入为跟踪器提取的特征观察值序列*O*l , l = 1,..., L。图像序列中手势识别过程可归纳为（流程图如图3
 所示）：
     （1）检测手部位置初始化跟踪器，跟踪算法如文献[2]所述。
     （2）在执行手势的图像序列中提取手部轮廓并按固定时间间隔获得手形与手势运动轨迹两类特征向量。
     （3）由（2）可得特征观察值序列*o* l，将其作为训练所得HMM 模型的输入值，并应用**前后向递推算法**求其似然值，判断出最大可能手势。
![](https://img-blog.csdn.net/20151222121215200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考文献：
[1]段洪伟. 基于视觉的手势识别及其应用 [D]. 上海：上海大学，2004
 [2]NUMMIARO K，KOLLER-MEIER E，VAN GOOLL. An adaptive color-based particle filter [J]. Image and VisionComputing，2002，21（1）：99-110
[3]齐苏敏，黄贤武，孟静. 基于自适应活动轮廓模型的实时手势跟踪 [J]. 计算机科学，2006，33（11）：192-194.
[4]孔晓明. 人机交互系统中动态手势识别的研究 [D].上海：上海大学，2005
[5]FREEMAN W，WEISSMAN C. Television control by hand gestures [C]. Proc IEEE Int'I Conf. on Automated Face and Gesture Recognition （FG'95），1995：179-183
[6]RAMAMOORTHY A ， VASWANI N ， etl.all.Recognition of dynamic hand gestures [J]. Pattern Recognition，36（9）：2069-2081
