# 深入理解 hashcode 和 hash 算法 - Spark高级玩法 - CSDN博客
2018年11月25日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：179
**摘要**
- 
二进制计算的一些基础知识
- 
为什么使用 hashcode
- 
String 类型的 hashcode 方法
- 
为什么大部分 hashcode 方法使用 31
- 
HashMap 的 hash 算法的实现原理（为什么右移 16 位，为什么要使用 ^ 位异或）
- 
HashMap 为什么使用 & 与运算代替模运算？
- 
HashMap 的容量为什么建议是 2的幂次方？
- 
我们自定义 HashMap 容量最好是多少？
**前言**
作为一个有抱负的 Java 程序员，在经过长期的CRUD 和 HTML 填空之后必须有所思考，因为好奇心是驱动人类进步的动力之一，我们好奇，比如我们常用的 HashMap 到底是如何实现的？我想，说到这里，稍微有点经验的大佬都会说：擦，面试必问好嘛？怎么可能不知道？
但是，我们真的了解他吗？
我们知道 HashMap 依赖的 hashcode 和 hash 算法到底是怎么实现的嘛？如果大佬说：早他么知道了。那就装不知道，听楼主吹吹牛逼好不啦。。。。
今天楼主不会讲 HashMap 的 put 方法实现和 get 方法实现，楼主要讲的是 HashMap 高度依赖的 hashcode 和 hash 算法，虽然在很多书里面，都说这是数学家应该去研究的事情，但我想，程序员也应该了解他是怎么实现的。为什么这么做？就像娶老婆，你可能做不到创造老婆，但是你得知道你老婆是怎么来的？家是哪的？为什么喜欢你？扯远了，回来，那么今天我们就开始吧！
**1. 二进制计算的一些基础知识**
首先，因为今天的文章会涉及到一些位运算，因此楼主怕大家忘了（其实楼主自己也忘了），因此贴出一些位运算符号的意思，以免看代码的时候懵逼。
<< : 左移运算符，num << 1,相当于num乘以2  低位补0
>> : 右移运算符，num >> 1,相当于num除以2  高位补0
>>> : 无符号右移，忽略符号位，空位都以0补齐
 % : 模运算 取余
^ :   位异或 第一个操作数的的第n位于第二个操作数的第n位相反，那么结果的第n为也为1，否则为0
 & : 与运算 第一个操作数的的第n位于第二个操作数的第n位如果都是1，那么结果的第n为也为1，否则为0
 | :  或运算 第一个操作数的的第n位于第二个操作数的第n位 只要有一个是1，那么结果的第n为也为1，否则为0
 ~ : 非运算 操作数的第n位为1，那么结果的第n位为0，反之，也就是取反运算（一元操作符：只操作一个数）
好了，大概了解一下就好了，因为位运算平时在项目里真的用不上，在我们普通的业务项目里，代码易读性比这点位运算性能要重要的多。但是，在框架中，位运算的必要性就显示出来的了。因为需要服务大量的运算，性能要求也极高，如果性能渣渣，谁还用你？
**2. 为什么使用 hashcode**
那么我们就说说为什么使用 hashcode ，hashCode 存在的第一重要的原因就是在 HashMap(HashSet 其实就是HashMap) 中使用（其实Object 类的 hashCode 方法注释已经说明了 ），我知道，HashMap 之所以速度快，因为他使用的是散列表，根据 key 的 hashcode 值生成数组下标（通过内存地址直接查找，没有任何判断），时间复杂度完美情况下可以达到 n1（和数组相同，但是比数组用着爽多了，但是需要多出很多内存，相当于以空间换时间）。
**3. String 类型的 hashcode 方法**
在 JDK 中，Object 的 hashcode 方法是本地方法，也就是用 c 语言或 c++ 实现的，该方法直接返回对象的 内存地址。这么做会有说明问题呢？我们用代码看看：
classTest1{
  String name;
publicTest1(String name){
this.name = name;
  }
publicstaticvoidmain(String[] args){
    Map<Test1, String> map = new HashMap<>(4);
map.put(new Test1("hello"), "hello");
    String hello = map.get(new Test1("hello"));
    System.out.println(hello);
  }
}
这段代码打印出来的会是什么呢？
**答： null。**
从某个角度说，这两个对象是一样的，因为名称一样，name 属性都是 hello，当我们使用这个 key 时，按照逻辑，应该返回 hello 给我们。但是，由于没有重写 hashcode 方法，JDK 默认使用 Objective 类的 hashcode 方法，返回的是一个虚拟内存地址，而每个对象的虚拟地址都是不同的，所以，这个肯定不会返回 hello 。
如果我们重写 hashcode 和 equals 方法：
@Override
publicbooleanequals(Object o){
if (this == o) {
returntrue;
        }
if (o == null || getClass() != o.getClass()) {
returnfalse;
        }
        Test1 test1 = (Test1) o;
return Objects.equals(name, test1.name);
    }
@Override
publicinthashCode(){
return Objects.hash(name);
    }
