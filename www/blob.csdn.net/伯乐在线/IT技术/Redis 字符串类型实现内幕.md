# Redis 字符串类型实现内幕 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Float_Lu](http://www.jobbole.com/members/5334670325) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 摘要
Redis不仅仅是一个key-value存储，它更是一个数据结构服务，支持不同类型的值。这意味着在传统的key-value存储中，我们用string的key关联string的value。而在Redis中，我们可以存储的值不受限于string，我们还可以存储复杂的数据结构。string是我们在使用Redis过程中能接触到的最简单的数据类型，也是Memcached中仅有的类型，因此对于Redis新手来说，首先选择使用string类型是理所当然的。这篇文章主要介绍Redis的string类型的实现内幕。
## 初识：简单动态字符串
Redis中使用的字符串是通过包装的，基于c语言字符数组实现的一个抽象数据结构，后文中提到的sds指的就是简单动态字符串，它的定义和实现在sds.h和sds.c中，结构是这样的：

Shell
```
struct sdshdr {
    int len;
    int free;
    char buf[];
};
```
Redis中定义了这样一个结构体来表示字符串，字段含义如下：
- len表示buf中存储的字符串的长度。
- free表示buf中空闲空间的长度。
- buf用于存储字符串内容。
举个例子：
![13](http://jbcdn2.b0.upaiyun.com/2016/06/7e51746feafa7f2621f71943da8f603c2.jpg)
**图1**
假设上面图1是当前buf中存储的内容，那么这个时候len为8，free为2，sds的内存占用量可以用下面公式表示：

Shell
```
sizeof(struct sdshdr) + len + free + 1
```
初识了sds之后，我们下面分别从使用字符串的时候最关心的几个点来继续认识sds：
- 存储内容
- 长度计算
- 字符串拼接
- 字符串截断
## 存储内容：二进制安全字符串
Redis keys是二进制安全的，对于是不是二进制安全，简单理解就是对于字符串结构，我们能不能用它来存储二进制。我们都知道传统的C字符串是zero-terminated的，也就是C语言字符串函数库认为字符串是以’\0’结尾的，因此对于用来表示字符串的C语言字符数组中中间不能有’\0’，不然在处理的过程中会出错，比如下面这段:
![11](http://jbcdn2.b0.upaiyun.com/2016/06/7bfc85c0d74ff05806e0b5a0fa0c1df13.jpg)
**图2**
我们申请了length为9的char数组，将每个字母都放到对应的位置，我们期望得到的是”Float Lu”这样的字符串，而实际C字符串函数处理的过程中会以为这个字符串是”Float”，而这并不是我们期望的结果。
而二进制安全的字符串，Redis中给的术语是binary-safe，它允许我们把图2中表示的数据当做字符串来使用，那这个二进制有什么关系呢，因为二进制数据通常会有中间某个字节存储’\0’的这种情况，比如我们存储一个JPEG格式图片，因此二进制安全的字符串结构允许我们存储像JPEG格式图片的这种数据。
从而在Redis中我们不仅仅可以使用传统字符串来当做key，使用二进制来作为key也是被允许的，比如图片、视频、音频……whatever，然而你不要高兴太早，Redis对key的长度是有限制的，最大长度是512MB。
## 长度计算：O（1）时间复杂度
### c语言中strlen的实现
strlen在c语言中用来计算c语言字符串的长度，strlen的实现很简单，从内存中字符串开始的位置开始扫描并计数，知道碰到第一个’\0’为止，这也是为什么c语言字符串是zero-terminated的原因。很显然，strlen的时间复杂度是O（N）。
### sds中sdslen的实现
sds中用于对字符串长度计算的函数为sdslen，我们看一下它的实现：

Shell
```
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}
```
我们想要获取的sds的长度就是sdshdr中定义的len的值，时间复杂度是O（1）。
## 字符串拼接：动态扩容机制
通常情况我们对于字符串的拼接不仅仅是一次，而是很多次，我们写JAVA的通常很有感触，比如我们要根据用户名来拼接一个字符串，又考虑到执行效率，我们通常会借助于StringBuilder像下面这样写：

Shell
```
public String makeWelcomeStr(String username) {
     StringBuilder sb = new StringBuilder();
     sb.append("welcome ");
     sb.append(username);
     sb.append("!");
     return sb.toString();
}
```
对于C语言来说我们并不能这么潇洒，我们需要先苦逼的申请一块内存区域将”welcome “放入，当我们需要拼接username的时候，我们需要苦逼的再申请一块内存，长度为原有内容长度加上username的长度，然后再将原有内容拷贝到新的内存区域，然后再放心的将username放入新的内存区域的后面……还有”!”没有拼接呢，我天！
苦逼！
sds中我们不需要考虑拼接的时候要不要扩容，扩多少容等，这些sds都为我们做了，我们只需要简单的调用sdscat即可（sds中用来拼接字符串的函数是sdscat），sdscat的核心实现在sdscatlen和sdsMakeRoomFor中，假设我们正在拼接字符串：
![12](http://jbcdn2.b0.upaiyun.com/2016/06/c8b2f17833a4c73bb20f88876219ddcd3.jpg)
**图3**
我的名字是”Float Lu”，我将它拼接在”welcome”后面，我不需要考虑buf的free长度是多少，能不能放下”Float Lu”，我们将要放的字符串长度为8，看看sds是怎么做的：
在拼接新的字符串之前会检查当前free是否够用，如果当前的free空间大于等于8，则不需要申请内存，直接将字符串放入，修改len和free。
如果空间不够用，sds有一套扩容规则，接着上面的例子，老的内容长度为len=9，新的内容长度newlen=len+8，为16：
- 如果newlen小于1024（byte） * 1024（byte）=1（MB）则新的长度为二倍的newlen。
- 如果newlen的长度大于等于1MB，则新的newlen的长度为newlen的长度加上1MB。
（这让我想起了Netty的内存扩容规则），接着上面的例子，扩容完之后的len为16，free为16，加上1字节的’\0’。
这个时候我们再继续拼接”!”的时候可以直接将”!”放入刚才申请多余的内存区域内同时将len加1，将free减1即可。
sds通过预分配一些内存区域来减少内存申请，拷贝的次数，虽然预分配规则很简单，但是是很有效的。
## 字符串截断：内存空间懒释放
考虑到我们要清理字符串中的一些内容，传统的做法是新申请一块内存区域，将需要保留的内容放入新的区域然后释放原始区域，这其中必然会涉及内存的申请，拷贝。加入这个时候又有往刚才保留的字符串后面拼接一个字符串又要涉及一些重操作，比如内存申请，拷贝。。。
我们来看看sds是怎么做的，在sds中提供了sdstrim这样的一个方法，它的定义：

Shell
```
sds sdstrim(sds s, const char *cset)
```
即清除s中所有在cset中出现过的字符，看一个例子：

Shell
```
s = sdsnew("AA...AA.aHelloWorld::");
s = sdstrim(s,"A. :");
printf("%s\n", s);
```
结果是”Hello World”。
对于上面的情况，原来的len为21，假如free为0，清理完成之后不涉及内存的申请操作，len为10，free为11，加入这个时候有字符串拼接需求，直接将内容放到free的11个字节内即可，当然是如果放的下的话。
sds并不会立即释放掉不需要的已经申请的内存，实际中，这些内存后续很可能还能会被用到，如果你担心内存浪费的话，可以手动调用sds提供的接口释放这些空间，比如sdsfree函数。
## sds VS c语言字符串
上面我们分别字符串操作最常涉及到的一些问题认识了sds，最后我们通过将sds和c语言字符串进行比较一下来总结sds的优缺点：
|C语言|sds|
|----|----|
|占用内存通常为内容长度|占用内存包括结构体和free的长度|
|非二进制安全|二进制安全|
|长度计算时间复杂度为O（N）|长度计算时间复杂度为O（1）|
|需要掌握字符串的长度|sds帮助我们把握长度和内存申请|
|字符串拼接每次要进行内存申请和拷贝|不一定内次都要申请内存和拷贝|
## 总结
sds在Redis中作为字符串基础服务，为Redis的keys和其他涉及string操作的地方提供服务，sds的设计不仅考虑到api使用的安全性，更多的是为了提高性能，为高性能Redis奠定基础。字符串操作方面提高性能的核心点在于尽量减少内存的申请和内存拷贝，在设计的时候允许利用一定的内存空间换取时间效率。
## 参考文献
《Redis Documentation》
《Redis2.8.13源码》
《Redis设计与实现》
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/05/7cb05a1a5b9ef06cc9d40cc79a096f293.png)![](http://jbcdn2.b0.upaiyun.com/2016/05/12e72c4df391cc981614cc68aedd44085.jpg)
