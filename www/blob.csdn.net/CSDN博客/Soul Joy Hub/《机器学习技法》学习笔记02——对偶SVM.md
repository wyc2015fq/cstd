# 《机器学习技法》学习笔记02——对偶SVM - Soul Joy Hub - CSDN博客

2017年08月02日 12:26:56[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：499
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/76574969](http://blog.csdn.net/u011239443/article/details/76574969)

# 对偶SVM的目标

如果是非线性SVM，那么问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-3c6c64b79a1de670.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

$z_n是x_n在d+1$高维空间映射所得到的值，于是就出现了困境：

![](http://upload-images.jianshu.io/upload_images/1621805-e6fe64643d24892a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

对偶SVM的目标就是：

![](http://upload-images.jianshu.io/upload_images/1621805-0e63fdb40ccab01c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们由拉格朗日乘子法得：

![](http://upload-images.jianshu.io/upload_images/1621805-fb1c1d529ba5990d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

因为$y_n(w^Tz_n+b)>=1$

所以$1-y_n(w^Tz_n+b)<=0$

为了让符号不变，我们规定$α_n >=0 $， 

则$α_n（1-y_n(w^Tz_n+b)<=0）$

则$1/2w^Tw+α_n(1-y_n(w^Tz_n+b)<=0)<=1/2w^Tw$

则$max(1/2w^Tw+α_n（1-y_n(w^Tz_n+b)<=0)) 约等于 1/2w^Tw$

所以我们的问题就变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-a50586711425598d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

下面式子中方括号代表$1/2w^Tw$，如果超平面分割预测和真实点的函数值$y_n(w^Tz_n+b)$有误or正确却在间隔带内，则$y_n(w^Tz_n+b) < 1$，则$1-y_n(w^Tz_n+b) > 0 $,则max(L(b,w,α))趋于无穷。 
$y_n(w^Tz_n+b)$正确且在间隔带外（包含间隔带边界），则$y_n(w^Tz_n+b) >= 1$，则$1-y_n(w^Tz_n+b) <= 0 $,则$max(L(b,w,α))=1/2w^Tw$
![](http://upload-images.jianshu.io/upload_images/1621805-9a3051d03b9d81b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

于是我们的问题就变成：

![](http://upload-images.jianshu.io/upload_images/1621805-6dc019969be20e1c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 拉格朗日对偶SVM

上式问题并不好解。我们有：

![](http://upload-images.jianshu.io/upload_images/1621805-77cf169fbae3804d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于上式右边的最大值还是要小于等于左边式子，于是我们就得到了拉格朗日对偶问题：

![](http://upload-images.jianshu.io/upload_images/1621805-5afde379372ff334.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

当上式符合约束规格时等号就成立。约束规格： 

1.是凸优化 

2.存在解 

3.约束条件是线性的

这里符合约束规格，于是我们的问题变成了：

![](http://upload-images.jianshu.io/upload_images/1621805-c55f419f582fdef7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这样括号里面就成了只是关于b和w的问题，我们可以先求括号里面。对L关于b求导：

![](http://upload-images.jianshu.io/upload_images/1621805-02763f6b56301040.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

把它代入问题中，就消去了b：

![](http://upload-images.jianshu.io/upload_images/1621805-7d792f0fb4a00480.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

再对L关于w求导：

![](http://upload-images.jianshu.io/upload_images/1621805-91ede0fc1fa19f11.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-57cbe7f666af4232.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

把它代入问题得到：

![](http://upload-images.jianshu.io/upload_images/1621805-04c7606dbc8972ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

该问题最优化需要符合KKT条件： 

1.原问题约束：

![](http://upload-images.jianshu.io/upload_images/1621805-077226fbaede97d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

2.对偶问题约束：

![](http://upload-images.jianshu.io/upload_images/1621805-c57db1b7690da43e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

3.原问题的最优化条件：

![](http://upload-images.jianshu.io/upload_images/1621805-e95d85b64d62081c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

4.对偶问题的最优化条件：

![](http://upload-images.jianshu.io/upload_images/1621805-f70c34180e9a001f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 求解对偶SVM

对问题乘以-1，得到最小化问题：

![](http://upload-images.jianshu.io/upload_images/1621805-87485746d96b0843.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

当我们用KKT条件求解出二次规划最优解$α_n$之后，我们如何求解w和b呢？ 

w很简单，就用对偶问题的最优化条件能求出来。  

求解b，由原问题约束、对偶问题约束和原问题的最优化条件可知:

![](http://upload-images.jianshu.io/upload_images/1621805-f9d62813510b126e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 对偶问题背后的意义

我们之前说过，“寻找与超平面最近的点”，所以除边界上的点外，其他点对优化没有意义。 

我们称$α_n>0$ 的$(z_n ,y_n )$为支持向量：

![](http://upload-images.jianshu.io/upload_images/1621805-d7f0ad38dcc20f83.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们也可以看到，其实也只有边界上的支持向量才会代入计算：

![](http://upload-images.jianshu.io/upload_images/1621805-2ee02721927d9b04.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从另外一个角度看，无论是SVM 还是 PLA，w都是$y_nz_n$的组合，可以看成是由数据表示出来的：

![](http://upload-images.jianshu.io/upload_images/1621805-5ef09de0a365a365.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们来回顾下对偶SVM的目标：

![](http://upload-images.jianshu.io/upload_images/1621805-0e63fdb40ccab01c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们已经基本上达成这个目标：

![](http://upload-images.jianshu.io/upload_images/1621805-239cd976e4089678.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

但是我们还留有一个问题，$Q_D$中：

![](http://upload-images.jianshu.io/upload_images/1621805-18c1b3c490b28a99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

所以搞了半天，依旧存在z，即依旧存在x到d+1高维空间的映射，d依旧可能非常大甚至趋于无穷。这该如何是好呢，请听下回分解~

![这里写图片描述](https://img-blog.csdn.net/20170802165831106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