再次运行：得到的结果就不是 null 了，而是 hello。
这才是比较符合逻辑，符合直觉的。
JDK 中，我们经常把 String 类型作为 key，那么 String 类型是如何重写 hashCode 方法的呢？
我们看看代码：
publicinthashCode() {
int h = hash;
if (h == 0 && value.length > 0) {
char val[] = value;
for (int i = 0; i < value.length; i++) {
                h = 31 * h + val[i];
            }
            hash = h;
        }
return h;
    }
**4. 为什么大部分 hashcode 方法使用 31**
如果有使用 eclipse 的同学肯定知道，该工具默认生成的 hashCode 方法实现也和 String 类型差不多。都是使用的 31 ，那么有没有想过：为什么要使用 31 呢？
在名著 《Effective Java》第 42 页就有对 hashCode 为什么采用 31 做了说明：
之所以使用 31， 是因为他是一个奇素数。如果乘数是偶数，并且乘法溢出的话，信息就会丢失，因为与2相乘等价于移位运算（低位补0）。使用素数的好处并不很明显，但是习惯上使用素数来计算散列结果。 31 有个很好的性能，即用移位和减法来代替乘法，可以得到更好的性能： 31 * i == (i << 5） - i， 现代的 VM 可以自动完成这种优化。这个公式可以很简单的推导出来。
可以看到，使用 31 最主要的还是为了性能。当然用 63 也可以。但是 63 的溢出风险就更大了。那么15 呢？仔细想想也可以。
在《Effective Java》也说道：编写这种散列函数是个研究课题，最好留给数学家和理论方面的计算机科学家来完成。我们此次最重要的是知道了为什么使用31。
**5. HashMap 的 hash 算法的实现原理（为什么右移 16 位，为什么要使用 ^ 位异或）**
好了，知道了 hashCode 的生成原理了，我们要看看今天的主角，hash 算法。
其实，这个也是数学的范畴，从我们的角度来讲，只要知道这是为了更好的均匀散列表的下标就好了，但是，就是耐不住好奇心啊！ 能多知道一点就是一点，我们来看看 HashMap 的 hash 算法（JDK 8）.
staticfinalinthash(Object key){
int h;
return (key == null) ? 0 : (h = key.hashCode()) ^ (h >>> 16);
    }
乍看一下就是简单的异或运算和右移运算，但是为什么要异或呢？为什么要移位呢？而且移位16？
在分析这个问题之前，我们需要先看看另一个事情，什么呢？就是 HashMap 如何根据 hash 值找到数组种的对象，我们看看 get 方法的代码：
final Node<K,V> getNode(int hash, Object key) {
        Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
if ((tab = table) != null && (n = tab.length) > 0 &&
// 我们需要关注下面这一行
            (first = tab[(n - 1) & hash]) != null) {
if (first.hash == hash && // always check first node
                ((k = first.key) == key || (key != null && key.equals(k))))
return first;
if ((e = first.next) != null) {
if (first instanceof TreeNode)
return ((TreeNode<K,V>)first).getTreeNode(hash, key);
do {
if (e.hash == hash &&
                        ((k = e.key) == key || (key != null && key.equals(k))))
return e;
                } while ((e = e.next) != null);
            }
        }
returnnull;
    }
我们看看代码中注释下方的一行代码：first = tab[(n - 1) & hash])。
使用数组长度减一 与运算 hash 值。这行代码就是为什么要让前面的 hash 方法移位并异或。
我们分析一下：
首先，假设有一种情况，对象 A 的 hashCode 为 1000010001110001000001111000000，对象 B 的 hashCode 为 0111011100111000101000010100000。
如果数组长度是16，也就是 15 与运算这两个数， 你会发现结果都是0。这样的散列结果太让人失望了。很明显不是一个好的散列算法。
但是如果我们将 hashCode 值右移 16 位，也就是取 int 类型的一半，刚好将该二进制数对半切开。并且使用位异或运算（如果两个数对应的位置相反，则结果为1，反之为0），这样的话，就能避免我们上面的情况的发生。
总的来说，使用位移 16 位和 异或 就是防止这种极端情况。但是，该方法在一些极端情况下还是有问题，比如：10000000000000000000000000 和 1000000000100000000000000 这两个数，如果数组长度是16，那么即使右移16位，在异或，hash 值还是会重复。但是为了性能，对这种极端情况，JDK 的作者选择了性能。毕竟这是少数情况，为了这种情况去增加 hash 时间，性价比不高。
**6. HashMap 为什么使用 & 与运算代替模运算？**
好了，知道了 hash 算法的实现原理还有他的一些取舍，我们再看看刚刚说的那个根据hash计算下标的方法：
tab[(n - 1) & hash]；
其中 n 是数组的长度。其实该算法的结果和模运算的结果是相同的。但是，对于现代的处理器来说，除法和求余数（模运算）是最慢的动作。
上面情况下和模运算相同呢？
a % b == (b-1) & a ,当b是2的指数时，等式成立。
我们说 & 与运算的定义：与运算 第一个操作数的的第n位于第二个操作数的第n位如果都是1，那么结果的第n为也为1，否则为0；
当 n 为 16 时， 与运算 101010100101001001101 时，也就是
1111 & 101010100101001001000 结果：1000 = 8
1111 & 101000101101001001001 结果：1001 = 9
1111 & 101010101101101001010 结果： 1010 = 10
1111 & 101100100111001101100 结果： 1100 = 12
可以看到，当 n 为 2 的幂次方的时候，减一之后就会得到 1111* 的数字，这个数字正好可以掩码。并且得到的结果取决于 hash 值。因为 hash 值是1，那么最终的结果也是1 ，hash 值是0，最终的结果也是0。
**7. HashMap 的容量为什么建议是 2的幂次方？**
到这里，我们提了一个关键的问题： HashMap 的容量为什么建议是 2的幂次方？正好可以和上面的话题接上。楼主就是这么设计的。
为什么要 2 的幂次方呢？
我们说，hash 算法的目的是为了让hash值均匀的分布在桶中（数组），那么，如何做到呢？试想一下，如果不使用 2 的幂次方作为数组的长度会怎么样？
假设我们的数组长度是10，还是上面的公式：
1010 & 101010100101001001000 结果：1000 = 8
1010 & 101000101101001001001 结果：1000 = 8
1010 & 101010101101101001010 结果： 1010 = 10
1010 & 101100100111001101100 结果： 1000 = 8
看到结果我们惊呆了，这种散列结果，会导致这些不同的key值全部进入到相同的插槽中，形成链表，性能急剧下降。
所以说，我们一定要保证 & 中的二进制位全为 1，才能最大限度的利用 hash 值，并更好的散列，只有全是1 ，才能有更多的散列结果。如果是 1010，有的散列结果是永远都不会出现的，比如 0111，0101，1111，1110…，只要 & 之前的数有 0， 对应的 1 肯定就不会出现（因为只有都是1才会为1）。大大限制了散列的范围。
**8. 我们自定义 HashMap 容量最好是多少？**
那我们如何自定义呢？自从有了阿里的规约插件，每次楼主都要初始化容量，如果我们预计我们的散列表中有2个数据，那么我就初始化容量为2嘛？
绝对不行，如果大家看过源码就会发现，如果Map中已有数据的容量达到了初始容量的 75%，那么散列表就会扩容，而扩容将会重新将所有的数据重新散列，性能损失严重，所以，我们可以必须要大于我们预计数据量的 1.34 倍，如果是2个数据的话，就需要初始化 2.68 个容量。当然这是开玩笑的，2.68 不可以，3 可不可以呢？肯定也是不可以的，我前面说了，如果不是2的幂次方，散列结果将会大大下降。导致出现大量链表。那么我可以将初始化容量设置为4。 当然了，如果你预计大概会插入 12 条数据的话，那么初始容量为16简直是完美，一点不浪费，而且也不会扩容。
**总结**
好了，分析完了 hashCode 和 hash 算法，让我们对 HashMap 又有了全新的认识。当然，HashMap 中还有很多有趣的东西值得挖掘，楼主会继续写下去。争取将 HashMap 的衣服扒光。
总的来说，通过今天的分析，对我们今后使用 HashMap 有了更多的把握，也能够排查一些问题，比如链表数很多，肯定是数组初始化长度不对，如果某个map很大，注意，肯定是事先没有定义好初始化长度，假设，某个Map存储了10000个数据，那么他会扩容到 20000，实际上，根本不用 20000，只需要 10000* 1.34= 13400 个，然后向上找到一个2 的幂次方，也就是 16384 初始容量足够。
原文：https://blog.csdn.net/qq_38182963/article/details/78940047 
推荐阅读：
[海量数据处理之BloomFilter](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485970&idx=1&sn=37fe6a9d46625ad06ba58a0e6ad44d1b&chksm=9f38e93aa84f602c7bd7c9078fa3877bdc17b2b37b366aa9a33e30ad2d6941f6731aef490f9c&scene=21#wechat_redirect)
[怎样写出高性能的 Java 代码？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485405&idx=1&sn=8e4eb9cb71c9d410ba3a899fe9aa1e04&chksm=9f38e4f5a84f6de319577ea694c0062660a4cd41822631888a2c069f9f156ab6bfb9c0c66597&scene=21#wechat_redirect)
[JAVA中序列化和反序列化中的静态成员问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485102&idx=1&sn=5092b0dd4236d43a98eda2c3d03916ec&chksm=9f38e586a84f6c90a25072672a8eafad0f171d94a49840cc66e58e0e28eeeb0906cf131d33c0&scene=21#wechat_redirect)
[Java中定义常量(Constant) 的几种方法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485129&idx=1&sn=93599a0b4e954abaed42e7f8e21425cf&chksm=9f38e5e1a84f6cf792695f5a8277e3fa7cd84eda73aad87fa6f65c0ea67f58cebc4b3313f7b9&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎点赞并转发给小伙伴。
