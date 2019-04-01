# 语言模型srilm



# 一、基本用法



## 一、基本训练

### 基本参数

```shell
##功能
#读取分词后的text文件或者count文件，然后用来输出最后汇总的count文件或者语言模型
##参数
#输入文本：
#  -read 读取count文件
#  -text 读取分词后的文本文件
#词典文件：
#  -vocab 限制text和count文件的单词，没有出现在词典的单词替换为<unk>；如果没有，所有的单词将会被自动加入词典
#  -limit-vocab 只限制count文件的单词（对text文件无效），没有出现在词典里面的count将会被丢弃
#  -write-vocab 输出词典
#语言模型：
#  -lm 输出语言模型
#  -write-binary-lm 输出二进制的语言模型
#  -sort 输出语言模型gram排序
123456789101112131415
```

有两种训练方法，分别如下：

```shell
#choice1: text->count->lm
#ngram-count -text $text -vocab ${vocab} -order 2 -sort -tolower -lm ${arpa}

#choice2: text->count count->lm
#ngram-count -text ${text} -order 2 -sort -tolower -write ${count}12345
```

## 二、语言模型打分

```shell
##功能
#用于评估语言模型的好坏，或者是计算特定句子的得分，用于语音识别的识别结果分析。
##参数
#计算得分：
#  -order 模型阶数，默认使用3阶
#  -lm 使用的语言模型
#  -use-server S 启动语言模型服务，S的形式为port@hostname
#  -ppl 后跟需要打分的句子（一行一句，已经分词），ppl表示所有单词，ppl1表示除了</s>以外的单词
#    -debug 0 只输出整体情况
#    -debug 1 具体到句子
#    -debug 2 具体每个词的概率
#产生句子：
#  -gen 产生句子的个数
#  -seed 产生句子用到的random seed
ngram -lm ${lm} -order 2 -ppl ${file} -debug 1 > ${ppl}123456789101112131415
```

## 三、语言模型剪枝

```shell
##功能
#用于减小语言模型的大小，剪枝原理参考(http://blog.csdn.net/xmdxcsj/article/details/50321613)
##参数
#模型裁剪：
#  -prune threshold 删除一些ngram，满足删除以后模型的ppl增加值小于threshold，越大剪枝剪得越狠
#  -write-lm 新的语言模型
ngram -lm ${oldlm} -order 2 -prune ${thres} -write-lm ${newlm}1234567
```

## 四、语言模型合并

```shell
##功能
#用于多个语言模型之间插值合并，以期望改善模型的效果
##参数
#模型插值：
#  -mix-lm 用于插值的第二个ngram模型，-lm是第一个ngram模型
#  -lambda 主模型（-lm对应模型）的插值比例，0~1，默认是0.5
#  -mix-lm2 用于插值的第三个模型
#  -mix-lambda2 用于插值的第二个模型（-mix-lm对应的模型）的比例，那么第二个模型的比例为1-lambda-mix-lambda2
#  -vocab 当两个模型的词典不一样的时候，使用该参数限制词典列表，没有效果
#  -limit-vocab 当两个模型的词典不一样的时候，使用该参数限制词典列表，没有效果
ngram -lm ${mainlm} -order 2 -mix-lm ${mixlm} -lambda 0.8 -write-lm ${mergelm}1234567891011
```

在合并语言模型之前，可以使用脚本计算出最好的比例，参考srilm的compute-best-mix脚本

## 五、语言模型使用词典限制

有两种方法可以根据给定的词典对模型加以限制 
一种是在训练的时候使用-vocab限制 
另外一种是在训练完成以后，使用srilm的脚本，如下：

```shell
##功能
#对已有的语言模型，使用新的字典进行约束，产生新的语言模型
#1.n-grams的概率保持不变
#2.回退概率重新计算
#3.增加新的一元回退概率
##参数
#模型裁剪：
#  -vocab 词典单词的列表，不包括发音
#  -write-lm 新的语言模型
change-lm-vocab -vocab ${vocab} -lm ${oldlm} -write-lm ${newlm} -order 2
```







# 二、prune剪枝



## 为什么引入剪枝

