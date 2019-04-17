# 语言模型srilm（三） 折扣平滑算法 - xmdxcsj的专栏 - CSDN博客





2015年12月21日 18:06:01[xmucas](https://me.csdn.net/xmdxcsj)阅读数：5467








## 一、概况

在训练语言模型的时候有几个常用词：折扣（discounting）、平滑（smoothing）、插值（interpolate），下面用一个例子简单介绍一下。 

我们的训练语料里面有“a、b、c、d”四个词，其中以a、b开头的3gram有以下数据： 

a b c 5次 

a b d 10次 

那么当我们需要计算p(c|ab)的概率的时候，可以使用最大似然估计 


$p(c|ab)=\frac{c(abc)}{c(ab)}$

用“a b c”的次数除以“a b”的总个数，上式的概率值为$\frac{5}{15}$。但是，按照上面的计算方法，$p(a|ab)$的概率会始终为0，这样就会引入问题，由于我们训练语料有限，导致会出现概率为0的ngram。为了解决上面的问题，引入了**折扣discounting**方法，也叫做**平滑smoothing**方法。 

折扣的含义就是从已有gram的概率中拿出来一部分给未出现的gram，同时保证概率和为1。有**回退（backoff）**和**插值（interpolate）**两种方法来实现平滑。
### （一）、回退

引入回退概率bow(ab)，对于没有出现的gram（例子中的aba），可以使用回退概率表示如下： 
$p(a|ab)=bow(ab)*p(a|b)$

与此同时，对于出现过的gram，需要减去一部分值给回退概率，以保证$\sum p(*|ab)=1$，可以表示如下： 
$p(c|ab)=\frac{c(abc)-D}{c(ab)}$
$p(d|ab)=\frac{c(abd)-D}{c(ab)}$

由概率和为1的限制： 
$p(c|ab)+p(d|ab)+p(a|ab)+p(b|ab)=1$

可以得出回退概率bow(ab)： 
$bow(ab)=\frac{1-p(c|ab)-p(d|ab)}{p(a|b)+p(b|b)}$

如果给出折扣变量D的值，那么就可以根据上式计算出bow(ab)。不同的算法计算D的方式不同。
### （二）、插值

和上面的回退类似，引入回退概率来实现对没有出现的gram的概率表示，不同之处在于不仅未出现的gram引入回退，已出现的gram概率的计算也要引入回退： 
$p(a|ab)=bow(ab)*p(a|b)$
$p(b|ab)=bow(ab)*p(b|b)$
$p(c|ab)=\frac{c(abc)-D}{c(ab)}+bow(ab)*p(c|b)$
$p(d|ab)=\frac{c(abd)-D}{c(ab)}+bow(ab)*p(d|b)$

根据经验，插值相比于回退来讲，效果更好。
### （三）、总结

参考[ngram-discount](http://www.speech.sri.com/projects/srilm/manpages/ngram-discount.7.html)

假设Z表示全集，Z1表示c(a_z)>0的集合，Z0表示c(a_z)=0的集合 

回退方法： 
$p(a\_z)=(c(a\_z)>0) ? f(a\_z) : bow(a\_)p(\_z)$
$bow(a\_)=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 p(\_z))=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 f(\_z))$

插值方法： 
$p(a\_z)=(c(a\_z)>0) ? g(a\_z)+bow(a\_)p(\_z) : bow(a\_)p(\_z)$
$bow(a\_)=1-Sum\_Z1 g(a\_z)$
## 二、平滑算法

有很多平滑算法，平滑算法默认都是使用回退的形式，部分算法使用-interpolate参数可以使用插值方式。

### （一）、Good-Turing

Good-Turing是ngram-count默认使用的算法，针对gram的出现次数分别处理。次数的限制使用参数-gtnmax控制，默认的1gram的次数大于1，其他gram的次数大于7认为是可信的。f(a_z)的计算方法如下： 

1. c(a_z) > gtmax 

概率的计算使用原始的最大似然估计： 
$f(a\_z)=\frac{c(a\_z)}{c(a\_)}$

2. 1 <= c(a_z) <= gtmax 

这种情况，认为次数是不可信的，需要对该次数做出修正，如下： 
$A=(gtmax+1)\frac{n[gtmax+1]}{n[1]}$
$c'(a\_z)=(c(a\_z)+1)\frac{n[c(a\_z)+1]}{n[c(a\_z)]}$
$f(a\_z)=\frac{c(a\_z)}{c(a\_)}\frac{(c'(a\_z)/c(a\_z)-A)}{(1-A)}$

上式中n[x]代表出现次数是x的gram的个数。 

该平滑算法不支持插值，只支持回退，所以最终的概率计算如下： 
$p(a\_z)=(c(a\_z)>0) ? f(a\_z):bow(a\_)p(\_z)$
$bow(a\_)=(1-Sum\_Z1 f(a\_z)) / (1-Sum\_Z1 f(\_z))$
### （二）、Kneser-Ney

该算法的思想是修改回退概率的计算方法。 

1. 对于回退方法来讲，公式修改为如下形式： 
$f(a\_z)=\frac{c(a\_z)-D0}{c(a\_)}$高阶gram 
$f(\_z)=\frac{n(*\_z)-D1}{n(*\_*)}$低阶gram 

其中，n(*_*)表示满足*_*格式的gram的种类数。 

2. 对于插值方法来讲，公式修改为如下形式： 
$g(a\_z)=max(0, c(a\_z)-D)/c(a\_)$高阶gram 
$bow(a\_)=1-Sum\_Z1 g(a\_z)=\frac{c(a\_)-Sum\_Z1c(a\_z)+Sum\_Z1D}{c(a\_)}$
$=\frac{Sum\_Z1D}{c(a\_)}=\frac{n(a\_*)D}{c(a\_)}$高阶gram 

假设Z2表示集合n(*_z)>0，那么可以得到低阶gram回退概率： 
$g(\_z)=max(0, n(*\_z)-D)/n(*\_*)$低阶gram 
$bow(\_)=1-Sum\_Z2 g(\_z)=\frac{n(*\_*)-Sum\_Z2n(*\_z)+Sum\_Z2D}{n(*\_*)}=\frac{Dn(\_*)}{n(*\_*)}$低阶gram 

3. D的计算方法 

原始的Knser-Ney方法(-ukndiscount)对于每一阶的ngram使用相同的常数D 
$D=\frac{n1}{n1+2*n2}$

n1表示出现一次的ngram的个数，n2表示出现两次的ngram的个数 

后来，Chen和Goodman修改了常数D的计算方法(-kndiscount)，针对每一阶的ngram，都有三个常数： 

D1-出现一次的ngram个数 

D2-出现两次的ngram个数 

D3-出现三次及以上的ngram个数 

计算如下： 
$Y=\frac{n1}{n1+2*n2}$
$D1=1-2Y\frac{n2}{n1}$
$D2=2-3Y\frac{n3}{n2}$
$D3=3-4Y\frac{n4}{n3}$
## 三、tricks
- 效果最好的modified Knser-Ney算法，使用插值方法，即-kndiscount -interpolate
- -gtnmin表示出现次数小于特定值的gram将会被丢弃，对于所有的平滑算法都一样。默认是所有的1元和2元都会保留，其他阶的count>=2的才会保留。
- -gtnmax表示次数小于特定值的count将会被修改，只是针对Good-Turing算法而言。









