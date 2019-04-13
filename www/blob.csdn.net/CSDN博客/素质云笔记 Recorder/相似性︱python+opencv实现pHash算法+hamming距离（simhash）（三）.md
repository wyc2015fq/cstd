
# 相似性︱python+opencv实现pHash算法+hamming距离（simhash）（三） - 素质云笔记-Recorder... - CSDN博客

2017年04月20日 19:03:05[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：9513


pHash跟simhash很多相近的地方。一个是较多用于图像，一个较多用于文本。
---之前写关于R语言实现的博客：

---[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（一，基本原理）](http://blog.csdn.net/sinat_26917383/article/details/52451028)

---[R语言实现︱局部敏感哈希算法（LSH）解决文本机械相似性的问题（二，textreuse介绍）](http://blog.csdn.net/sinat_26917383/article/details/52457252)

---机械相似性python版的四部曲：

---[LSH︱python实现局部敏感随机投影森林——LSHForest/sklearn（一）](http://blog.csdn.net/sinat_26917383/article/details/70243066)

---[LSH︱python实现局部敏感哈希——LSHash（二）](http://blog.csdn.net/sinat_26917383/article/details/70285678)

---[相似性︱python+opencv实现pHash算法+hamming距离（simhash）（三）](http://blog.csdn.net/sinat_26917383/article/details/70287521)

---[LSH︱python实现MinHash-LSH及MinHash LSH Forest——datasketch（四）](http://blog.csdn.net/sinat_26917383/article/details/70332325)

---一、pHash跟simhash

---1、simhash

---可参考：

---[Python基础教程-python实现simhash算法实例详细介绍](http://www.osetc.com/archives/15143.html)

---Simhash的算法简单的来说就是，从海量文本中快速搜索和已知simhash相差小于k位的simhash集合，这里每个文本都可以用一个simhash值来代表，一个simhash有64bit，相似的文本，64bit也相似，论文中k的经验值为3。

---该方法的缺点如优点一样明显，主要有两点，

---对于短文本，k值很敏感；

---另一个是由于算法是以空间换时间，系统内存吃不消。

![这里写图片描述](https://img-blog.csdn.net/20170420185113657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
---[ ](https://img-blog.csdn.net/20170420185113657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---.

---[

](https://img-blog.csdn.net/20170420185113657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---2、感知哈希算法(pHash)

---[
](https://img-blog.csdn.net/20170420185113657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

---节选自：

---[图像检索︱图像的相似性搜索与图像向量化、哈希化（文献、方法描述）](http://blog.csdn.net/sinat_26917383/article/details/63306206)

---平均哈希算法过于严格，不够精确，更适合搜索缩略图，为了获得更精确的结果可以选择感知哈希算法，它采用的是DCT（离散余弦变换）来降低频率的方法

---一般步骤：

---缩小图片：32 * 32是一个较好的大小，这样方便DCT计算

---转化为灰度图：把缩放后的图片转化为256阶的灰度图。（具体算法见平均哈希算法步骤）

---计算DCT:DCT把图片分离成分率的集合

---缩小DCT：DCT计算后的矩阵是32 * 32，保留左上角的8 * 8，这些代表的图片的最低频率

---计算平均值：计算缩小DCT后的所有像素点的平均值。

---进一步减小DCT：大于平均值记录为1，反之记录为0.

---得到信息指纹：组合64个信息位，顺序随意保持一致性。

---最后比对两张图片的指纹，获得汉明距离即可。

---这等同于”汉明距离”(Hamming distance,在信息论中，两个等长字符串之间的汉明距离是两个字符串对应位置的不同字符的个数)。如果不相同的数据位数不超过5，就说明两张图像很相似；如果大于10，就说明这是两张不同的图像。

---.

---二、pHash算法python+opencv实现

---参考自：

---[opencv resize （C/C++/Python）](http://blog.csdn.net/u012005313/article/details/51943442)

---主要针对图像来进行解析。下面的是pHash算法的主函数：

---import

---cv2

---import

---numpy

---as

---np

---from

---compiler.ast

---import

---flatten

---import

---sys

---def

---pHash

---(imgfile)

---:

---"""get image pHash value"""

---\#加载并调整图片为32x32灰度图片

---img=cv2.imread(imgfile,

---0

---) 
    img=cv2.resize(img,(

---64

---,

---64

---),interpolation=cv2.INTER_CUBIC)

---\#创建二维列表

---h, w = img.shape[:

---2

---]
    vis0 = np.zeros((h,w), np.float32)
    vis0[:h,:w] = img

---\#填充数据

---\#二维Dct变换

---vis1 = cv2.dct(cv2.dct(vis0))

---\#cv.SaveImage('a.jpg',cv.fromarray(vis0)) \#保存图片

---vis1.resize(

---32

---,

---32

---)

---\#把二维list变成一维list

---img_list=flatten(vis1.tolist())

---\#计算均值

---avg = sum(img_list)*

---1.

---/len(img_list)
    avg_list = [

---'0'

---if

---i<avg

---else

---'1'

---for

---i

---in

---img_list]

---\#得到哈希值

---return

---''

---.join([

---'%x'

---% int(

---''

---.join(avg_list[x:x+

---4

---]),

---2

---)

---for

---x

---in

---range(

---0

---,

---32

---*

---32

---,

---4

---)])

---'''
cv2.imread
flags>0时表示以彩色方式读入图片 
flags=0时表示以灰度图方式读入图片 
flags<0时表示以图片的本来的格式读入图片
interpolation - 插值方法。共有5种：
１）INTER_NEAREST - 最近邻插值法
２）INTER_LINEAR - 双线性插值法（默认）
３）INTER_AREA - 基于局部像素的重采样（resampling using pixel area relation）。对于图像抽取（image decimation）来说，这可能是一个更好的方法。但如果是放大图像时，它和最近邻法的效果类似。
４）INTER_CUBIC - 基于4x4像素邻域的3次插值法
５）INTER_LANCZOS4 - 基于8x8像素邻域的Lanczos插值
http://blog.csdn.net/u012005313/article/details/51943442
'''

---其中需要关注的是这段代码：

---''

---.

---join

---(avg_list[x:x+

---4

---]),

---2

---)

---for

---x

---in

---range(

---0

---,

---32

---*

---32

---,

---4

---)]

---把数字变为字符型，然后进行对比。

---001.jpg：7ffc0000ffffe000

---002.jpg：7fff0000fffff800

---.

---得到哈希值之后，需要求距离，这里较多使用

---海明距离

---（

---[来源](http://sm4llb0y.blog.163.com/blog/static/1891239720099195041879/)

---）。

---这等同于”汉明距离”(Hamming distance,在信息论中，两个等长字符串之间的汉明距离是两个字符串对应位置的不同字符的个数)。如果不相同的数据位数不超过5，就说明两张图像很相似；如果大于10，就说明这是两张不同的图像。

---def

---hammingDist

---(s1, s2)

---:

---assert

---len(s1) == len(s2)

---return

---sum([ch1 != ch2

---for

---ch1, ch2

---in

---zip(s1, s2)])

---那么有了主函数，有了海明距离，就可以简单地实现：

---HASH1=pHash(

---'C:\\001.png'

---)
HASH2=pHash(

---'C:\\002.png'

---)
out_score =

---1

---- hammingDist(HASH1,HASH2)

---*1

---. / (

---32

---*32

---/

---4

---)

---先导入，然后哈希化。同时，得到海明相似性。

---.

---三、海量数据查找经验

---本节来源于：

---[海量数据相似度计算之simhash短文本查找](http://www.lanceyan.com/tech/arch/simhash_hamming_distance_similarity2-html.html)

---simhash的数据也会暴增，如果一天100w，10天就1000w了。

---我们如果插入一条数据就要去比较1000w次的simhash，计算量还是蛮大，普通PC 比较1000w次海明距离需要 300ms ，和5000w数据比较需要1.8 s。看起来相似度计算不是很慢，还在秒级别。

---原来是5000w次顺序比较，现在是少了2的16次方比较，前面16位变成了hash查找。后面的顺序比较的个数是多少？ 2^16 = 65536， 5000w/65536 = 763 次。。。。实际最后链表比较的数据也才 763次！所以效率大大提高！

![这里写图片描述](http://www.lanceyan.com/wp-content/uploads/2013/09/simhash3.png)
---[ ](http://www.lanceyan.com/wp-content/uploads/2013/09/simhash3.png)

---到目前第一点降到3.6毫秒、支持5000w数据相似度比较做完了。还有第二点同一时刻发出的文本如果重复也只能保留一条和短文本相识度比较怎么解决。其实上面的问题解决了，这两个就不是什么问题了。

---[
](http://www.lanceyan.com/wp-content/uploads/2013/09/simhash3.png)

---之前的评估一直都是按照线性计算来估计的，就算有多线程提交相似度计算比较，我们提供相似度计算服务器也需要线性计算。比如同时客户端发送过来两条需要比较相似度的请求，在服务器这边都进行了一个排队处理，一个接着一个，第一个处理完了在处理第二个，等到第一个处理完了也就加入了simhash库。所以只要服务端加了队列，就不存在同时请求不能判断的情况。

---simhash如何处理短文本？换一种思路，simhash可以作为局部敏感哈希第一次计算缩小整个比较的范围，等到我们只有比较700多次比较时，就算使用我们之前精准度高计算很慢的编辑距离也可以搞定。当然如果觉得慢了，也可以使用余弦夹角等效率稍微高点的相似度算法。

---[            ](http://www.lanceyan.com/wp-content/uploads/2013/09/simhash3.png)


