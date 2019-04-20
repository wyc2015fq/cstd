# 海量数据处理之bitmap - Spark高级玩法 - CSDN博客
2018年11月16日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：165
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVTgibLSUIGrncPBTricf27tzJycFNmSWTiaXVKA8sPjMhssFNHA5aI4pCMoDo4IBf2mtiawRoQ7MtJmg/640)
**一、概述**
本文将讲述Bit-Map算法的相关原理,Bit-Map算法的一些利用场景，例如BitMap解决海量数据寻找重复、判断个别元素是否在海量数据当中等问题.最后说说BitMap的特点已经在各个场景的使用性。
**二、Bit-Map算法**
先看看这样的一个场景：给一台普通PC，2G内存，要求处理一个包含40亿个不重复并且没有排过序的无符号的int整数，给出一个整数，问如果快速地判断这个整数是否在文件40亿个数据当中？
问题思考：
40亿个int占（40亿*4）/1024/1024/1024 大概为14.9G左右，很明显内存只有2G，放不下，因此不可能将这40亿数据放到内存中计算。要快速的解决这个问题最好的方案就是将数据搁内存了，所以现在的问题就在如何在2G内存空间以内存储着40亿整数。一个int整数在java中是占4个字节的即要32bit位，如果能够用一个bit位来标识一个int整数那么存储空间将大大减少，算一下40亿个int需要的内存空间为40亿/8/1024/1024大概为476.83 mb，这样的话我们完全可以将这40亿个int数放到内存中进行处理。
具体思路：
1个int占4字节即4*8=32位，那么我们只需要申请一个int数组长度为 int tmp[1+N/32]即可存储完这些数据，其中N代表要进行查找的总数，tmp中的每个元素在内存在占32位可以对应表示十进制数0~31,所以可得到BitMap表:
tmp[0]:可表示0~31
tmp[1]:可表示32~63
tmp[2]可表示64~95
.......
那么接下来就看看十进制数如何转换为对应的bit位：
假设这40亿int数据为：6,3,8,32,36,......，那么具体的BitMap表示为：
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFVTgibLSUIGrncPBTricf27tzPndkiaceHqicBxFW8EWmSc6NR95PBJ6a3Ls8Gcm9svernuaeJ09LCeug/640)
如何判断int数字在tmp数组的哪个下标，这个其实可以通过直接除以32取整数部分，例如：整数8除以32取整等于0，那么8就在tmp[0]上。另外，我们如何知道了8在tmp[0]中的32个位中的哪个位，这种情况直接mod上32就ok，又如整数8，在tmp[0]中的第8 mod上32等于8，那么整数8就在tmp[0]中的第八个bit位（从右边数起）。
java里面有个实现就是bitset，一个老早就有的工具，具体使用，可以参考如下源码：
```
import java.util.BitSet;
public class BitSetTest {
    public static void main(String[] args) {
        int [] array = new int [] {1,2,3,22,0,3,63};
        BitSet bitSet  = new BitSet(1);
        System.out.println(bitSet.size());   //64
        bitSet  = new BitSet(65);
        System.out.println(bitSet.size());   //128
        bitSet  = new BitSet(23);
        System.out.println(bitSet.size());   //64
        //将数组内容组bitmap
        for(int i=0;i<array.length;i++)
        {
            bitSet.set(array[i], true);
        }
        System.out.println(bitSet.get(22));
        System.out.println(bitSet.get(60));
        System.out.println("下面开始遍历BitSet：");
        for ( int i = 0; i < bitSet.size(); i++ ){
            System.out.println(bitSet.get(i));
        }
    }
}
```
当然，这个是使用现成，自己写的话也是很简单，下面有一个简单的实现：
/**
 * 实现BitMap
 *注：这个bitMap的index是从1开始的
 */
public class BitMap {
private long length;
private static int[] bitsMap;
//构造函数中传入数据中的最大值
public BitMap(long length) {
this.length = length;
// 根据长度算出，所需数组大小
bitsMap = new int[(int) (length >> 5) + ((length & 31) > 0 ? 1 : 0)];
}
public int getBit(long index) {
int intData = bitsMap[(int) ((index - 1) >> 5)];
int offset = (int) ((index - 1) & 31);
return intData >> offset & 0x01;
}
public void setBit(long index) {
// 求出该index - 1所在bitMap的下标
int belowIndex = (int) ((index - 1) >> 5);
// 求出该值的偏移量(求余)
int offset = (int) ((index - 1) & 31);
int inData = bitsMap[belowIndex];
bitsMap[belowIndex] = inData | (0x01 << offset);
}
public static void main(String[] args) {
BitMap bitMap = new BitMap(32);
bitMap.setBit(32);
System.out.println(bitMap.getBit(1));
System.out.println(bitMap.getBit(32));
}
}
推荐阅读：
[JVM的垃圾回收算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483661&idx=1&sn=d6427470f125b58e92a5d6b27085e982&chksm=9f38e225a84f6b3374e48002abc6f86d5190a9cf5ce59fa5ad6ca4bdd2df64c654e8f85c5413&scene=21#wechat_redirect)
[Java面试知识点解析——JVM篇](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484940&idx=1&sn=c58300507839260ce276aaaa5f478c59&chksm=9f38e524a84f6c321f606434cc53dc354ea6254131f628777550121c45c4ddb7ef349147a888&scene=21#wechat_redirect)
[HBase最佳实践－读性能优化策略](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485168&idx=1&sn=a3ad3ef87ac23f6ef7a84f8d133b887a&chksm=9f38e5d8a84f6cce60b2b3c670ab653b48dcb6c04c52de480295f0d160e78b054622d066de86&scene=21#wechat_redirect)
[Kafka源码系列之0.10版本的Producer源码解析及性能点讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483747&idx=1&sn=ba7bb28c7b8eacd607ffe6f9afbe7e2b&chksm=9f38e24ba84f6b5dd752e7fced79ad6cad9370e4356a39524272a0805eef5f50674534cca31c&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
欢迎点赞转发，星球入口点击阅读原文
