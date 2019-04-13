
# 海量数据处理算法—Bloom Filter - bitcarmanlee的博客 - CSDN博客


2017年11月26日 00:00:08[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：450


## 1. Bloom-Filter算法简介
Bloom-Filter，即布隆过滤器，1970年由Bloom中提出。它可以用于检索一个元素是否在一个集合中。
Bloom Filter（BF）是一种空间效率很高的随机数据结构，它利用位数组很简洁地表示一个集合，并能判断一个元素是否属于这个集合。它是一个判断元素是否存在集合的快速的概率算法。Bloom Filter有可能会出现错误判断，但不会漏掉判断。也就是Bloom Filter判断元素不再集合，那肯定不在。如果判断元素存在集合中，有一定的概率判断错误。因此，Bloom Filter不适合那些“零错误”的应用场合。而在能容忍低错误率的应用场合下，Bloom Filter比其他常见的算法（如hash，折半查找）极大节省了空间。
它的优点是空间效率和查询时间都远远超过一般的算法，缺点是有一定的误识别率和删除困难。
## 2. Bloom-Filter的基本思想
Bloom-Filter算法的核心思想就是利用多个不同的Hash函数来解决“冲突”。
计算某元素x是否在一个集合中，首先能想到的方法就是将所有的已知元素保存起来构成一个集合R，然后用元素x跟这些R中的元素一一比较来判断是否存在于集合R中；我们可以采用链表等数据结构来实现。但是，随着集合R中元素的增加，其占用的内存将越来越大。试想，如果有几千万个不同网页需要下载，所需的内存将足以占用掉整个进程的内存地址空间。即使用MD5，UUID这些方法将URL转成固定的短小的字符串，内存占用也是相当巨大的。
于是，我们会想到用Hash table的数据结构，运用一个足够好的Hash函数将一个URL映射到二进制位数组（位图数组）中的某一位。如果该位已经被置为1，那么表示该URL已经存在。
Hash存在一个冲突（碰撞）的问题，用同一个Hash得到的两个URL的值有可能相同。为了减少冲突，我们可以多引入几个Hash，如果通过其中的一个Hash值我们得出某元素不在集合中，那么该元素肯定不在集合中。只有在所有的Hash函数告诉我们该元素在集合中时，才能确定该元素存在于集合中。这便是Bloom-Filter的基本思想。
原理要点：一是位数组， 而是k个独立hash函数。
1）位数组：
假设Bloom Filter使用一个m比特的数组来保存信息，初始状态时，Bloom Filter是一个包含m位的位数组，每一位都置为0，即BF整个数组的元素都设置为0。
![这里写图片描述](https://img-blog.csdn.net/20171125232708775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2）添加元素，k个独立hash函数
为了表达S={x1, x2,…,xn}这样一个n个元素的集合，Bloom Filter使用k个相互独立的哈希函数（Hash Function），它们分别将集合中的每个元素映射到{1,…,m}的范围中。
当我们往Bloom Filter中增加任意一个元素x时候，我们使用k个哈希函数得到k个哈希值，然后将数组中对应的比特位设置为1。即第i个哈希函数映射的位置hashi(x)就会被置为1（1≤i≤k）。
注意，如果一个位置多次被置为1，那么只有第一次会起作用，后面几次将没有任何效果。在下图中，k=3，且有两个哈希函数选中同一个位置（从左边数第五位，即第二个“1“处）。
![这里写图片描述](https://img-blog.csdn.net/20171125232934234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3）判断元素是否存在集合
在判断y是否属于这个集合时，我们只需要对y使用k个哈希函数得到k个哈希值，如果所有hashi(y)的位置都是1（1≤i≤k），即k个位置都被设置为1了，那么我们就认为y是集合中的元素，否则就认为y不是集合中的元素。下图中y1就不是集合中的元素（因为y1有一处指向了“0”位）。y2或者属于这个集合，或者刚好是一个false positive。
![这里写图片描述](https://img-blog.csdn.net/20171125233021860?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
显然这 个判断并不保证查找的结果是100%正确的。
Bloom Filter的缺点：
1）Bloom Filter无法从Bloom Filter集合中删除一个元素。因为该元素对应的位会牵动到其他的元素。所以一个简单的改进就是 counting Bloom filter，用一个counter数组代替位数组，就可以支持删除了。 此外，Bloom Filter的hash函数选择会影响算法的效果。
2）还有一个比较重要的问题，如何根据输入元素个数n，确定位数组m的大小及hash函数个数，即hash函数选择会影响算法的效果。当hash函数个数k=(ln2)*(m/n)时错误率最小。在错误率不大于E的情况 下，m至少要等于n*lg(1/E) 才能表示任意n个元素的集合。但m还应该更大些，因为还要保证bit数组里至少一半为0，则m应 该>=nlg(1/E)*lge ，大概就是nlg(1/E)1.44倍(lg表示以2为底的对数)。
举个例子我们假设错误率为0.01，则此时m应大概是n的13倍。这样k大概是8个。
注意：
这里m与n的单位不同，m是bit为单位，而n则是以元素个数为单位(准确的说是不同元素的个数)。通常单个元素的长度都是有很多bit的。所以使用bloom filter内存上通常都是节省的。
一般BF可以与一些key-value的数据库一起使用，来加快查询。由于BF所用的空间非常小，所有BF可以常驻内存。这样子的话，对于大部分不存在的元素，我们只需要访问内存中的BF就可以判断出来了，只有一小部分，我们需要访问在硬盘上的key-value数据库。从而大大地提高了效率。
一个Bloom Filter有以下参数：
m   bit数组的宽度（bit数）
n   加入其中的key的数量
k   使用的hash函数的个数
f   False Positive的比率
## 3.错误率估计
前面我们已经提到了，Bloom Filter在判断一个元素是否属于它表示的集合时会有一定的错误率（false positive rate），下面我们就来估计错误率的大小。在估计之前为了简化模型，我们假设`kn<m`且各个哈希函数是完全随机的。当集合S={x1, x2,…,xn}的所有元素都被k个哈希函数映射到m位的位数组中时，这个位数组中某一位还是0的概率是：
![这里写图片描述](https://img-blog.csdn.net/20171125233540724?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中1/m表示任意一个哈希函数选中这一位的概率（前提是哈希函数是完全随机的），(1-1/m)表示哈希一次没有选中这一位的概率。要把S完全映射到位数组中，需要做kn次哈希。某一位还是0意味着kn次哈希都没有选中它，因此这个概率就是（1-1/m）的kn次方。令p = e-kn/m是为了简化运算，这里用到了计算e时常用的近似：
![这里写图片描述](https://img-blog.csdn.net/20171125233756721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
令ρ为位数组中0的比例，则ρ的数学期望E(ρ)= p’。在ρ已知的情况下，要求的错误率（false positive rate）为：
![这里写图片描述](https://img-blog.csdn.net/20171125233832820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
(1-ρ)为位数组中1的比例，(1-ρ)k就表示k次哈希都刚好选中1的区域，即false positive rate。上式中第二步近似在前面已经提到了，现在来看第一步近似。p’只是ρ的数学期望，在实际中ρ的值有可能偏离它的数学期望值。M. Mitzenmacher已经证明[2] ，位数组中0的比例非常集中地分布在它的数学期望值的附近。因此，第一步的近似得以成立。分别将p和p’代入上式中，得：
![这里写图片描述](https://img-blog.csdn.net/20171125233921680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125233943994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
相比p’和f’，使用p和f通常在分析中更为方便。
## 4.最优的哈希函数个数
既然Bloom Filter要靠多个哈希函数将集合映射到位数组中，那么应该选择几个哈希函数才能使元素查询时的错误率降到最低呢？这里有两个互斥的理由：如果哈希函数的个数多，那么在对一个不属于集合的元素进行查询时得到1的概率就大；但另一方面，如果哈希函数的个数少，那么位数组中的0就多。为了得到最优的哈希函数个数，我们需要根据上一小节中的错误率公式进行计算。
先用p和f进行计算。注意到f = exp(k ln(1 − e−kn/m))，我们令g = k ln(1 − e−kn/m)，只要让g取到最小，f自然也取到最小。由于p = e-kn/m，我们可以将g写成
![这里写图片描述](https://img-blog.csdn.net/20171125234704318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据对称性法则可以很容易看出当p = 1/2，也就是k = ln2· (m/n)时，g取得最小值。在这种情况下，最小错误率f等于(1/2)k ≈ (0.6185)m/n。另外，注意到p是位数组中某一位仍是0的概率，所以p = 1/2对应着位数组中0和1各一半。换句话说，要想保持错误率低，最好让位数组有一半还空着。
需要强调的一点是，p = 1/2时错误率最小这个结果并不依赖于近似值p和f。同样对于f’ = exp(k ln(1 − (1 − 1/m)kn))，g’ = k ln(1 − (1 − 1/m)kn)，p’ = (1 − 1/m)kn，我们可以将g’写成
![这里写图片描述](https://img-blog.csdn.net/20171125234736568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
同样根据对称性法则可以得到当p’ = 1/2时，g’取得最小值。
## 5.参数如何取值
在实际使用BloomFilter时，一般会关注false positive probability，因为这和额外开销相关。实际的使用中，期望能给定一个false positive probability和将要插入的元素数量，能计算出分配多少的存储空间较合适。
假设BloomFilter中元素总bit数量为m，插入的元素个数为n，hash函数的个数为k，false positive probability记做p，它们之间有如下关系
如果需要最小化false positive probability，则k的取值如下
k = m * ln2 / n;
而p的取值，和m，n又有如下关系
m = - n * lnp / (ln2) ^ 2;
把公式一代入公式二，得出给定n和p，k的取值应该为
k = -lnp / ln2;
最后，也同样可以计算出m。
## 6.实际使用
最后，以一个例子具体描述使用BloomFilter的场景，以及在此场景下，BloomFilter的优势和劣势。
一组元素存在于磁盘中，数据量特别大，应用程序希望在元素不存在的时候尽量不读磁盘，此时，可以在内存中构建这些磁盘数据的BloomFilter，对于一次读数据的情况，分为以下几种情况：
1.请求的元素不在磁盘中，如果BloomFilter返回不存在，那么应用不需要走读盘逻辑，假设此概率为P1；如果BloomFilter返回可能存在，那么属于误判情况，假设此概率为P2
2.请求的元素在磁盘中，BloomFilter返回存在，假设此概率为P3
如果使用hashmap或者set的数据结构，情况如下：
1.请求的数据不在磁盘中，应用不走读盘逻辑，此概率为P1+P2
2.请求的元素在磁盘中，应用走读盘逻辑，此概率为P3
假设应用不读盘逻辑的开销为C1，走读盘逻辑的开销为C2，那么，BloomFilter和hashmap的开销为
```python
Cost(BloomFilter) = P1
```
```python
* C1 + (P2 + P3) *
```
```python
C2
Cost(HashMap) = (P1 + P2)
```
```python
* C1 + P3 *
```
```python
C2;
Delta = Cost(BloomFilter) - Cost(HashMap)
```
```python
= P2 * (C2 - C1)
```
因此，BloomFilter相当于以增加P2 * (C2 - C1)的时间开销，来获得相对于hashmap而言更少的空间开销。
参考链接：
1.[http://oserror.com/backend/bloomfilter/](http://oserror.com/backend/bloomfilter/)
2.[http://www.cnblogs.com/elric/archive/2010/09/17/1829512.html](http://www.cnblogs.com/elric/archive/2010/09/17/1829512.html)
3.[http://www.178linux.com/2751](http://www.178linux.com/2751)
4.[http://colobu.com/2016/07/02/bloom-filter-for-scala/](http://colobu.com/2016/07/02/bloom-filter-for-scala/)
5.[https://en.wikipedia.org/wiki/Bloom_filter](https://en.wikipedia.org/wiki/Bloom_filter)

