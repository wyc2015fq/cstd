# 运动目标跟踪（十二）--KCF跟踪及CSK,CN对比 - 工作笔记 - CSDN博客





2016年08月17日 09:27:53[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：13519
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









作者的主页：http://home.isr.uc.pt/~henriques/


可以下载到文章和代码，文章的名字叫　High-Speed Tracking with　Kernelized Correlation Filters.

发展历史轨迹为CSK=>>KCF/DCF/CN.



#### **CSK:[paper:Exploiting the Circulant Structure of Tracking-by-detection with Kernels（作者和KCF/DCF同一个作者）]**
- **文章特点：**- 输入：整个候选search区域的raw pixel（作为特征,并不是每个候选框），label(y_i),label为符合Gauss分布的连续取值 
- 目的：训练一个分类器，学习分类器的权重W。 

- 求解分类器的权重W,探究了subimage_window和circular struct以及kernel的关系，利用这个关系引入kernel Trick. 

- 利用cirlular matrix和来求解分类器的权重，利用FFT以及循环矩阵的性质，避免了求W时的矩阵逆运算。    

- 输出：相对平移量，实验选区响应值最大的位置作为目标移动的大小。
- **缺点：**- scale问题。    
- 循环矩阵bounding效应（SRDCF解决）。    

- 输入为raw gray pixel（KCF,CN，DeepSRDCF丰富了特征的选取）. 

- **details**- **首先**，候选框subimage_window存在很多重合，计算特征导致冗余。所以为了满足一定的速度要求，无法Dense sampling，只能random sampling少许，导致结果不好。    
- **发现**，所有subimage_window可以有候选区域和循环矩阵来表示。设候选区域的特征连接为vector:**V,**循环矩阵为如下：




![](https://img-blog.csdn.net/20160510165053123?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
> 
all_subimage_window = C(**u**)**V，**其实就是一个kernel变换，从候选区域中得到子区域。其实C(**u**_i),为（1,1,0. ...）等。其实这里作者只是以此说明这个关系，具体在代码中使用时却没那么复杂。



> **有了上面的发现**，则回到tracking问题，跟踪其实就是训练一个分类器:

![](https://img-blog.csdn.net/20160510165908412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
 其中w为分类器的系数，需要学习。引入kernel trick（get the subimage_window）：


![](https://img-blog.csdn.net/20160510170204585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

> 
如果式（1）中的loss function：L取二范数距离，则求解的结果为：![](https://img-blog.csdn.net/20160510170308554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


> 
进而转化为求a_i，公式如下：

![](https://img-blog.csdn.net/20160805094859631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中最为关键的是求K为循环矩阵，而选择好对应的核变换函数（线性核，高斯核，多项式核）即可求K，从而得到a_i,及分类器的权重。利用FFT变换，将卷积变换为频域的dot-product，加快速度。
- **核函数的选举有多种**，具体看原paper.所以通过这一步一步从而求解出分类器。

#### **KCD/DCF[paper:High-Speed Tracking with Kernelized Correlation Filters]，和CSK是同一个作者**

这篇文章核心算法同CSK，只是从特征和多尺度以及核变换进行了改进。
- **在CSK的基础上解决了如下几个问题：**- 输入为multi-channels(可以是彩色，可以是Hog)，并定义了multi-channel特征的连接方法。    

- 采用不同函数，Gauss核函数，paper叫KCF，采用linear kernel时，paper取名叫DCF,其中DCF由于采用的linear-kernel,所以multi-channel合并时有优势，速度比KCF快，效果差一点点。

- **detail**

> 
多通道特征连接，由于卷积在频域是dot-product的求和，所以将不同channels的特征vector连接在一起为一个vector即可。multi-channel特征可以是彩色，也可以时Hog和及其方向的不同channel.如果时Gauss核（KCF）则核函数计算如paper中式（31），如果时线性核（DCF）则根据式（32）计算。paper中分析了速度方面的影响，linear-kernel的DCF更简单，所以速度更快。





#### **CN[paper:Adaptive Color Attributes for Real-Time Visual Tracking]**
- 在CSK的基础上，将输入变为11个颜色空间，具体略

#### **SRDCF**
- 在kcf上解决scale[多尺度搜索]和bounding effect[加入惩罚项]


#### **DeepSRDCF**
- 在SRDCF基础上 用CNN来提取特征[CNN第一层输出作为特征]

总结来说，这也是一种Tracking By Detection的跟踪方法，跟ＴＬＤ，OAB同宗,以跟踪对象为正样本，以周围坏境为负样本，训练一个判别分类器．paper的实验结果显示，kcF（Kernelized Correlation Filter）准确率比**Struck**和**TLD**都高(paper中提到With HOG features, both the linear DCF　and non-linear KCF outperform by
 a large margin top-ranking trackers, such as Struck [7] or Track-Learn-Detect(TLD) [4], while comfortably running at hundreds of frames-per-second.)。之所以能有这么快的速度，得益于作者巧妙地通过循环偏移构建出了分类器的训练样本，从而使得数据矩阵变成了一个循环矩阵。然后基于循环矩阵的特性把问题的求解变换到了离散傅里叶变换域，从而避免了矩阵求逆的过程，降低了好几个数量级的算法复杂度.

  ４．１　样本训练过程实际上是一个岭回归问题，或者叫做正则化最小二乘问题，即

![](https://img-blog.csdn.net/20151229160226177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

公式１实际就是正则化最小二乘回归，训练的最终目的就是为寻找一组权值Ｗ，得到公式２

![](https://img-blog.csdn.net/20151229160528447?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

X,y就是对应的样本和类别．因为考虑到后面要在傅里叶域进行计算，这里来转化一下，给出一个复数情况下的求解结果，其中 
 是 
 的共轭转置， 
 是 
 的共轭。

![](https://img-blog.csdn.net/20151229160717653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)   （3）

随着样本数目的增加，直接求解上式时非常耗时的（因为有求逆计算）．这篇作者的贡献就是非常高效的避开了求逆运算，接着看．

（样本循环着移来移去利用了循环矩阵的性质，使得只需要计算循环矩阵的第一行就可以达到计算循环整个矩阵的效果．）


４．２　Cyclic shifts（循环偏移）＊＊＊

式 
 中的样本矩阵 
 如果是一个循环矩阵的话，该式子的计算就会变得容易很多。即，





其中， 
 是矩阵的第一行，整个矩阵式由这一行的循环偏移得到的。那我们假设存在这么一个循环矩阵，看看接下来式 
 会变成怎样。首先列出一个循环矩阵拥有的一个性质[5](http://www.skyoung.org/kcf-tracking-method/#easy-footnote-bottom-5)如下：





其中， 
 头上的那个小帽 
 代表 
 的傅里叶变换， 
 是离散傅里叶变换矩阵，即满足 
 。这样把式 
 代入式 
 中得，





其中， 
 代表向量对应元素相乘，然后两边同时左乘 
 得，





至此，我们可以看出通过上述变换后，权重向量 
 的求解变换到了傅里叶变换域，而且计算量大大降低。

以上介绍的都是线性回归的情况，如果能引入核函数，分类器的性能将会更好。核函数的引入是把特征空间映射到一个更高维的空间去，这里我们假设这个映射函数为 
 ，则分类器的权重向量变为，





这样我们最终要求解的参数就由 
 变为 
 ，这里 
 。因为其实我们并不知道核函数映射的高维空间是什么，我们只是知道高维空间下的两个向量的乘积可以通过一个映射函数把其在低维空间下的乘积映射到高维空间，也就是核函数。这里设不同样本之间的乘积的核函数结果组成的矩阵为





这样最终的回归函数变为，





直接计算上述函数相对来说是很耗时的，下面还是结合循环矩阵的特性实现一种快速的核函数计算方法。

## 快速训练

基于核函数下的岭回归的解为[6](http://www.skyoung.org/kcf-tracking-method/#easy-footnote-bottom-6)，





其中， 
 核函数矩阵，如式 
 所示。如果我们能够证明 
 是循环矩阵，则上式的求解就可以转换到DFT域，即，





这里， 
 是核函数矩阵 
 的第一行元素组成的向量。

如果两个向量的元素的次序发生变化不影响最终通过核函数计算的结果，则该核函数构成的矩阵就是一个循环矩阵，像高斯核函数，多项式核函数都是满足上面条件的。

## 快速检测

上面已经提到直接计算式 
 是非常耗时的，快速的解法是像式 
 那样，通过构建测试样本和训练样本的核函数矩阵如下，





其中， 
 是这个循环矩阵的第一行组成的向量。这样就可以同时计算基于测试样本 
 的循环偏移构成的所有测试样本的响应，即，





注意这里 
 不同于式 
 ，它是一个向量，由基于base样本 
 不同循环偏移下的响应值组成。根据循环矩阵的性质（如式 
 所示），上式变换到DFT域后，





## 快速计算核函数相关性

到现在为止，只剩下前面部分提到的 
 和 
 没有阐明如何计算了。首先列出 
 的计算公式如下，





其中， 
 是核函数， 
 是基于 
 为第一行的循环矩阵。参考式 
 所示循环矩阵的特性，代入上式得，





所以，对于多项式核函数，其计算公式如下，





对于高斯核函数，其计算公式如下，






附上伪代码，感觉这伪代码逻辑特别清晰






**[cpp]**[view plain](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[copy](http://blog.csdn.net/zwlq1314521/article/details/50427038#)

[print](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[?](http://blog.csdn.net/zwlq1314521/article/details/50427038#)

- Inputs  
- • x: training image patch, m × n × c  
- • y: regression target, Gaussian-shaped, m × n  
- • z: test image patch, m × n × c  
- Output  
- • responses: detection score for each location, m × n  
- function alphaf = train(x, y, sigma, lambda)  
- k = kernel_correlation(x, x, sigma);  
- alphaf = fft2(y) ./ (fft2(k) + lambda);  
- end  
- function responses = detect(alphaf, x, z, sigma)  
- k = kernel_correlation(z, x, sigma);  
- responses = real(ifft2(alphaf .* fft2(k)));  
- end  
- function k = kernel_correlation(x1, x2, sigma)  
- c = ifft2(sum(conj(fft2(x1)) .* fft2(x2), 3));  
- d = x1(:)’*x1(:) + x2(:)’*x2(:) - 2 * c;  
- k = exp(-1 / sigma^2 * abs(d) / numel(d));  
- end  

![](http://static.blog.csdn.net/images/save_snippets.png)

```cpp
Inputs
• x: training image patch, m × n × c
• y: regression target, Gaussian-shaped, m × n
• z: test image patch, m × n × c
Output
• responses: detection score for each location, m × n
function alphaf = train(x, y, sigma, lambda)
k = kernel_correlation(x, x, sigma);
alphaf = fft2(y) ./ (fft2(k) + lambda);
end
function responses = detect(alphaf, x, z, sigma)
k = kernel_correlation(z, x, sigma);
responses = real(ifft2(alphaf .* fft2(k)));
end
function k = kernel_correlation(x1, x2, sigma)
c = ifft2(sum(conj(fft2(x1)) .* fft2(x2), 3));
d = x1(:)’*x1(:) + x2(:)’*x2(:) - 2 * c;
k = exp(-1 / sigma^2 * abs(d) / numel(d));
end
```





[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)[](http://blog.csdn.net/zwlq1314521/article/details/50427038#)










