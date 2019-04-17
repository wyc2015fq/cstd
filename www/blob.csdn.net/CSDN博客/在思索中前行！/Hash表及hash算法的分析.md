# Hash表及hash算法的分析 - 在思索中前行！ - CSDN博客





2014年10月17日 12:57:27[_Tham](https://me.csdn.net/txl16211)阅读数：753








**Hash表中的一些原理/概念，及根据这些原理/概念:**

**一.       Hash表概念**

**二.       Hash构造函数的方法，及适用范围**

**三.       Hash处理冲突方法，各自特征**

**四.       Hash查找过程**

**五.       实现一个使用Hash存数据的场景-------Hash查找算法，插入算法**

**六.       JDK中HashMap的实现**

**七.       Hash表与HashMap的对比，性能分析**




 结构之法，算法之道 ：[从头到尾彻底解析Hash表算法](http://blog.csdn.net/v_july_v/article/details/6256463)




## **一.    Hash表概念 **


               在查找表中我们已经说过，在Hash表中，**记录在表中的位置和其关键字之间存在着一种确定的关系**。这样       我们就能预先知道所查关键字在表中的位置，从而直接通过下标找到记录。使ASL趋近与0.




              1) 哈希(Hash)函数是一个映象，即： 将关键字的集合映射到某个地址集合上，它的设置很灵活，只要这个地址集合的大小不超出允许范围即可；


              2)  由于哈希函数是一个压缩映象，因此，在一般情况下，很容易产生“冲突”现象，即： key1¹ key2，而  f (key1) = f(key2)。


              3).  只能尽量减少冲突而不能完全避免冲突，这是因为通常关键字集合比较大，其元素包括所有可能的关键字，而地址集合的元素仅为哈希表中的地址值


            在构造这种特殊的“查找表” 时，除了需要选择一个**“好”(尽可能少产生冲突)**的哈希函数之外；还需要找到一种**“处理冲突”** 的方法。



## **二 .     Hash构造函数的方法，及适用范围**
- **直接定址法**
- **数字分析法**
- **平方取中法**
- **折叠法**
- **除留余数法**
- **随机数法**


      （1）直接定址法：


                哈希函数为关键字的线性函数，H(key) = key 或者 H(key) = a ´ key + b

**此法仅适合于**：地址集合的大小 = = 关键字集合的大小，其中a和b为常数。


     （2）数字分析法：


             假设关键字集合中的每个关键字都是由 s 位数字组成 (u1, u2, …, us)，分析关键字集中的全体， 并从中提取分布均匀的若干位或它们的组合作为地址。

**此法适于:**能预先估计出全体关键字的每一位上各种数字出现的频度。


     （3）平方取中法：


               以关键字的平方值的中间几位作为存储地址。求“关键字的平方值” 的目的是“扩大差别” ，同时平方值的中间各位又能受到整个关键字中各位的影响。

**此法适于:**关键字中的每一位都有某些数字重复出现频度很高的现象。


     （4）折叠法：


            将关键字分割成若干部分，然后取它们的叠加和为哈希地址。两种叠加处理的方法：移位叠加:将分割后的几部分低位对齐相加；间界叠加:从一端沿分割界来回折叠，然后对齐相加。

**此法适于：**关键字的数字位数特别多。


     （5）除留余数法：


             设定哈希函数为:H(key) = key MOD p   ( p≤m )，其中， m为表长，p 为不大于 m 的素数，或是不含 20 以下的质因子


     （6）随机数法：


           设定哈希函数为:H(key) = Random(key)其中，Random 为伪随机函数

**此法适于：**对长度不等的关键字构造哈希函数。




         实际造表时，采用何种构造哈希函数的方法取决于建表的关键字集合的情况(包括关键字的范围和形态)，以及哈希表    长度（哈希地址范围），**总的原则是使产生冲突的可能性降到尽可能地小。**



## **三.       Hash处理冲突方法，各自特征**

**“处理冲突” 的实际含义是：为产生冲突的关键字寻找下一个哈希地址。**
- **  开放定址法**
- **  再哈希法**
- **  链地址法**


      （1）开放定址法：


               为产生冲突的关键字地址 H(key) 求得一个地址序列： H0, H1, H2, …, Hs  1≤s≤m-1，Hi = ( H(key)+di  ) MOD m，其中： i=1, 2, …, s，H(key)为哈希函数;m为哈希表长;




      （2）链地址法：
