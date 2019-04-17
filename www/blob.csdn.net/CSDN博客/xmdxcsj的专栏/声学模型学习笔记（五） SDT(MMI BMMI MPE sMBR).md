# 声学模型学习笔记（五） SDT(MMI/BMMI/MPE/sMBR) - xmdxcsj的专栏 - CSDN博客





2016年10月08日 19:58:22[xmucas](https://me.csdn.net/xmdxcsj)阅读数：6872








DNN训练使用的CE准则是基于每一帧进行分类的优化，最小化帧错误率，但是实际上语音识别是一个序列分类的问题，更关心的是序列的准确性。所以引入SDT(sequence-discriminative training)，训练准则更符合实际，有利于提升识别率。常用的准则包括MMI/BMMI、MPE、MBR等。
|准则|目标函数|
|----|----|
|CE|帧错误率|
|MMI/BMMI|句子正确率|
|MPE|phone错误率|
|sMBR|状态错误率|

## MMI

MMI(maximum mutual information)准则最大化观察序列分布和word序列分布之间的互信息，减小句子错误率。 

假设观察序列$o^m=o_1^m,...,o_{T_m}^m$，word序列$w^m=w_1^m,...,w_{N_m}^m$，其中$m$表示utterance，$T_m$表示帧数，$N_m$表示word个数。训练集为$S=\{(o^m,w^m)|0\le m\le M\}，$MMI准则可以表示如下: 


$J_{MMI}(\theta;S)=\sum_{m=1}^MJ_{MMI}(\theta;o^m,w^m)=\sum_{m=1}^MlogP(w^m|o^m;\theta)$


$=\sum_{m=1}^Mlog\frac{p(o^m|s^m;\theta)^kP(w^m)}{\sum_w p(o^m|s^w;\theta)^k P(w)}$

其中$k$表示acoustic scale，$\theta$表示模型参数，$s^m$表示状态序列。物理意义可以理解为：分子表示准确结果对应路径的总得分（声学和语言），分母表示所有路径对应的得分总和（为了计算上的可操作性，实际用lattice简化表示）。模型参数的梯度可以表示如下： 


$\nabla{J_{MMI}(\theta;o^m,w^m)}=\sum_m\sum_t \nabla_{z_{mt}^L}{J_{MMI}(\theta;o^m,w^m)} \frac{\partial z_{mt}^L}{\partial \theta}=\sum_m\sum_t \ddot{e}_{mt}^L\frac{\partial z_{mt}^L}{\partial \theta}$

其中$z_{mt}^L$表示softmax层的输入（没有做softmax运算），跟CE准则的不同体现在$\ddot{e}_{mt}^L$，进一步计算如下: 


$\ddot{e}_{mt}^L(i)=\nabla_{z_{mt}^L(i)}{J_{MMI}(\theta;o^m,w^m)}$


$=\sum_r \frac{\partial J_{MMI}(\theta;o^m,w^m)}{\partial logp(o_t^m|r)}\frac{\partial logp(o_t^m|r)}{\partial z_{mt}^L(i)}$
### 第一部分



$\frac{\partial J_{MMI}(\theta;o^m,w^m)}{\partial logp(o_t^m|r)}$


$=\frac{\partial log\frac{p(o^m|s^m)^kP(w^m)}{\sum_w p(o^m|s^w)^k P(w)}}{\partial logp(o_t^m|r)}$


$=k\frac{\partial log p(o^m|s^m)}{\partial log p(o_t^m|r)}-\frac{\partial log\sum_w p(o^m|s^w)^k P(w)}{\partial logp(o_t^m|r)}$

考虑到$p(o^m|s^m)=p(o_1^m|s_1^m)p(o_2^m|s_2^m)...p(o_{T_m}^m|s_{T_m}^m)$，所以上式第一项可以简化为： 
$k\frac{\partial p(o^m|s^m)}{\partial logp(o_t^m|r)}=k(\delta(r=s_t^m))$

第二项可以进一步求导： 


$\frac{\partial log\sum_w p(o^m|s^w)^k P(w)}{\partial logp(o_t^m|r)}$


$=\frac{\partial log\sum_w e^{logp(o^m|s^w)^k P(w)}}{\partial logp(o_t^m|r)}$


$=\frac{1}{\sum_w e^{logp(o^m|s^w)^k P(w)}}\frac{\partial \sum_w e^{log p(o^m|s^w)^k P(w)}}{\partial logp(o_t^m|r)}$


$=\frac{1}{\sum_w p(o^m|s^w)^k P(w)}*\sum_w e^{logp(o^m|s^w)^k P(w)}*\frac{\partial log p(o^m|s^w)^k P(w)}{\partial logp(o_t^m|r)}$


$=\frac{1}{\sum_w p(o^m|s^w)^k P(w)}*\sum_w p(o^m|s^w)^k P(w)*\delta (s_t^m=r)$


$=\frac{\sum_{w:s_t=r} p(o^m|s^w)^k P(w)}{\sum_w p(o^m|s^w)^k P(w)}$

综合前面的第一项和第二项，可得： 


$\frac{\partial J_{MMI}(\theta;o^m,w^m)}{\partial logp(o_t^m|r)}=k(\delta(r=s_t^m)-\frac{\sum_{w:s_t=r} p(o^m|s^m)^k P(w)}{\sum_w p(o^m|s^m)^k P(w)})$
### 第二部分

考虑到$p(x|y)*p(y)=p(y|x)*p(x)$，第二部分可以表示如下： 


$\frac{\partial logp(o_t^m|r)}{\partial z_{mt}^L(i)}$


$=\frac{\partial log p(r|o_t^m)-logp(r)+logp(o_t^m)}{\partial z_{mt}^L(i)}$


$=\frac{\partial log p(r|o_t^m)}{\partial z_{mt}^L(i)}$

其中$p(r|o_t^m)$表示DNN的第r个输出， 


$p(r|o_t^m)=softmax_r(z_{mt}^L)=\frac{e^{z_{mt}^L(r)}}{\sum_j e^{z_{mt}^L(j)}}$

所以， 


$\frac{\partial logp(o_t^m|r)}{\partial z_{mt}^L(i)}=\delta(r=i)$

按照文章的推导应该得到这个结果，但是实际上分母还包含$z_{mt}^L(i)$，是不是做了近似认为分母是常量，这一步有疑问？？？？
综合上面两部分，可以得到最终的公式: 


$\ddot{e}_{mt}^L(i)=k(\delta(i=s_t^m)-\frac{\sum_{w:s_t=i} p(o^m|s^m)^k P(w)}{\sum_w p(o^m|s^m)^k P(w)})$

## Boosted MMI



$J_{BMMI}(\theta;S)=\sum_{m=1}^MJ_{BMMI}(\theta;o^m,w^m)=\sum_{m=1}^Mlog \frac{P(w^m|o^m)}{\sum_w P(w|o^m)e^{-bA(w,w^m)}}$


$=\sum_{m=1}^Mlog \frac{P(o^m|w^m)^kP(w^m)}{\sum_w P(o^m|w^m)^k P(w)e^{-bA(w,w^m)}}$

相比于MMI，BMMI在分母上面增加了一个权重系数$e^{-bA(w,w^m)}$，一般$b=0.5$,$A(w,w^m)$是$w$和$w^m$之间准确率的度量，可以是word/phoneme/state级别的准确率。 

物理意义： 

参考[3]给出的解释，We boost the likelihood of the sentences that have more errors, thus generating more confusable data. Boosted MMI can viewed as trying to enforce a soft margin that is proportional to the number of errors in a hypothesised sentence。 

结合参数理解，就是$w$和$w^m$越接近(错误的word越少)，$e^{-bA(w,w^m)}$这个权重越小，相反，权重会越大，增加了数据的困惑度。 

通过可以推导出误差信号： 


$\ddot{e}_{mt}^L(i)=k(\delta(i=s_t^m)-\frac{\sum_{w:s_t=i} p(o^m|s^w)^k P(w) e^{-bA(w,w^m)}}{\sum_w p(o^m|s^w)^k P(w) e^{-bA(w,w^m)}})$
## MPE/sMBR

MBR(minimum Bayes risk)的目标函数是最小化各种粒度指标的错误，比如MPE是最小化phone级别的错误，sMBR最小化状态的错误。目标函数如下： 


$J_{MBR}(\theta;S)=\sum_{m=1}^MJ_{MBR}(\theta;o^m,w^m)=\sum_{m=1}^M \sum_w P(w|o^m) A(w,w^m)$


$=\sum_{m=1}^M \frac{\sum_w P(o^m|s^w)^kP(w) A(w,w^m)}{\sum_{w'} P(o^m|s^{w'})^k P(w')}$

其中$A(w,w^m)$表示两个序列之间的差异，MPE就是正确的phone的个数，sMBR是指正确的state的个数。求导可得： 


$\ddot{e}_{mt}^L(i)=\nabla_{z_{mt}^L(i)}{J_{MBR}(\theta;o^m,w^m)}$


$=\sum_r \frac{\partial J_{MBR}(\theta;o^m,w^m)}{\partial logp(o_t^m|r)}\frac{\partial logp(o_t^m|r)}{\partial z_{mt}^L(i)}$
### 第一部分

对于MPE，参考文献[4]： 

首先将$J_{MBR}(\theta;o^m,s^m)$分子分母求和部分分为两块，$r\in s^w$和$r\notin s^w$


$J_{MBR}(\theta;o^m,s^m)=\frac{\sum_s P(o^m|s)^kP(s) A(s, s^m)}{\sum_{s'} P(o^m|s')^k P(s')}$


$= \frac{\sum_{s:r\in s} P(o^m|s)^kP(s) A(s, s^m)+\sum_{s:r\notin s} P(o^m|s)^kP(s) A(s, s^m)}{\sum_{s':r\in s'} P(o^m|s')^k P(s')+\sum_{s':r\notin s'} P(o^m|s')^k P(s')}$

- 如果满足$r\in s$，那么导数满足以下关系： 


$\frac{\partial P(o^m|s)^k}{\partial log p(o_t^m|r)}=\frac{\partial e^{k*logP(o^m|s)}}{\partial log p(o_t^m|r)}=k*P(o^m|s)^k$

- 如果不满足$r\in s$，那么导数将为0： 


$\frac{\partial P(o^m|s)^k}{\partial log p(o_t^m|r)}=0$
不难推出： 


$\frac{\partial J_{MBR}(\theta;o^m,s^m)}{\partial logp(o_t^m|r)}$


$=k*\frac{\sum_{s:r\in s} P(o^m|s)^kP(s) A(s, s^m)}{\sum_{s'} P(o^m|s')^k P(s')}-k*\frac{\sum_s P(o^m|s)^kP(s) A(s, s^m)}{\sum_{s'} P(o^m|s')^k P(s')}*\frac{\sum_{s:r\in s} P(o^m|s)^kP(s)}{\sum_{s'} P(o^m|s')^k P(s')}$

上面的等式可以简化为以下形式： 


$\frac{\partial J_{MBR}(\theta;o^m,s^m)}{\partial logp(o_t^m|r)}=k*\ddot{\gamma}_{mt}^{DEN}(r)(\bar{A}^m(r=s_t^m)-\bar{A}^m) $

各个部分的定义如下： 


$\ddot{\gamma}_{mt}^{DEN}(r)=\frac{\sum_{s:r\in s} P(o^m|s)^kP(s)}{\sum_{s'} P(o^m|s')^k P(s')}$


$\bar{A}^m=\frac{\sum_s P(o^m|s)^kP(s) A(s, s^m)}{\sum_{s'} P(o^m|s')^k P(s')}$


$\bar{A}^m(r=s_t^m)=\mathbb E(A(s, s^m))=\frac{\sum_{s:r\in s} P(o^m|s)^kP(s)A(s,s^m)}{\sum_{s':r\in s'} P(o^m|s')^k P(s')}$

第一项表示occupancy statistics 

第二项表示lattice中所有路径的平均准确率 

第三项表示lattice中所有经过r的路径的平均准确率，是$A(s, s^m)$的均值，可以将三个三项合并起来进行还原就很容易里面均值的含义。
### 第二部分

第二部分和MMI的一致

## tricks

### lattice generation

区分性训练时生成高质量的lattice很重要，需要使用最好的模型来生成对应的lattice，并且作为seed model。

### lattice compensation

如果lattice产生的不合理的话，会导致计算出来的梯度异常，比如分子的标注路径没有在分母中的lattice出现，这种情况对于silience帧尤其常见，因为silience经常出现在分子的lattice，但是很容易被分母的lattice忽略。有一些方法可以解决这种问题： 

- fame rejection，直接删除这些帧 

- 根据reference hypothesis修正lattice，比如在lattice中人为地添加一下silience边
### frame smoothing

SDT很容易出现overfitting，两方面原因 

- sparse lattice 

- sdt的squence相比于frame增加了建模的维度，导致训练集的后验概率分布更容易跟测试集出现差异
可以修改训练准则来减弱overfitting，通过结合squence criteria和frame criteria来实现： 


$J_{FS-SEQ}(\theta;S)=(1-H)J_{CE}(\theta;S)+HJ_{SEQ}(\theta;S)$
$H$成为smoothing factor，经验值设为$4/5$到$10/11$
### learning rate

SDT的学习率相比于CE要下，因为 

- SDT的起点一般基于CE训练出来的model 

- SDT训练容易出现overfittingcriterion selection

sMBR效果相比其他会好一点，MMI比较容易理解和实现。noise contrastIve estimation

NCE可以用于加速训练参考

[1]《automatic speech recognition a deep learning approach》 chapter8 

[2]Sequence-discriminative training of deep neural networks 

[3]Boosted MMI for model and feature-space discriminative training 

[4]discriminative training for large vocabulary speech recognition {daniel povey的博士论文chapter6}