传统的N-gram backoff模型，提升性能的两条主要路径是增加阶数和增加语料，两者带来的共同副作用是增加了模型的大小，进而增加了语音识别解码器的内存占用。为了减少模型的大小，同时保证性能最大化，引入了prune。 
为了实现剪枝选取的准则需要满足以下三个条件：

- soundness 
  The criterion should optimize some wellunderstood information-theoretic measure of language 
  model quality.
- efficiency 
  高效、快速
- self-containedness 
  从实用性角度考虑，只有当前的语言模型可用，没有其他多余的信息可用

srilm采用了基于相对熵的剪枝方法（N-gram pruning based on relative entropy）。

------

## 基于相对熵的剪枝

剪枝的主要目的是为了删除已有ngram，同时保证未删除的ngram不变，还需要重新计算回退概率。 
那么怎么样衡量剪枝以后语言模型性能的改变呢？ 
*一个想法*就是去最小化剪枝前后两个模型之间概率分布的距离。自然的，我们会选择相对熵或者KL距离，定义参考[链接](http://blog.csdn.net/xmdxcsj/article/details/50051579): 
$$
D(p||p')=-\sum_{w_i,h_j}p(w_i,h_j)[logp'(w_i|h_j)-logp(w_i|h_j)]
$$
其中p表示裁剪前语言模型的概率，p′表示裁剪后的模型。

我们不可能遍历所有需要裁剪的ngram的集合（指数型的复杂度），从可操作性考虑，这里需要假设所以ngram对相对熵的影响都是独立的，有了这个假设，我们就可以计算删除每一条ngram对应的相对熵，然后按照大小进行排序，删除那些相对熵最小（剪掉以后对原语言模型影响最小）的ngram。

另外一个想法就是根据模型裁剪前后ppl值的相对变化来衡量裁剪对模型的影响。 

原始模型的ppl值：
$$
PPL=e^{-\sum_{h,w}p(h,w)logp(w|h)}
$$
裁剪后模型的ppl值：
$$
PPL'=e^{-\sum_{h,w}p(h,w)logp'(w|h)}
$$
所以困惑度的相对变化可以表示为：
$$
\frac{PPL'-PPL}{PPL}=e^{D(p||p')}-1
$$
所以，裁剪语言模型的流程如下： 
\1. 给定一个困惑度相对变化的门限threshold 
\2. 计算删除单独一条ngram，模型困惑度的相对变化 
\3. 挑选出那些低于门限值的ngram，删除，然后重新计算回退权重

------

## 基于相对熵剪枝的计算方法

如果删除一条ngram(h,w)，h表示历史词，w表示当前词，h’表示历史词去掉第一个词，那么将会带来两方面的影响：