![](https://img-blog.csdn.net/20140327112842875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmd0ZW5neXVhbjIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






       将所有哈希地址相同的记录都链接在同一链表中。


      （3）再哈希法：


               方法：构造若干个哈希函数，当发生冲突时，根据另一个哈希函数计算下一个哈希地址，直到冲突不再发生。即：Hi=Rhi(key)     i=1,2,……k，其中：Rhi——不同的哈希函数，特点：计算时间增加

## **四.       Hash查找过程**

      对于给定值 K,计算哈希地址 i = H(K)，若 r[i] = NULL  则查找不成功，若 r[i].key = K  则查找成功， 否则 “求下一地址 Hi” ，直至r[Hi] = NULL  (查找不成功)  或r[Hi].key = K  (查找成功) 为止。



## **五.       实现一个使用Hash存数据的场景-------Hash查找算法，插入算法**


         假设我们要设计的是一个用来保存在校学生个人信息的数据表。因为在校学生数量也不是特别巨大(8W?)，每个学生的学号是唯一的,因此，我们可以简单的应用直接定址法，声明一个10W大小的数组，每个学生的学号作为主键。然后每次要添加或者查找学生，只需要根据需要去操作即可。


      但是，显然这样做是**很脑残**的。这样做系统的可拓展性和复用性就非常差了，比如有一天人数超过10W了？如果是用来保存别的数据呢？或者我只需要保存20条记录呢？声明大小为10W的数组显然是太浪费了的。




     如果我们是用来保存大数据量（比如银行的用户数，4大的用户数都应该有3-5亿了吧？），这时候我们计算出来的HashCode就很可能会有冲突了， 我们的系统应该有“处理冲突”的能力，此处我们**通过挂链法“处理冲突”**。




     如果我们的数据量非常巨大，并且还持续在增加，如果我们仅仅只是通过挂链法来处理冲突，可能我们的链上挂了上万个数据后，这个时候再通过静态搜索来查找链表，显然性能也是非常低的。所以我们的系统应该还能实现自动扩容，**当容量达到某比例后，即自动扩容，使装载因子保存在一个固定的水平上**。






## **什么时候ReHash**

**在介绍HashMap的内部实现机制时提到了两个参数，DEFAULT_INITIAL_CAPACITY和DEFAULT_LOAD_FACTOR，DEFAULT_INITIAL_CAPACITY是table数组的容量，DEFAULT_LOAD_FACTOR则是为了最大程度避免哈希冲突，提高HashMap效率而设置的一个影响因子，将其乘以DEFAULT_INITIAL_CAPACITY就得到了一个阈值threshold，当HashMap的容量达到threshold时就需要进行扩容，这个时候就要进行ReHash操作了，可以看到下面addEntry函数的实现，当size达到threshold时会调用resize函数进行扩容。**





**[java]**[view
 plain](http://blog.csdn.net/zhangtengyuan23/article/details/21255311#)[copy](http://blog.csdn.net/zhangtengyuan23/article/details/21255311#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)



- void addEntry(int hash, K key, V value, int bucketIndex) {    
- ntry<K,V> e = table[bucketIndex];    
-       table[bucketIndex] = new Entry<K,V>(hash, key, value, e);    
- if (size++ >= threshold)    
-           resize(2 * table.length);    
-   }    





**在扩容的过程中需要进行ReHash操作，而这是非常耗时的，在实际中应该尽量避免。**




常用字符串哈希函数有BKDRHash，APHash，DJBHash，JSHash，RSHash，SDBMHash，PJWHash，ELFHash等等。对于以上几种哈希函数，我对其进行了一个小小的评测。

|Hash函数|数据1|数据2|数据3|数据4|数据1得分|数据2得分|数据3得分|数据4得分|平均分|
|----|----|----|----|----|----|----|----|----|----|
|BKDRHash|2|0|4774|481|96.55|100|90.95|82.05|92.64|
|APHash|2|3|4754|493|96.55|88.46|100|51.28|86.28|
|DJBHash|2|2|4975|474|96.55|92.31|0|100|83.43|
|JSHash|1|4|4761|506|100|84.62|96.83|17.95|81.94|
|RSHash|1|0|4861|505|100|100|51.58|20.51|75.96|
|SDBMHash|3|2|4849|504|93.1|92.31|57.01|23.08|72.41|
|PJWHash|30|26|4878|513|0|0|43.89|0|21.95|
|ELFHash|30|26|4878|513|0|0|43.89|0|21.95|




其中数据1为100000个字母和数字组成的随机串哈希冲突个数。数据2为100000个有意义的英文句子哈希冲突个数。数据3为数据1的哈希值与1000003(大素数)求模后存储到线性表中冲突的个数。数据4为数据1的哈希值与10000019(更大素数)求模后存储到线性表中冲突的个数。


经过比较，得出以上平均得分。平均数为平方平均数。可以发现，BKDRHash无论是在实际效果还是编码实现中，效果都是最突出的。APHash也是较为优秀的算法。DJBHash,JSHash,RSHash与SDBMHash各有千秋。PJWHash与ELFHash效果最差，但得分相似，其算法本质是相似的。


在信息修竞赛中，要本着易于编码调试的原则，个人认为BKDRHash是最适合记忆和使用的。

## 各种哈希函数的C语言程序代码



```cpp
unsigned int SDBMHash(char *str)
{
    unsigned int hash = 0;

    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
unsigned int RSHash(char *str)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
unsigned int JSHash(char *str)
{
    unsigned int hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
unsigned int PJWHash(char *str)
{
unsigned int BitsInUnignedInt=(unsigned int)(sizeof(unsigned int) * 8);
unsigned int ThreeQuarters=(unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth  = (unsigned int)(BitsInUnignedInt / 8);
 unsigned int HighBits= (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash   = 0;
    unsigned int test   = 0;

    while (*str)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
unsigned int ELFHash(char *str)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;

    while (*str)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// AP Hash Function
unsigned int APHash(char *str)
{
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
}
```








