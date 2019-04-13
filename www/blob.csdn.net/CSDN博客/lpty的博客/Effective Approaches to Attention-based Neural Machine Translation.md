
# Effective Approaches to Attention-based Neural Machine Translation - lpty的博客 - CSDN博客

2018年12月28日 10:45:54[lpty](https://me.csdn.net/sinat_33741547)阅读数：124



# 引言
Minh-Thang Luong等在2015年arXiv上的论文，原文地址：[地址](https://arxiv.org/pdf/1508.04025v3.pdf)
这是继Bahdanau提出attention用于NLP的后续工作，作者提出了两种新的，相对于前面工作简单但有效的网络，一种称为global方法，实现上与Bahdanau使用的方式很相似，在每一次生成目标词时，都需要所有对源语句隐藏状态计算相似度，但相对来说更为直接简单；另一种称为local方法，不同的是在计算时只需要对源语句某个子集计算相似度，之后基于这个子集生成context vector。
此外，作者对比了不同的对齐函数在模型上所取得的效果，使用文中提出的模型，作者在英文翻译到德文的多项任务中取得了SOTA的结果。
# 模型
### Neural Machine Translation
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227173025996.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
一般的神经网络翻译模型，都是直接对条件概率$p(y|x)$建模，其中x为源语句，y为目标语句。具体到上述图中，蓝色部分的encoder，将目标语句编译成语义表示s，而红色部分的decoder接收源语句表示s，逐个字翻译目标语句，表达式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227173920626.png)
在上述端到端的模型中，一般在神经单元后续加上一个softmax函数，生成一个字典大小的概率分布向量，决定当前生成的字，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227174227636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
如上所示，在整个计算过程中，源语句语义表示s都是不变的，如果通过一些方式使得s可以动态的变化，就是attention模型了。
[
](https://img-blog.csdnimg.cn/20181227174227636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### Attention-based Models
[
](https://img-blog.csdnimg.cn/20181227174227636.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)Attention模型实际上讲的就是如何计算动态的源语句语义表示，这里称为c，对应的t时间步的语义表示则称为$c_t$。
对于global或者local方法来说，不同的只是通过当前时间t隐层输出$h_t$与源语句隐层输出$h_s$计算出context$c_t$的方式，其后续步骤都是一致的，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227180611444.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227180543230.png)
上述公式中，将计算得出的context$c_t$与当前隐层输出$h_t$拼接，通过简单的计算生成attention隐层输出，加上softmax得到最终结果，下面看一下如何计算context$c_t$。
[
](https://img-blog.csdnimg.cn/20181227180543230.png)
#### Global attentional model
[
](https://img-blog.csdnimg.cn/20181227180543230.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227175704377.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
对于context$c_t$的计算，都是通过当前隐层输出$h_t$分别与源隐层输出$h_s$进行某种相似度比对，得到一个可变长度的对齐向量$a_t$，最后将$a_t$与源隐层输出$h_s$计算加权平均得到，如：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228091341197.png)
上述式子中的score函数，作者实验了以下几种方式，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228091447368.png)
还有一种在早期的实验中提出的计算$a_t$的方法，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228091522772.png)
可以看到，本文中所提出的方法与之前Bahdanau的工作中提出的计算路径有了一些不同，本文的计算路径如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228092601882.png)
而在Bahdanau的工作中，计算路径如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228092806810.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228092827191.png)
[
](https://img-blog.csdnimg.cn/20181228092806810.png)
#### Local attention model
[
](https://img-blog.csdnimg.cn/20181228092806810.png)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228092951583.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
对于globa方式，每次都必须与所有源隐层输出进行计算，在面对长序列时计算量会变得很大，所以作者又提出了local方式，对于每个时间步t都为其定义一个对应源句的位置$p_t$，通过经验给定的窗口大小D确定观测范围$[p_t - D, p_t + D]$，之后的计算方式与global一致，得到对应的加权平均值。
位置$p_t$的定义，作者提出了以下两种方式，
###### Monotonic alignment (local-m)
作者认为在翻译任务中，原句与目标句在一定程度上是单调对应的，所以定义$p_t = t$，其中t为时间步，由此计算context$c_t$。
###### Predictive alignment (local-p)
与上述不同，针对每个目标输出，作者定义了一个方法预测其在原句的对齐位置，
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018122810023848.png)
其中，S为原语句长度，通过与sigmoid后的值计算得出目标位置，这里还有一个小trick，作者引入了一个服从于$N(p_t, D/2)$的高斯分布来对齐权重，从直觉上考虑，距离目标位置越近的词理当起到更到的作用，
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018122810104067.png)
同时也可以看到，在没有引入高斯分布之前，位置$p_t$并没有直接与网络相连，虽然计算$p_t$的过程可以微分，但是作为窗口中心这个操作是不可微的，因此也需要某种额外的方式将其与网络关联起来。
#### Input-feeding Approach
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228102001626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
在翻译任务中，需要注意哪些词语已经在前面被翻译过了，但是在上述模型中，计算attentino时并没有包含前述信息，因此作者额外增加了input feeding，将上一个时间步的attention隐层输出作为输入，用来计算下一时间步的目标词。
[
](https://img-blog.csdnimg.cn/20181228102001626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 实验
[
](https://img-blog.csdnimg.cn/20181228102001626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### English-German Results
[
](https://img-blog.csdnimg.cn/20181228102001626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228103134248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### German-English Results
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228103443195.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 分析
### Attentional Architectures
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228103824631.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
从上述结果可以看到，在globa方法中，使用dot能得到最好的结果；而在local方法中，使用general得到最好的结果。
从所有方法来看，local方法的模型质量最高。
### Alignment Quality
作者提出使用AER来评价对齐质量，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181228104112623.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
# 结论
attention的后续工作，扩展了attention的计算方式，同时local attention使用局部注意力的方式很有启发。
# 引用
1、Effective Approaches to Attention-based Neural Machine Translation
2、Neural Machine Translation by Jointly Learning to Align and Translate

