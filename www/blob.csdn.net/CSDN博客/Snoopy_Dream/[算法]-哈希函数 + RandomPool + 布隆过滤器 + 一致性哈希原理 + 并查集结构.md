# [算法] - 哈希函数 + RandomPool + 布隆过滤器 + 一致性哈希原理 + 并查集结构 - Snoopy_Dream - CSDN博客





2019年01月07日 17:25:38[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：77
所属专栏：[编程感悟](https://blog.csdn.net/column/details/31618.html)









**目录**

[哈希函数](#%E5%93%88%E5%B8%8C%E5%87%BD%E6%95%B0)

[哈希函数的作用：通过 f 哈希函数使得数据变均匀分布，离散化](#%E5%93%88%E5%B8%8C%E5%87%BD%E6%95%B0%E7%9A%84%E4%BD%9C%E7%94%A8%EF%BC%9A%E9%80%9A%E8%BF%87%20f%20%E5%93%88%E5%B8%8C%E5%87%BD%E6%95%B0%E4%BD%BF%E5%BE%97%E6%95%B0%E6%8D%AE%E5%8F%98%E5%9D%87%E5%8C%80%E5%88%86%E5%B8%83%EF%BC%8C%E7%A6%BB%E6%95%A3%E5%8C%96)

[哈希表的实现：](#%E5%93%88%E5%B8%8C%E8%A1%A8%E7%9A%84%E5%AE%9E%E7%8E%B0%EF%BC%9A)

[扩容：](#%E6%89%A9%E5%AE%B9%EF%BC%9A)

[笔试tricks：](#%E7%AC%94%E8%AF%95tricks%EF%BC%9A)

[在C++里面，不怎么用哈希表，而是使用int[1000]这样的数组，时间复杂度会低。](#%E5%9C%A8C%2B%2B%E9%87%8C%E9%9D%A2%EF%BC%8C%E4%B8%8D%E6%80%8E%E4%B9%88%E7%94%A8%E5%93%88%E5%B8%8C%E8%A1%A8%EF%BC%8C%E8%80%8C%E6%98%AF%E4%BD%BF%E7%94%A8int%5B1000%5D%E8%BF%99%E6%A0%B7%E7%9A%84%E6%95%B0%E7%BB%84%EF%BC%8C%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6%E4%BC%9A%E4%BD%8E%E3%80%82)

[在刷题的过程中只要找到O（N*logN）就足够了，N不可能过大，不用找O（N））](#%E5%9C%A8%E5%88%B7%E9%A2%98%E7%9A%84%E8%BF%87%E7%A8%8B%E4%B8%AD%E5%8F%AA%E8%A6%81%E6%89%BE%E5%88%B0O%EF%BC%88N*logN%EF%BC%89%E5%B0%B1%E8%B6%B3%E5%A4%9F%E4%BA%86%EF%BC%8CN%E4%B8%8D%E5%8F%AF%E8%83%BD%E8%BF%87%E5%A4%A7%EF%BC%8C%E4%B8%8D%E7%94%A8%E6%89%BEO%EF%BC%88N%EF%BC%89%EF%BC%89)

[根据数据范围，来找时间复杂度大概多大就过分了。](#%E6%A0%B9%E6%8D%AE%E6%95%B0%E6%8D%AE%E8%8C%83%E5%9B%B4%EF%BC%8C%E6%9D%A5%E6%89%BE%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6%E5%A4%A7%E6%A6%82%E5%A4%9A%E5%A4%A7%E5%B0%B1%E8%BF%87%E5%88%86%E4%BA%86%E3%80%82)

[C++里面1s可执行10^8~9操作。不能超过10^8~9。](#C%2B%2B%E9%87%8C%E9%9D%A21s%E5%8F%AF%E6%89%A7%E8%A1%8C10%5E8~9%E6%93%8D%E4%BD%9C%E3%80%82%E4%B8%8D%E8%83%BD%E8%B6%85%E8%BF%8710%5E8~9%E3%80%82)

[如果告诉你数据是10^6，O（N^2）肯定过不了。10^12超过了10^8~9。](#%E5%A6%82%E6%9E%9C%E5%91%8A%E8%AF%89%E4%BD%A0%E6%95%B0%E6%8D%AE%E6%98%AF10%5E6%EF%BC%8CO%EF%BC%88N%5E2%EF%BC%89%E8%82%AF%E5%AE%9A%E8%BF%87%E4%B8%8D%E4%BA%86%E3%80%8210%5E12%E8%B6%85%E8%BF%87%E4%BA%8610%5E8~9%E3%80%82)

[设计RandomPool结构](#%E8%AE%BE%E8%AE%A1RandomPool%E7%BB%93%E6%9E%84)

[详解布隆过滤器【可能误报，但是不会错报，意思就是在名单里的不会错，但不在的可能会误报】](#%E8%AF%A6%E8%A7%A3%E5%B8%83%E9%9A%86%E8%BF%87%E6%BB%A4%E5%99%A8%E3%80%90%E5%8F%AF%E8%83%BD%E8%AF%AF%E6%8A%A5%EF%BC%8C%E4%BD%86%E6%98%AF%E4%B8%8D%E4%BC%9A%E9%94%99%E6%8A%A5%EF%BC%8C%E6%84%8F%E6%80%9D%E5%B0%B1%E6%98%AF%E5%9C%A8%E5%90%8D%E5%8D%95%E9%87%8C%E7%9A%84%E4%B8%8D%E4%BC%9A%E9%94%99%EF%BC%8C%E4%BD%86%E4%B8%8D%E5%9C%A8%E7%9A%84%E5%8F%AF%E8%83%BD%E4%BC%9A%E8%AF%AF%E6%8A%A5%E3%80%91)

[布隆过滤器公式：](#%E5%B8%83%E9%9A%86%E8%BF%87%E6%BB%A4%E5%99%A8%E5%85%AC%E5%BC%8F%EF%BC%9A)

[作用范围：](#%E4%BD%9C%E7%94%A8%E8%8C%83%E5%9B%B4%EF%BC%9A)

[详解一致性哈希原理](#%E8%AF%A6%E8%A7%A3%E4%B8%80%E8%87%B4%E6%80%A7%E5%93%88%E5%B8%8C%E5%8E%9F%E7%90%86)

[作用范围](#%E4%BD%9C%E7%94%A8%E8%8C%83%E5%9B%B4)

[实现的过程【确定一个东西的归属】](#%E5%AE%9E%E7%8E%B0%E7%9A%84%E8%BF%87%E7%A8%8B%E3%80%90%E7%A1%AE%E5%AE%9A%E4%B8%80%E4%B8%AA%E4%B8%9C%E8%A5%BF%E7%9A%84%E5%BD%92%E5%B1%9E%E3%80%91)

[上面的说法还隐含着负载不均衡的问题](#%E4%B8%8A%E9%9D%A2%E7%9A%84%E8%AF%B4%E6%B3%95%E8%BF%98%E9%9A%90%E5%90%AB%E7%9D%80%E8%B4%9F%E8%BD%BD%E4%B8%8D%E5%9D%87%E8%A1%A1%E7%9A%84%E9%97%AE%E9%A2%98)

[【解决办法】](#%E3%80%90%E8%A7%A3%E5%86%B3%E5%8A%9E%E6%B3%95%E3%80%91)

[并查集结构的详解和实现 【岛屿问题】](#%E5%B9%B6%E6%9F%A5%E9%9B%86%E7%BB%93%E6%9E%84%E7%9A%84%E8%AF%A6%E8%A7%A3%E5%92%8C%E5%AE%9E%E7%8E%B0%C2%A0)

[切分矩阵，并行](#%E5%88%87%E5%88%86%E7%9F%A9%E9%98%B5%EF%BC%8C%E5%B9%B6%E8%A1%8C)

[issameset](#issameset)

[union](#%C2%A0union)

[优化向上查找的过程](#%E4%BC%98%E5%8C%96%E5%90%91%E4%B8%8A%E6%9F%A5%E6%89%BE%E7%9A%84%E8%BF%87%E7%A8%8B)

[时间复杂度](#%E6%97%B6%E9%97%B4%E5%A4%8D%E6%9D%82%E5%BA%A6)

# 哈希函数

1.经典的，**输入域无穷，输出范围有限**，比如MD5，输入无限字符串，输出有限2^64。输入大，输出小。

2.不随机，**同输入，同输出**。不同的输入也 可能输出相同**【哈希碰撞】**

3. 如果输入特别大，输入小，那么输出上每个点是均匀变厚的，如下图，虽然并不一定没个都是33，但是基本差不多。**【均匀性，离散型】**

**多说一句，哈希函数的均匀性，代表着将原始的输入数据完完全全打乱，就算是两个很相近的数据，经过哈希函数之后，对应的值相差也会很远的。**

![](https://img-blog.csdnimg.cn/20190107171705149.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

第三点的性质很重要，**哈希函数处理把不同的数分开的效果很好。**比如20亿个数，找频数最大的数。【只有2g内存，一次处理不完，内存爆炸】

## 哈希函数的作用：**通过 f 哈希函数使得数据变均匀分布，离散化**

每个数经过哈希函数f，**【作用：原先的输入是挺大的，但不一定是均匀分布的，****哈希函数的首个作用就是让其通过f变均匀】**然后%10，**按照余数分成10份，每一种数肯定对应10份里面的一个，所以求众数可以直接10个里面各自找，而且均匀分布保证了10个里面数的数目差不多，2亿左右**】然后10个里面找出每一个频数最大的数，再比较。

输入无限离散，但不一定均匀分布，哈希函数的作用就是让它变均匀分布。然后上面的之后%10也是均匀分布。

![](https://img-blog.csdnimg.cn/20190107171712891.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



## 哈希表的实现：

0~16的桶+单链表的结构。【java里改进单链表变成有序表】

原始数据经过f之后再%17，比如说13，就在之后链接上一个数，如下图。

寻找的时候，比如说zuo 经过f之后 在%17==13，在13里面，遍历查找，对应的32得出。

![](https://img-blog.csdnimg.cn/20190107171733162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## 扩容：

如果出现某一个桶里面太大，我们扩两倍不是%17而是%34,，将旧的全部O（N）到新的桶。

虽然上面说O（N）但使用时增删是O（1），离线变成%34，最终全部扩容完，才让用户接触%34。



![](https://img-blog.csdnimg.cn/20190107171741187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)===================================================================

# **笔试tricks：**
- 
## **在C++里面，不怎么用哈希表，而是使用int[1000]这样的数组，时间复杂度会低。**

- 
## **在刷题的过程中只要找到O（N*logN）**就足够了，N不可能过大，不用找O（N））


## ![](https://img-blog.csdnimg.cn/20190107171752296.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)
- 
## **根据数据范围，来找时间复杂度大概多大就过分了。**


### C++里面1s可执行10^8~9操作。不能超过10^8~9。

### 如果告诉你数据是10^6，O（N^2）肯定过不了。10^12超过了10^8~9。



===================================================================



# 设计RandomPool结构



【 题目】

设计一种结构， 在该结构中有如下三个功能:

insert(key):将某个key加入到该结构， 做到不重复加入

delete(key):将原本在结构中的某个key移除

getRandom(): 等概率随机返回结构中的任何一个key。

【 要求】

Insert、 delete和getRandom方法的时间复杂度都是O(1) 



**解法**：准备两张hash表(一张hash表无法做到严格等概率随机返回一个)

```java
HashMap<String,Integer> keyIndexMap = new HashMap<String, Integer>();
HashMap<Integer,String> indexKeyMap = new HashMap<Integer, String>();
```

**做法**：

A 第0个进入hash表 ， 表A key A value 0 表B key 0 value A 

B 第1个进入hash表 ， 表A key B value 1 表B key 1 value B
**insert(key)**代码实现：

```java
public void insert(String key){
    if(keyIndexMap.containsKey(key)){
        return;
    }else{
        keyIndexMap.put(key,number);
        indexKeyMap.put(number,key);
        number++;
    }
}
```

利用math的random函数，**随机从size**取一个数字，**在哈希表2取对应数字的key**，就是随机等概率的
**getRandom()**代码实现：

```java
public String getRandom(){
    if(size ==0){
        return null;
    }
    int index = (int)(Math.random()*size);
    return map2.get(index);
}
```

**如果要remove呢？**

直接remove会出现问题：删除key对应要删除某个index，那么就会产生“洞”，调用getRandom就一次调用得到等概率结果。
**那么该如何去删呢？**

如假设有1000个key,要删除str17，那么找到index17, 把str999在keyIndexMap的index变为17，map2的17改为str999，删除index999的洞，即产生洞的时候删除最后一条，再删除函数需要删除的key。通过交换最后一行数据保证index是连续的。

```java
public void delete(String key){
    if(keyIndexMap.containsKey(key)){
        Integer deleteIndex = keyIndexMap.get(key);
        int lastIndex = --number;
        String lastKey = indexKeyMap.get(lastIndex);
        indexKeyMap.put(deleteIndex,lastKey);
        keyIndexMap.put(lastKey,deleteIndex);
        keyIndexMap.remove(key);
        indexKeyMap.remove(number);
    }
}
```



两个哈希表可以做到。

不考虑删除的话，根据index直接math.random 26等概率取，然后对应就好。

删除的情况是，删除这一条记录之前，**将其与最后一条记录25置换**，然后删掉最后一个，这样中间没有空洞，size-1就好、

![](https://img-blog.csdnimg.cn/20190107171811443.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

```java
import java.util.HashMap;

public class Code02_RandomPool {

	public static class Pool<K> {
		private HashMap<K, Integer> keyIndexMap;
		private HashMap<Integer, K> indexKeyMap;
		private int size;

		public Pool() {
			this.keyIndexMap = new HashMap<K, Integer>();
			this.indexKeyMap = new HashMap<Integer, K>();
			this.size = 0;
		}

		public void insert(K key) {
			if (!this.keyIndexMap.containsKey(key)) {
				this.keyIndexMap.put(key, this.size);
				this.indexKeyMap.put(this.size++, key);
			}
		}

		public void delete(K key) {
			if (this.keyIndexMap.containsKey(key)) {
				int deleteIndex = this.keyIndexMap.get(key);
				int lastIndex = --this.size;
				K lastKey = this.indexKeyMap.get(lastIndex);
				this.keyIndexMap.put(lastKey, deleteIndex);
				this.indexKeyMap.put(deleteIndex, lastKey);
				this.keyIndexMap.remove(key);
				this.indexKeyMap.remove(lastIndex);
			}
		}

		public K getRandom() {
			if (this.size == 0) {
				return null;
			}
			int randomIndex = (int) (Math.random() * this.size); // 0 ~ size -1
			return this.indexKeyMap.get(randomIndex);
		}

	}

	public static void main(String[] args) {
		Pool<String> pool = new Pool<String>();
		pool.insert("zuo");
		pool.insert("cheng");
		pool.insert("yun");
		System.out.println(pool.getRandom());
		System.out.println(pool.getRandom());
		System.out.println(pool.getRandom());
		System.out.println(pool.getRandom());
		System.out.println(pool.getRandom());
		System.out.println(pool.getRandom());

	}

}
```

```

```



=========================================================================

# **详解布隆过滤器**【可能误报，但是不会错报，意思就是在名单里的不会错，但不在的可能会误报】



设置**黑名单url**的时候，如果用hashset 内存640G太过了。



![](https://img-blog.csdnimg.cn/20190107171835764.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

一个url经过**k**个哈希函数[特征点],%**m**之后进入位图，抹黑，将0-1,1还是1,。

来了一个新的url，经过**k**个哈希函数[特征点],%**m后****如果都是黑就说在黑名单里，有一个位置不是黑就肯定不在黑名单。**

![](https://img-blog.csdnimg.cn/20190107171900916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



## 布隆过滤器公式：

只和样本量和失误率有关

向上取整数

![](https://img-blog.csdnimg.cn/20190107171911733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

**3）是指真实的失误率，因为你给的不是12.7而是给13，所以真实的失误率比p0.0001要小。**

## ***作用范围： ***

***黑名单系统，爬虫去重的系统、在不在一个集合里，样本很大的情况里，多嘴问一句允不允许用有误报率。系统上给我的内存空间多大。***



***=====================================================================***



# 详解一致性哈希原理



## 作用范围

**最常使用的后台服务器架构之一，分布式数据库啊什么的就会用到。**

**是为了解决经典服务器的弊端产生的，处理数据，在将数据分开放到不同的机器上的时候要用到一致性哈希。**

经典服务器如下：request之后通过前端，那么多端都经过哈希和%得出2就去后端2.

弊端：增减后端机器，代价是全量的。

![](https://img-blog.csdnimg.cn/20190107171924584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



通过md5生成哈希，这里看成环结构。

后端m1 m2 m3都有各自独特的hostkey ip等，把他通过哈希，打到环上。来了一个新的请求request**直接f哈希函数，但是*不用%***，顺时针遇到的第一个后端就是对应的处理的机器。

下图中r代表着request，m代表着后端的服务器

![](https://img-blog.csdnimg.cn/20190107171932427.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



## **实现的过程【确定一个东西的归属】**

![](https://img-blog.csdnimg.cn/20190107171952823.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



## **上面的说法还隐含着负载不均衡的问题**

你不能保证三台m服务器均分任务，即便是认为分开三个，再新加一个进来，没上就负载不均衡了。

## **【解决办法】**

虚拟节点

a表示虚拟节点，和m服务器构成路由表。

虚拟节点去抢环。当增加m4机器的时候，几乎是向其他三个等要一些，最后1/4。

最后对应到哪个m机器。

而且如果m1特别好的机器，可以多给写虚拟节点，达到管理负载、分配负载的功能。



![](https://img-blog.csdnimg.cn/20190107172005841.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

===========================================================================

# **并查集结构的详解和实现 【岛屿问题】**



【 题目】

一个矩阵中只有0和1两种值， 每个位置都可以和自己的上、 下、 左、 右 四个位置相连， 如果有一片1连在一起， 这个部分叫做一个岛， 求一个矩阵中有多少个岛?

【 举例】

001010

111010

100100

000000

这个矩阵中有三个岛

【 进阶】

如何设计一个并行算法解决这个问题 



将矩阵从左向右，从上向下遍历，遇到1的时候，实现一个感染过程，把所有能接触到的1变成2【递归】.岛的数目+1.![](https://img-blog.csdnimg.cn/201901071720163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



```java
public class Code03_Islands {

	public static int countIslands(int[][] m) {
		if (m == null || m[0] == null) {
			return 0;
		}
		int N = m.length;
		int M = m[0].length;
		int res = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (m[i][j] == 1) {
					res++;
					infect(m, i, j, N, M);
				}
			}
		}
		return res;
	}

	public static void infect(int[][] m, int i, int j, int N, int M) {
		if (i < 0 || i >= N || j < 0 || j >= M || m[i][j] != 1) {
			return;
		}
		m[i][j] = 2;
		infect(m, i + 1, j, N, M);
		infect(m, i - 1, j, N, M);
		infect(m, i, j + 1, N, M);
		infect(m, i, j - 1, N, M);
	}
```

```

```

## **切分矩阵，并行**

核心逻辑：如下图切一刀。发现有abcd4个岛屿，记录岛屿的四个边界信息。

我们把刀附近的2标记为属于a，之后合并的时候看一下，ac是不是一个集合，不是，但是刀左右，所以合并为一个岛屿。



![](https://img-blog.csdnimg.cn/20190107172035181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

**并查集结构**

![](https://img-blog.csdnimg.cn/20190107172104105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

刚开始将每一个数据，搞成一个集合，有两个操作

1.查是不是一个集合里的 issameset

2.合并操作 union

虽然说list和hash都可以实现，但是不高效，都涉及到了数据整体迁移的过程。【数据整体迁移——》不高效】

代表节点：头结点

## **issameset**

不断往上查，是否有同一个头。



![](https://img-blog.csdnimg.cn/201901071721109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## union

先用issameset查一下，是不是同一个，不是就可以union

比较少节点的挂多的。小集合的代表节点指向大集合的代表节点。

删除掉小集合，把大集合中的个数增加2.

![](https://img-blog.csdnimg.cn/20190107172116663.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)

## **优化向上查找的过程**

比如上面5-3-4，变成3-4,5-4，意思就是如果不是直接与head相连，变成直接的。【扁平化】



## **时间复杂度 **

**如果a+b的次数大于N，时间复杂度是O(1)**![](https://img-blog.csdnimg.cn/20190107172124750.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



```java
import java.util.HashMap;
import java.util.List;
import java.util.Stack;

public class Code04_UnionFind {

	public static class Element<V> {//加了个封装
		public V value;

		public Element(V value) {
			this.value = value;
		}

	}

	public static class UnionFindSet<V> {
		public HashMap<V, Element<V>> elementMap;//v为值【用户课件】，Element为元素【自己用】
		public HashMap<Element<V>, Element<V>> fatherMap;//父节点
		public HashMap<Element<V>, Integer> sizeMap;//代表节点的集合有多少个节点

		public UnionFindSet(List<V> list) {
			elementMap = new HashMap<>();
			fatherMap = new HashMap<>();
			sizeMap = new HashMap<>();
			for (V value : list) {
				//初始化
				Element<V> element = new Element<V>(value);
				elementMap.put(value, element);
				fatherMap.put(element, element);//每个节点是自己的父
				sizeMap.put(element, 1);//每个节点是自己代表节点
			}
		}

		private Element<V> findHead(Element<V> element) {
			Stack<Element<V>> path = new Stack<>();//沿途的路
			while (element != fatherMap.get(element)) {
				path.push(element);//一直走到代表节点
				element = fatherMap.get(element);
			}
			while (!path.isEmpty()) {
				fatherMap.put(path.pop(), element);//扁平化，所有的节点都是直接指向代表节点的
			}
			return element;
		}

		public boolean isSameSet(V a, V b) {
			//看a有没有注册过，一个值都有对应的element
			if (elementMap.containsKey(a) && elementMap.containsKey(b)) {
				return findHead(elementMap.get(a)) == findHead(elementMap.get(b));
			}
			return false;
		}

		public void union(V a, V b) {
			if (elementMap.containsKey(a) && elementMap.containsKey(b)) {
				//先找代表节点【头结点】
				Element<V> aF = findHead(elementMap.get(a));//找a对应的元素的代表节点
				Element<V> bF = findHead(elementMap.get(b));//代表节点里面有这个结合的大小
				if (aF != bF) {
					//将少的挂在大的下面
					Element<V> big = sizeMap.get(aF) >= sizeMap.get(bF) ? aF : bF;//big是大集合的代表节点
					Element<V> small = big == aF ? bF : aF;
					//直接将少的集合的代表节点换成大集合的代表节点
					fatherMap.put(small, big);
					//改变代表节点对应的size的大小
					sizeMap.put(big, sizeMap.get(aF) + sizeMap.get(bF));
					sizeMap.remove(small);//删除这个代表节点的记录
				}
			}
		}

	}

}
```







