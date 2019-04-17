# 重新定义程序员职业，Facebook推出代码推荐工具Aroma - 知乎
# 



> 乾明 发自 凹非寺 
量子位 出品 | 公众号 QbitAI

解放程序员，Facebook是认真的。

这一社交网络巨头在其官方博客上宣布，推出面向程序员的工具**Aroma**。

直击程序员想实现功能，而不知道如何写代码的痛点。

它能帮助程序员轻松地找到可以“参考使用”的代码，节省他们日常开发工作流程中的时间和精力。

比如，一个安卓程序员想要了解其他人如何解码手机中的bitmap，他可以使用下面的这些代码直接进行搜索：


```
Bitmap bitmap = BitmapFactory.decodeStream(input);
```


然后，Aroma会给出一些代码推荐：


```
final BitmapFactory.Options options = new BitmapFactory.Options();
options.inSampleSize = 2;
// ...
Bitmap bmp = BitmapFactory.decodeStream(is, null, options);
```


这段代码来自与五个实现类似功能的代码，但是去除了相关细节，以便程序员更好地参考。


除此之外，Aroma也能够集成在编码环境中。



![](https://pic2.zhimg.com/v2-853f1a418c92341d5002668360110de5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='450'></svg>)



Facebook表示，使用这一工具，程序员再也不用像原来那样，自己手动浏览几十个代码搜索结果了。


哪怕代码库非常大，它也能在几秒钟内给出推荐，并且不需要提前进行模式挖掘。

现在，Facebook已经将其用到了自己内部代码库中，并为其一些开源项目创建了Aroma版本。

这一工具亮相之后，便引发了巨大的关注。

甚至有人在Twitter上评论称，可以重新定义程序员这一职业。



![](https://pic4.zhimg.com/v2-5c56230455b37d22283c82865811bc53_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='977' height='293'></svg>)



Facebook是如何做到的？

## **给程序员推荐代码**



![](https://pic2.zhimg.com/v2-d95cb664325a9ba376967789742642f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='298'></svg>)



用Aroma给程序员推荐代码，主要分为3个阶段：

**第一，基于特征的搜索。**

首先，Aroma会解析语料库中的每个代码，并创建其解析树，并将代码集索引为稀疏矩阵。

然后，从每个代码的解析树中提取一组结构特征。这些结构特征都是经过精心选择的，可以捕捉每个代码的用法、调用方法和结构信息。

进而根据每个代码的特点，为其创建稀疏向量。

最后，所有代码的特征向量构成索引矩阵，用于搜索检索。



![](https://pic1.zhimg.com/v2-98e9b8cf398a557673cbdec259478abc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='383'></svg>)



当一个程序员编写新的代码时，Aroma会以上述的方式创建一个稀疏向量，并将该向量与包含所有现有代码的特征向量的矩阵相乘。


点积最高的前1000个代码会被检索出来，作为推荐代码的候选。

尽管代码语料库可以包含数百万种代码，但由于稀疏向量和矩阵的点积能够非常高效的实现，这种检索的速度会很快。



![](https://pic3.zhimg.com/v2-58066267dadcb5d17f52dab28f4a28c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='331'></svg>)



**第二，重新排序和聚类。**


通过检索得到推荐代码的候选集之后，接下来就是对其进行聚类。为了做到这一点，Aroma要先根据候选的各个代码和查询代码的相似性来对它们进行重新排序。

因为稀疏向量只包含关于存在哪些特征的抽象信息，所以点积分数低估了代码之间的实际相似性。

因此，Aroma对方法语法树进行修剪，去掉方法主体中不相关的部分，只保留与查询片段最匹配的部分，并根据候选代码片段与查询的实际相似性对其重新排序。

在获得了最后的排序列表之后，Aroma会运行一个迭代的聚类算法来寻找相似的代码片段，并在创建代码时推荐额外的代码语句。



![](https://pic2.zhimg.com/v2-65f48554978f13c91aee8d7f75e68205_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='341'></svg>)



**第三，用交叉算法创建代码进行推荐。**


交叉算法的工作原理，是将所有推荐代码中的第一个代码片段作为“基础”代码，然后与其余的代码进行对比，迭代地对其进行“修剪”。

比如下面的这几段代码，每一个都有特定于其项目的代码，但都包含公共的代码。

第一段代码：


```
InputStream is = ...;
final BitmapFactory.Options options = new BitmapFactory.Options();
options.inSampleSize = 2;
Bitmap bmp = BitmapFactory.decodeStream(is, null, options);
ImageView imageView = ...;
imageView.setImageBitmap(bmp);
// some more code
```


第二段代码：



```
BitmapFactory.Options options = new BitmapFactory.Options();
while (...) {
  in = ...;
  options.inSampleSize = 2;
  options.inJustDecodeBounds = false;
  bitmap = BitmapFactory.decodeStream(in, null, options);
}
```


基于上述的方法，Aroma会对第一段代码和第二段代码进行比较，来找到公共代码。第一段代中关于ImageView的代码，没有出现在第二段代码中，因此将被删除。结果如下所示：


```
InputStream is = ...;
final BitmapFactory.Options options = new BitmapFactory.Options();
options.inSampleSize = 2;
Bitmap bmp = BitmapFactory.decodeStream(is, null, options);
```


然后，将得到代码与第三段代码，第四段代码进行比较……


修剪之后剩下的代码，就是所有方法中常见的代码，它将成为最后被推荐的代码。

其他被推荐的代码，也是以这样的过程创建出来的。

此外，Aroma的算法也可以确保这些推荐彼此之间有很大的不同，因此程序员可以看到几个不同的代码，来学习各种各样的编程模式。

## **程序员们的担忧**

虽然这一工具受到了不少程序员的好评， 但也有一些程序员表达了不一样的观点。

有人在Hacker News上表示，这也许就是自己作为程序员职业生涯结束的开始。



![](https://pic1.zhimg.com/v2-17da7da5f29de1d59454f85a64dbb304_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='128'></svg>)



也有人说，这个工具似乎是试图强迫机器学习解决实际上没有人有问题的例子。

他说，很多人搜索习惯用法的主要用例，就是知道如何进行高级别的重构，而不是获得相似的代码。




![](https://pic3.zhimg.com/v2-57dd0cdfe10c824a67b15b00426513de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='463'></svg>)



此外，也有人担忧有人写错了代码，它会不会在其他代码库中大肆传播……




![](https://pic1.zhimg.com/v2-202d55651c96c606074b5675b6a51d50_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='186'></svg>)



你觉得呢？这一工具，对于程序员来说，影响几何？欢迎在评论区给出你的看法。


## **传送门**

Aroma论文链接：

Aroma: Code Recommendation via Structural Code Search
[https://arxiv.org/abs/1812.01158](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1812.01158)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