- h的回退概率α(h)，将变为α′(h)；与此同时，所有历史是h对应的回退概率将会改变，统一将这些ngram表示为BO(wi,h)
- p(w|h)将变为$p'(w|h)=\alpha'(h)p(w|h')$

对于历史词不是h的所有ngram，概率值不变，所以这里只需要考虑： 
\1. 历史词$h_i$： 
$h$
\2. 当前词$w_i$ 
一个是$w$；另外一个是涉及回退概率h对应的w（$BO(w_i,h)$） 
所以相对熵可以写成： 
$$
D(p||p')=-\sum_{w_i,h_j}p(w_i,h_j)[logp'(w_i|h_j)-logp(w_i|h_j)]\\
=-p(w,h)[logp'(w|h)-logp(w|h)]-\sum_{w_i\in BO(w_i,h)}p(w_i,h)[logp'(w_i|h)-logp(w_i|h)]\\
=-p(h)\{p(w|h)[logp'(w|h)-logp(w|h)]-p(w_i|h)\sum_{w_i\in BO(w_i,h)}p(w_i,h)[logp'(w_i|h)-logp(w_i|h)]\}
$$
计算上式的后面一项，需要遍历词典，对于词典很大的语言模型，计算复杂度较高。可以考虑回退概率：
$$
p'(w|h)=\alpha'(h)p(w|h')\\
logp'(w_i|h)-logp(w_i|h)=log\alpha'(h)+logp(w_i|h')-log\alpha(h)-logp(w_i|h')=log\alpha'(h)-log\alpha(h)
$$
所以，相对熵将变为：
$$
D(p||p')=-p(h)\{p(w|h)[logp(w|h')+log\alpha'(h)-logp(w|h)]+[log\alpha'(h)-log\alpha(h)]\sum_{w_i\in BO(w_i,h)}p(w_i,h)
$$


到现在，未知量包括三项：$p(h)、α′(h)$和BO对应的累加项。

 

对于第一项，可以根据条件概率依次展开算出

 

对于第二项，回退概率的计算公式如下：
$$
\alpha(h)=\frac{1-\sum_{w_i\notin BO(w_i,h)}p(w_i|h)}{1-\sum_{w_i\notin BO(w_i,h)}p(w_i|h')}
$$
类似于α(h)，α′(h)只是在分子和分母分别多加上一个p(w|h)和p(w|h′)，每一个词历史h只需要计算一次。

对于第三项，也是每一个h只需要计算一次，如下
$$
\sum_{w_i\in BO(w_i,h)}p(w_i,h)=1-\sum_{w_i\notin BO(w_i,h)}p(w_i,h)
$$


------

## 参考文献

[Entropy-based Pruning of Backoff Language Models](https://www.sri.com/sites/default/files/publications/entropy-based_pruning_of_backoff_language_models.pdf)





# 三、折扣平滑算法





## 一、概况

在训练语言模型的时候有几个常用词：折扣（discounting）、平滑（smoothing）、插值（interpolate），下面用一个例子简单介绍一下。 
我们的训练语料里面有“a、b、c、d”四个词，其中以a、b开头的3gram有以下数据： 
a b c 5次 
a b d 10次 
那么当我们需要计算p(c|ab)的概率的时候，可以使用最大似然估计 
$$
p(c|ab)=\frac{c(abc)}{c(ab)}
$$
用“a b c”的次数除以“a b”的总个数，上式的概率值为5/15。但是，按照上面的计算方法，$p(a|ab)$的概率会始终为0，这样就会引入问题，由于我们训练语料有限，导致会出现概率为0的ngram。为了解决上面的问题，引入了**折扣discounting**方法，也叫做**平滑smoothing**方法。

折扣的含义就是从已有gram的概率中拿出来一部分给未出现的gram，同时保证概率和为1。有**回退（backoff）**

和**插值（interpolate）**两种方法来实现平滑。



### （一）、回退

引入回退概率bow(ab)，对于没有出现的gram（例子中的aba），可以使用回退概率表示如下： 
$$
p(a|ab)=bow(ab)*p(a|b)
$$
与此同时，对于出现过的gram，需要减去一部分值给回退概率，以保证$∑p(∗|ab)=1$，可以表示如下： 
$$
p(c|ab)=\frac{c(abc)-D}{c(ab)}\\
p(d|ab)=\frac{c(abd)-D}{c(ab)}
$$
由概率和为1的限制： 
$$
p(c|ab)+p(d|ab)+p(a|ab)+p(b|ab)=1
$$
可以得出回退概率bow(ab)： 
$$
bow(ab)=\frac{1-p(c|ab)-p(d|ab)}{p(a|b)+p(b|b)}
$$
如果给出折扣变量D的值，那么就可以根据上式计算出bow(ab)。不同的算法计算D的方式不同。

### （二）、插值

和上面的回退类似，引入回退概率来实现对没有出现的gram的概率表示，不同之处在于不仅未出现的gram引入回退，已出现的gram概率的计算也要引入回退： 
$$
p(a|ab)=bow(ab)*p(a|b)\\
p(b|ab)=bow(ab)*p(b|b)\\
p(c|ab)=\frac{c(abc)-D}{c(ab)}+bow(ab)*p(c|b)\\
p(d|ab)=\frac{c(abd)-D}{c(ab)}+bow(ab)*p(d|b)
$$
根据经验，插值相比于回退来讲，效果更好。

### （三）、总结

参考[ngram-discount](http://www.speech.sri.com/projects/srilm/manpages/ngram-discount.7.html) 
假设Z表示全集，Z1表示c(a_z)>0的集合，Z0表示c(a_z)=0的集合 
回退方法： 
$$
p(a\_z)=(c(a\_z)>0) ? f(a\_z) : bow(a\_)p(\_z)\\
bow(a\_)=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 p(\_z))\\=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 f(\_z))
$$


插值方法： 
$$
p(a\_z)=(c(a\_z)>0) ? g(a\_z)+bow(a\_)p(\_z) : bow(a\_)p(\_z)\\
bow(a\_)=1-Sum\_Z1 g(a\_z)
$$

## 二、平滑算法

有很多平滑算法，平滑算法默认都是使用回退的形式，部分算法使用-interpolate参数可以使用插值方式。

### （一）、Good-Turing

Good-Turing是ngram-count默认使用的算法，针对gram的出现次数分别处理。次数的限制使用参数-gtnmax控制，默认的1gram的次数大于1，其他gram的次数大于7认为是可信的。f(a_z)的计算方法如下： 
1.c(a_z) > gtmax 
概率的计算使用原始的最大似然估计： 
$$
f(a\_z)=\frac{c(a\_z)}{c(a\_)}
$$
2.1 <= c(a_z) <= gtmax 
这种情况，认为次数是不可信的，需要对该次数做出修正，如下： 
$$
A=(gtmax+1)\frac{n[gtmax+1]}{n[1]}\\
c'(a\_z)=(c(a\_z)+1)\frac{n[c(a\_z)+1]}{n[c(a\_z)]}\\
f(a\_z)=\frac{c(a\_z)}{c(a\_)}\frac{(c'(a\_z)/c(a\_z)-A)}{(1-A)}
$$
上式中n[x]代表出现次数是x的gram的个数。 
该平滑算法不支持插值，只支持回退，所以最终的概率计算如下： 
$$
p(a\_z)=(c(a\_z)>0) ? f(a\_z):bow(a\_)p(\_z)\\
bow(a\_)=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 f(\_z))
$$

### （二）、Kneser-Ney

该算法的思想是修改回退概率的计算方法。 
\1. 对于回退方法来讲，公式修改为如下形式： 
$f(a\_z)=\frac{c(a\_z)-D0}{c(a\_)}$高阶gram 
$f(\_z)=\frac{n(*\_z)-D1}{n(*\_*)}$低阶gram 
其中，n(*_*)表示满足*_*格式的gram的种类数。 
\2. 对于插值方法来讲，公式修改为如下形式： 
$g(a\_z)=max(0, c(a\_z)-D)/c(a\_)$高阶gram 
$bow(a\_)=1-Sum\_Z1 g(a\_z)=\frac{c(a\_)-Sum\_Z1c(a\_z)+Sum\_Z1D}{c(a\_)}$

$=\frac{Sum\_Z1D}{c(a\_)}=\frac{n(a\_*)D}{c(a\_)}$高阶gram 
假设Z2表示集合n(*_z)>0，那么可以得到低阶gram回退概率： 

$g(\_z)=max(0, n(*\_z)-D)/n(*\_*)$低阶gram 
$bow(\_)=1-Sum\_Z2 g(\_z)=\frac{n(*\_*)-Sum\_Z2n(*\_z)+Sum\_Z2D}{n(*\_*)}=\frac{Dn(\_*)}{n(*\_*)}$低阶gram 
\3. D的计算方法 
原始的Knser-Ney方法(-ukndiscount)对于每一阶的ngram使用相同的常数D 
$D=\frac{n1}{n1+2*n2}$
n1表示出现一次的ngram的个数，n2表示出现两次的ngram的个数 
后来，Chen和Goodman修改了常数D的计算方法(-kndiscount)，针对每一阶的ngram，都有三个常数： 
D1-出现一次的ngram个数 
D2-出现两次的ngram个数 
D3-出现三次及以上的ngram个数 
计算如下： 
$$
Y=\frac{n1}{n1+2*n2}\\
D1=1-2Y\frac{n2}{n1}\\
D2=2-3Y\frac{n3}{n2}\\
D3=3-4Y\frac{n4}{n3}
$$

## 三、tricks

- 效果最好的modified Knser-Ney算法，使用插值方法，即-kndiscount -interpolate
- -gtnmin表示出现次数小于特定值的gram将会被丢弃，对于所有的平滑算法都一样。默认是所有的1元和2元都会保留，其他阶的count>=2的才会保留。
- -gtnmax表示次数小于特定值的count将会被修改，只是针对Good-Turing算法而言。



