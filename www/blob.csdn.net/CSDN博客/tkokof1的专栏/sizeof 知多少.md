# sizeof 知多少? - tkokof1的专栏 - CSDN博客

2017年09月19日 19:43:28[tkokof1](https://me.csdn.net/tkokof1)阅读数：353


稍熟悉C/C++的朋友,对于sizeof肯定不陌生,通过他我们可以知晓某个类型或者实例的内存大小(以字节计),但是如果深入一下sizeof计算的细节,想来大部分朋友就不会那么熟稔了,不过平心而论,平日的工作中其实也很少需要准确计算类型的内存大小(当然,定性的估算类型内存占用还是需要的),但是了解一下sizeof底层的计算方式并没有什么坏处,甚至于可能对我们的灵魂都有益处(*Low-level programming is good for the programmer’s soul*),网上关于这个话题的信息其实挺多的,但是大多比较零散,自己尝试写了一篇,算作是一个总结吧~

## 0. 基本类型

像 char, int 等基本类型的 sizeof 大小应该属于基本常识了,值得注意的一点是部分基本类型在32位系统和64位系统中具有不同大小(譬如 long 类型在32位系统中一般为4字节大小,而在64位系统中一般为8字节大小),简单起见,后面的示例我们尽量限制了基本类型的使用,并且有以下约定:

###### sizeof(char) = 1(字节)

###### sizeof(short) = 2(字节)

###### sizeof(int) = 4(字节)

###### sizeof(*pointer*) = 4(字节)(其中*pointer*指代任意的指针类型)

## 1. 结构类型

通过定义结构,我们可以组合使用基本类型,考虑以下代码:

```
struct s1
{
    int m_1;
};
```

sizeof(s1)的大小是多少呢?很简单,对于只有单个成员的结构,其sizeof的大小便是其成员的sizeof大小,拿s1来说:

###### sizeof(s1) = sizeof(s1.m_1) = sizeof(int) = 4(字节)

那么如果s1有多个成员呢?考虑以下代码:

```
struct s1
{
    int m_1;
    int m_2;
    int m_3;
};
```

如果你简单做个试验,你会得到 sizeof(s1) = 12(字节) 的结果,综合之前的讨论,我们似乎可以得出以下结论:
- 结构体 S 的大小等于 S 中各成员的大小之和 *

写成公式可能更直观些(假设结构体 S 的各成员分别为 M1, M2, … Mn):
- sizeof(S) = sizeof(M1) + sizeof(M2) + … + sizeof(Mn) *

OK,既然有了公式,计算结构体的大小便简单了,仍然考虑之前的s1,我们稍稍改动一下他的定义(m_2成员由int类型改为了short类型):

```
struct s1
{
    int m_1;
    short m_2;
    int m_3;
};
```

按照之前总结的公式,我们有:
- sizeof(s1) = sizeof(s1.m_1) + sizeof(s1.m_2) + sizeof(s1.m_3) = sizeof(int) + sizeof(short) + sizeof(int) = 4 + 2 + 4 = 10(字节) *

但实际上,如果你再次试验一番,你会发现 sizeof(s1) **仍然是 12(字节)!**,我们究竟遗漏了什么?

其实这里面有一个**数据对齐**问题

**不同的计算机平台对于数据在内存中的存放位置是有限制的,一般要求数据的内存地址是其大小的整数倍(譬如 int 数据的大小是 4 字节,则其地址也应该是 4 的整数倍,例如0,4,8等等),如果违反了这个存放限制,一些平台需要多次访问内存才能完整读取数据(影响效率),一些平台则会直接产生错误**

由于存在上面的数据对齐问题,编译器在为结构体生成内存布局的时候,会在结构体的数据成员之间填充数据,以使结构体的各个成员都能满足内存对齐需求. 

而关于编译器填充数据的规则,一般都要求填充的数据越少越好,核心的方法如下(假设结构体 S 的各成员分别为 M1, M2, … Mn):

###### 首先假设结构体的起始地址为0(0对于任意的数据类型都是对齐地址)

###### 依次考虑M1, M2, … Mn来计算S的大小,例如对于其中Mi,考虑之前所计算的结构大小是否满足Mi的对齐需求,不满足则填充数据

规则看上去比较抽象,我们拿之前的s1来试验一下:

```
struct s1
{
    int m_1;
    short m_2;
    int m_3;
};
```

假设s1的起始地址是0,依次考虑各个数据成员:

###### 考虑 m_1,由于当前地址是0,满足对齐规则,我们不需要填充数据,s1的大小更新为 sizeof(s1.m_1) = 4

###### 考虑 m_2,由于当前地址是4,而m_2的大小是2,地址4是2的整数倍,所以m_2满足对齐规则,不需要填充数据, s1的大小更新为 4 + sizeof(s1.m_2) = 4 + 2 = 6

###### 考虑 m_3,由于当前地址是6,而m_3的大小是4,地址6不是4的整数倍,所以我们需要填充2字节来使地址增长为4的整数倍(即8),再加上m_3的大小, s1的大小更新为 6 + 2(填充) + sizeof(s1.m_3) = 6 + 2 + 4 = 12

一图胜千言,下面关于s1的内存布局示意图可能更直观一些(其中深色部分为填充数据):

![s1内存布局](https://img-blog.csdn.net/20170919193727939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其实上述的计算过程是可以用公式来表述的(虽然公式不是封闭形式,而是递推形式,但仍然比文字描述来的清晰)

###### * 我们定义函数 sizeof’(S, i) 为结构体 S 考虑了第 i 个成员后计算所得的sizeof大小,则有 sizeof(S) = sizeof’(S, n) (假设S有n个成员) *

###### * sizeof’(S, 0) = 0 *

###### * sizeof’(S, i) = ceil(sizeof’(S, i - 1) / sizeof(Mi)) * sizeof(Mi) + sizeof(Mi) (其中ceil为向上取整函数) *

我们再来利用公式计算一下s1的大小:

###### sizeof’(s1, 0) = 0

###### sizeof’(s1, 1) = ceil(sizeof’(s1, 0) / sizeof(s1.m_1)) * sizeof(s1.m_1) + sizeof(s1.m_1) = ceil(0 / 4) * 4 + 4 = 4

###### sizeof’(s1, 2) = ceil(sizeof’(s1, 1) / sizeof(s1.m_2)) * sizeof(s1.m_2) + sizeof(s1.m_2) = ceil(4 / 2) * 2 + 2 = 6

###### sizeof’(s1, 3) = ceil(sizeof’(s1, 2) / sizeof(s1.m_3)) * sizeof(s1.m_3) + sizeof(s1.m_3) = ceil(6 / 4) * 4 + 4 = 12

###### sizeof’(s1, 3) = sizeof(s1) = 12

可以看到通过使用公式我们清晰化了之前的计算流程

我们继续考虑下面的s1定义(增加了成员m_4):

```
struct s1
{
    int m_1;
    short m_2;
    int m_3;
    short m_4;
};
```

此时sizeof(s1)的大小是多少呢?简单,我们套用之前的公式计算即可:

###### sizeof’(s1, 0) = 0

###### sizeof’(s1, 1) = ceil(sizeof’(s1, 0) / sizeof(s1.m_1)) * sizeof(s1.m_1) + sizeof(s1.m_1) = ceil(0 / 4) * 4 + 4 = 4

###### sizeof’(s1, 2) = ceil(sizeof’(s1, 1) / sizeof(s1.m_2)) * sizeof(s1.m_2) + sizeof(s1.m_2) = ceil(4 / 2) * 2 + 2 = 6

###### sizeof’(s1, 3) = ceil(sizeof’(s1, 2) / sizeof(s1.m_3)) * sizeof(s1.m_3) + sizeof(s1.m_3) = ceil(6 / 4) * 4 + 4 = 12

###### sizeof’(s1, 4) = ceil(sizeof’(s1, 3) / sizeof(s1.m_4)) * sizeof(s1.m_4) + sizeof(s1.m_4) = ceil(12 / 2) * 2 + 2 = 14

###### sizeof’(s1, 4) = sizeof(s1) = 14

公式计算的结果是14,但实际上,如果你试验一下,sizeof(s1)的大小应该是16,我们又遗漏了什么吗?

其实这里还有一个结构体**连续存放**的问题

考虑结构体数组定义 S s[2],并且我们假设s的起始地址是0,对于结构s[0]来说,其各个成员都在对齐的内存地址上,但是对于结构s[1]来讲,其各个成员就未必在对齐的内存地址上了. 

就拿上面的s1举例,如果我们在各个成员对齐后不再做任何填充操作,那么他的大小便是我们刚才通过公式算出的14,那么对于s1的数组定义 s1 s[2](假设s的起始地址为0), 

s[0]的各成员都是满足内存地址对齐的,但是由于结构s1的大小为14,这将导致s[1]的起始地址为14,遂而导致s[1]中的部分成员(譬如s[1].m_1)违反内存对齐原则…

怎么解决这个问题呢?方法就是在对齐结构体的各数据成员之后,再根据结构体中**最大成员的大小**来填充结构体(这里有个前提就是成员大小都必须是2的幂次,或者说最大成员大小一定要能够整除其余成员大小)

继续拿之前的s1举例,我们计算出了他的大小为14,但这只是s1各个成员经过内存对齐之后的结果,我们还需要对s1进行一次“整体”填充,考虑到s1中最大的成员大小为4(s1.m_1和s1.m_3),由于14并不能整除4,所以需要在结构体末尾再填充2个字节(到达16),所以s1的实际大小应为16

依然给张示意图(其中深色部分为填充的数据):

![s1内存布局](https://img-blog.csdn.net/20170919193813796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

综合以上因素,我们可以继续完善结构体大小的计算公式:

###### * 我们定义函数 sizeof’(S, i) 为结构体 S 考虑了第 i 个成员后计算所得的sizeof大小(假设S有n个成员)) *

###### * sizeof’(S, 0) = 0 *

###### * sizeof’(S, i) = ceil(sizeof’(S, i - 1) / sizeof(Mi)) * sizeof(Mi) + sizeof(Mi) (其中ceil为向上取整函数) *

###### * maxsizeof = max(sizeof(M1), sizeof(M2), …, sizeof(Mn)) (其中max为最大值函数)*

###### * sizeof(S) = ceil(sizeof’(S, n) / maxsizeof) * maxsizeof *

## 2. 结构中的结构

有了上面的基础,我们接着来看下如果结构中嵌套结构,那么其sizeof的大小应该如何计算呢?考虑下面的代码:

```
struct s1
{
    int m_1;
    short m_2;
    int m_3;
    short m_4;
};

struct s2
{
    s1 m_1;
    short m_2;
};
```

这次我们要来尝试计算s2的大小,一开始的尝试自然是套用之前总结的公式:

首先对齐各个成员:

###### sizeof’(s2, 0) = 0

###### sizeof’(s2, 1) = ceil(sizeof’(s2, 0) / sizeof(s2.m_1)) * sizeof(s2.m_1) + sizeof(s2.m_1) = ceil(0 / 16) * 16 + 16 = 16

###### sizeof’(s2, 2) = ceil(sizeof’(s2, 1) / sizeof(s2.m_2)) * sizeof(s2.m_2) + sizeof(s2.m_2) = ceil(16 / 2) * 2 + 2 = 18

然后做一次整体填充:

###### maxsizeof = max(sizeof(s2.m_1), sizeof(s2.m_2)) = max(16, 2) = 16

###### sizeof(s2) = ceil(sizeof’(s2, 2) / maxsizeof) * maxsizeof = ceil(18 / 16) * 16 = 32

套用之前的公式,我们得到s2的大小为32,但实际上,试验后的s2大小为20,看来我们又再次遗漏了什么东西…

这次的问题是出在我们之前总结的公式上,由于之前我们仅考虑了基本类型,所以公式中大量使用了sizeof(Mi)这种形式, 

但实际上,我们真正需要的是**alignof(Mi)**这种表达(即某个成员变量的对齐值),而之前的运算之所以正确,完全是因为对于基本类型来说,sizeof(Mi)和alignof(Mi)是相等的! 

而现在我们引入了结构体形式的成员变量,sizeof(Mi) = alignof(Mi)这个前提便不再成立了,套用之前的公式自然也无法获得正确答案…

不过我们依然可以通过alignof来修正之前总结的计算公式(注意sizeof和alignof的使用):

###### * 我们定义函数 sizeof’(S, i) 为结构体 S 考虑了第 i 个成员后计算所得的内存大小(假设S有n个成员) *

###### * sizeof’(S, 0) = 0 *

###### * sizeof’(S, i) = ceil(sizeof’(S, i - 1) / alignof(Mi)) * alignof(Mi) + sizeof(Mi) (其中ceil为向上取整函数) *

###### * maxalignof = max(alignof(M1), alignof(M2), …, alignof(Mn)) (其中max为最大值函数)*

###### * sizeof(S) = ceil(sizeof’(S, n) / maxalignof) * maxalignof *

借助这个修正后的公式,我们再来计算一遍s2的大小:

首先对齐各个成员(结构体的对齐值是其成员的最大对齐值):

###### sizeof’(s2, 0) = 0

###### sizeof’(s2, 1) = ceil(sizeof’(s2, 0) / alignof(s2.m_1)) * alignof(s2.m_1) + sizeof(s2.m_1) = ceil(0 / 4) * 4 + 16 = 16

###### sizeof’(s2, 2) = ceil(sizeof’(s2, 1) / alignof(s2.m_2)) * alignof(s2.m_2) + sizeof(s2.m_2) = ceil(16 / 2) * 2 + 2 = 18

然后做一次整体填充:

###### maxalignof = max(alignof(s2.m_1), alignof(s2.m_2)) = max(4, 2) = 4

###### sizeof(s2) = ceil(sizeof’(s2, 2) / maxalignof) * maxalignof = ceil(18 / 4) * 4 = 20

这次我们得到了s2的正确大小

继续给张s2的内存示意图:

![s2内存布局](https://img-blog.csdn.net/20170919193839539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们接着来考虑以下的结构定义:

```
struct s3
{
    short m_1;
    int m_2[33];
};
```

这里结构体中出现了数组的定义,我们应该如何计算其sizeof大小呢?其实我们只要将数组看做一种特殊的结构即可(大小为 *数组元素大小 * 数组元素个数*, 对齐值为 *数组元素对齐值*).

据此我们依然可以套用之前的公式来计算s3的大小:

首先对齐各个成员:

###### sizeof’(s3, 0) = 0

###### sizeof’(s3, 1) = ceil(sizeof’(s3, 0) / alignof(s3.m_1)) * alignof(s3.m_1) + sizeof(s3.m_1) = ceil(0 / 2) * 2 + 2 = 2

###### sizeof’(s3, 2) = ceil(sizeof’(s3, 1) / alignof(s3.m_2)) * alignof(s3.m_2) + sizeof(s3.m_2) = ceil(2 / 4) * 4 + 4 * 33 = 136

然后做一次整体填充:

###### maxalignof = max(alignof(s3.m_1), alignof(s3.m_2)) = max(2, 4) = 4

###### sizeof(s3) = ceil(sizeof’(s3, 2) / maxalignof) * maxalignof = ceil(136 / 4) * 4 = 136

如果成员是更复杂一些的结构体数组,仍然可以使用相同的方法进行处理,只是这时候的数组元素是结构体罢了.

## 3. union

除了struct/class之外,C/C++中我们还可以定义union,考虑下面的定义:

```
union u1
{
    int m_1;
    short m_2;
    char m_3;
};
```

sizeof(u1)的大小为多少呢?由于联合体需要共用内存,所以其大小是其成员的最大大小,再加上根据联合体对齐值进行填充的数据大小(需要填充的原因和结构体一致),而联合体的对齐值则跟结构体一样,为其成员的最大对齐值

使用公式依然会更清晰一些(假设联合体 U 的各成员分别为 M1, M2, … Mn):

###### alignof(U) = max(alignof(M1), alignof(M2), …, alignof(Mn))

###### sizeof(U) = ceil(max(sizeof(M1), sizeof(M2), …, sizeof(Mn)) / alignof(U)) * alignof(U)

套用这个公式,我们来计算下u1的内存大小:

###### alignof(u1) = max(alignof(u1.m_1), alignof(u1.m_2), alignof(u1.m_3)) = max(4, 2, 1) = 4

###### sizeof(u1) = ceil(max(sizeof(u1.m_1), sizeof(u1.m_2), sizeof(u1.m_3)) / alignof(u1)) * alignof(u1) = ceil(max(4, 2, 1) / 4) * 4 = 4

继续考虑下面的union定义(其中包含了数组和结构):

```
struct s1
{
    int m_1;
    short m_2;
    int m_3;
    short m_4;
};

union u1
{
    int m_1;
    char m_2[33];
    s1 m_3[2];
};
```

初看上去有些吓人,该联合中甚至包含了结构数组!但是套用公式的话,我们依然可以简单的计算出联合的大小(我们已知s1的大小为16,对齐值为4):

###### alignof(u1) = max(alignof(u1.m_1), alignof(u1.m_2), alignof(u1.m_3)) = max(4, 1, 4) = 4

###### sizeof(u1) = ceil(max(sizeof(u1.m_1), sizeof(u1.m_2), sizeof(u1.m_3)) / alignof(u1)) * alignof(u1) = ceil(max(4, 1 * 33, 16 * 2) / 4) * 4 = 36

## 4. bit field(位域)

除了union,在C/C++中还可以通过设置位域来实现数据的紧凑存储,考虑下面的定义:

```
struct s4
{
    int m_1 : 16;
    int m_2 : 8;
    int m_3 : 8;
    short m_4 : 4;
};
```

那么sizeof(s4)应该如何计算呢?一般来讲,相邻并且类型相同的位域数据成员会被打包在一起存储,直到成员的位宽之和超过类型大小,或者遇到不同类型的数据成员(包括非位域数据成员),其中也会进行成员的内存对齐和最后的结构填充.

现在让我们根据上述的规则来计算一下s4的大小(以下计算的单位为位(bit)):

###### 首先假设s4的起始地址为0(对齐地址)

###### 考虑 m_1,由于其类型为int(大小为4(字节) = 32(位)),而 m_1 的位宽为 16, 累计位宽为 0 + 16 <= 32, 可以打包进一个int单元

###### 考虑 m_2,类型为int,与上一个成员类型相同, m_2 的位宽为 8, 累计位宽为 16 + 8 = 24 <= 32,可以打包进一个int单元

###### 考虑 m_3,类型为int,与上一个成员类型相同, m_3 的位宽为 8, 累计位宽为 24 + 8 = 32 <= 32,可以打包进一个int单元

###### 考虑 m_4,类型为short,与上一个成员类型不同,需要重新分配short单元,先考虑对齐,由于上一个数据类型是int,地址变更为4(字节),4(字节)可以整除2(字节)(sizeof(short)),所以不用填充对齐数据,之前的累计位宽被清0

###### 继续考虑 m_4,m_4 的位宽为4,累计位宽为 0 + 4 <= 16,可以打包进一个short单元

###### 尝试填充整个结构,这里可以使用之前总结的公式 sizeof(s4) = ceil(6(字节) / 4(字节)) * 4(字节) = 8(字节)(其中6(字节) = sizeof(int) + sizeof(short),4(字节)是s4的对齐值(即int的对齐值))

实际上位域的相关细节还有很多,不同编译器之间的实现也有不少差异,有兴趣的朋友可以继续[了解一下](http://en.cppreference.com/w/cpp/language/bit_field).

## 5. alignas 和 alignof

c++11引入了对齐相关的 alignas 说明符和 alignof 运算符,其中alignof的作用其实和我们之前自己定义的alignof函数相似,用以获取类型的对齐值,而alignas则提供了一种让我们自行设置类型对齐值的方法.

考虑下面的结构定义:

```
struct alignas(8) s5
{
    char m_1;
    alignas(1) int m_2;
    char m_3;
    alignas(4) short m_4;
};
```

结构体开头的 alignas *尝试*设置了s5的对齐值,之前我们讲过结构体的对齐值为其成员的最大对齐值,即(假设结构体 S 的各成员分别为 M1, M2, … Mn):

alignof(S) = max(alignof(M1), alignof(M2), …, alignof(Mn))

引入了alignas之后,结构体的对齐值变为:

alignof(S) = max(alignas, alignof(M1), alignof(M2), …, alignof(Mn))

注意这里max函数的运用,因为结构体的对齐值取得是alignas及各成员对齐值中的最大对齐值,所以alignas设置的数值不一定就是结构体的对齐值,同样的,对于结构体成员的对齐值我们也可以使用alignas设置,也依然遵循选取最大对齐值的规则.

根据这个规则,我们来计算一下s5的大小(依然可以套用之前的公式,需要注意alignof的计算):

首先对齐各个成员:

###### sizeof’(s5, 0) = 0

###### sizeof’(s5, 1) = ceil(sizeof’(s5, 0) / alignof(s5.m_1)) * alignof(s5.m_1) + sizeof(s5.m_1) = ceil(0 / 1) * 1 + 1 = 1

###### sizeof’(s5, 2) = ceil(sizeof’(s5, 1) / alignof(s5.m_2)) * alignof(s5.m_2) + sizeof(s5.m_2) = ceil(1 / 4) * 4 + 4 = 8 (alignof(s5.m_2) = max(1, 4))

###### sizeof’(s5, 3) = ceil(sizeof’(s5, 2) / alignof(s3.m_3)) * alignof(s3.m_3) + sizeof(s3.m_3) = ceil(8 / 1) * 1 + 1 = 9

###### sizeof’(s5, 4) = ceil(sizeof’(s5, 3) / alignof(s3.m_4)) * alignof(s3.m_4) + sizeof(s3.m_4) = ceil(9 / 4) * 4 + 2 = 14 (alignof(s5.m_4) = max(4, 2))

然后做一次整体填充:

###### maxalignof = max(8, alignof(s5.m_1), alignof(s5.m_2), alignof(s5.m_3), alignof(s5.m_4)) = max(8, 1, 4, 1, 4) = 8

###### sizeof(s5) = ceil(sizeof’(s5, 4) / maxalignof) * maxalignof = ceil(14 / 8) * 8 = 16

s5的内存布局如下:

![s5内存布局](https://img-blog.csdn.net/20170919193922193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 6. pragma pack etc. ?

很多网上的文章一上来都会提到 pragma pack(4),__declspec(align(32)) 或者 __attribute((aligned(16))) 之类的设置,其实是不恰当的,一是因为这些都是非标准的编辑器扩展;二是因为这些设置现在基本都能使用标准的alignas进行替代.

值得一提的是 pragma pack 这个VC扩展,他同alignas类似,可以设置结构体的对齐值,但是使用的是最小值规则,考虑以下定义:

```
#pragma pack(1)
struct s6
{
    int m_1;
    short m_2;
};
#pragma pack()
```

我们使用#pragma pack(1)尝试设置了s6(包括其各个成员)的对齐值为1,那么有:

###### alignof(s6.m_1) = min(1, 4) = 1

###### alignof(s6.m_2) = min(1, 2) = 1

###### alignof(s6) = min(1, alignof(s6.m_1), alignof(s6.m_2)) = 1

进而我们可以使用公式计算得到s6的大小为6

这种类似*压缩*的效果是使用标准的alignas无法实现的(因为alignas使用了max规则),所以在某些场景下可能还需要使用 #pragma pack

## 7. virtual method table (vtable)

C++为了实现多态使用了虚拟函数表,每一个包含至少一个虚函数的类型都会有一张虚函数表,每一个对应类型的实例都存有一个虚函数表指针指向该类型的虚函数表,一般来讲虚函数表指针都放在类型实例内存布局的首部.

考虑以下代码:

```
struct cv1
{
    int m_1;
    virtual ~cv1() {};
};
```

由于有虚拟析构函数,所以cv1的实例会包含一个虚拟函数表指针(简称为vptr),按照之前的约定,指针类型的sizeof大小为4字节,alignof大小也为4字节,在计算cv1的内存大小时,我们仍然可以沿用之前的计算公式:

首先对齐各个成员:

###### sizeof’(cv1, 0) = 0

###### sizeof’(cv1, 1) = ceil(sizeof’(cv1, 0) / alignof(cv1.vptr)) * alignof(cv1.vptr) + sizeof(cv1.vptr) = ceil(0 / 4) * 4 + 4 = 4

###### sizeof’(cv1, 2) = ceil(sizeof’(cv1, 1) / alignof(cv1.m_1)) * alignof(cv1.m_1) + sizeof(cv1.m_1) = ceil(4 / 4) * 4 + 4 = 8

然后做一次整体填充:

###### maxalignof = max(alignof(cv1.vptr), alignof(cv1.m_1)) = max(4, 4) = 4

###### sizeof(cv1) = ceil(sizeof’(cv1, 2) / maxalignof) * maxalignof = ceil(8 / 4) * 4 = 8

cv1的内存布局如下:

![cv1内存布局](https://img-blog.csdn.net/20170919193944862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 8. 单继承

接着我们来看一下类型间单继承的情况,看看内存布局会有什么变化:

```
struct cv1
{
    int m_1;
    virtual ~cv1() {};
};

struct cv2 : public cv1
{
    short m_2;
    virtual ~cv2() {};
};
```

一般而言,如果基类或者继承类存在虚函数表指针的话,vptr会置于类型的内存布局首部(继承类会复用基类的虚函数表指针),然后放置基类的数据成员,最后放置继承类的数据成员,放置方法仍然遵循之前所讲的对齐和填充规则,所以我们仍然可以套用公式来计算cv2的大小:

首先对齐各个成员:

###### sizeof’(cv2, 0) = 0

###### sizeof’(cv2, 1) = ceil(sizeof’(cv2, 0) / alignof(cv2.vptr)) * alignof(cv2.vptr) + sizeof(cv2.vptr) = ceil(0 / 4) * 4 + 4 = 4

###### sizeof’(cv2, 2) = ceil(sizeof’(cv2, 1) / alignof(cv2.m_1)) * alignof(cv2.m_1) + sizeof(cv2.m_1) = ceil(4 / 4) * 4 + 4 = 8 (cv2.m_1来自于基类cv1)

###### sizeof’(cv2, 3) = ceil(sizeof’(cv2, 2) / alignof(cv2.m_2)) * alignof(cv2.m_2) + sizeof(cv2.m_2) = ceil(8 / 2) * 2 + 2 = 10

然后做一次整体填充:

###### maxalignof = max(alignof(cv2.vptr), alignof(cv2.m_1), alignof(cv2.m_2)) = max(4, 4, 2) = 4

###### sizeof(cv2) = ceil(sizeof’(cv2, 3) / maxalignof) * maxalignof = ceil(10 / 4) * 4 = 12

cv2的内存布局如下:

![cv2内存布局](https://img-blog.csdn.net/20170919194012115?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 9. 多继承

C++还支持多继承特性,一个类型可以继承于多个基类(假设基类分别为B1, B2, …, Bn),其中每个基类都可能有成员数据及虚函数表,继承类I也必须能够无缝的向下转形为任一基类,其内存布局的一般规则如下:

###### a. 首先放置B1的虚函数指针(I会复用该虚函数表指针)

###### b. 接着放置B1的数据成员,并逐个执行内存对齐

###### c. 接着放置B2的虚函数指针(如果有的话)

###### d. 接着放置B2的数据成员,并逐个执行内存对齐

###### e. 对接下来的基类Bi重复c和d两个步骤,直到 Bn

###### f. 接着放置I自身的数据成员,并逐个执行内存对齐

###### g. 最后对I整体做一次数据填充

其中,如果B1没有虚函数表,但是后面的Bi有虚函数表,我们就把Bi提前放置(其实就是把之前的基类列表(B1, B2, …, Bi-1, Bi, Bi+1, …, Bn)映射(重排)成了(Bi, B1, B2, …, Bi-1, Bi+1, …, Bn));如果基类都没有虚函数表,但是I自身有虚函数表的话,I的首部则会放置自身的虚函数表指针,否则,I会复用第一个有虚函数表的基类的虚函数表指针.

看一个例子可能更清晰些:

```
struct b1
{
    int m_1;
    char m_2;
    virtual ~b1() {};
};

struct b2
{
    short m_3;
};

struct I : public b1, public b2
{
    int m_4;
    virtual ~I() {};
};
```

我们按照之前的公式来计算一下I的大小:

首先对齐各个成员:

###### sizeof’(I, 0) = 0

###### sizeof’(I, 1) = ceil(sizeof’(I, 0) / alignof(b1.vptr)) * alignof(b1.vptr) + sizeof(b1.vptr) = ceil(0 / 4) * 4 + 4 = 4 (b1的虚函数表指针,I会复用该指针)

###### sizeof’(I, 2) = ceil(sizeof’(I, 1) / alignof(b1.m_1)) * alignof(b1.m_1) + sizeof(b1.m_1) = ceil(4 / 4) * 4 + 4 = 8

###### sizeof’(I, 3) = ceil(sizeof’(I, 2) / alignof(b1.m_2)) * alignof(b1.m_2) + sizeof(b1.m_2) = ceil(8 / 1) * 1 + 1 = 9

###### sizeof’(I, 4) = ceil(sizeof’(I, 3) / alignof(b2.m_3)) * alignof(b2.m_3) + sizeof(b2.m_3) = ceil(9 / 2) * 2 + 2 = 12

###### sizeof’(I, 5) = ceil(sizeof’(I, 4) / alignof(I.m_4)) * alignof(I.m_4) + sizeof(I.m_4) = ceil(12 / 4) * 4 + 4 = 16

然后做一次整体填充:

###### maxalignof = max(alignof(b1.vptr), alignof(b1.m_1), alignof(b1.m_2), alignof(b2.m_3), alignof(I.m_4)) = max(4, 4, 1, 2, 4) = 4

###### sizeof(I) = ceil(sizeof’(I, 5) / maxalignof) * maxalignof = ceil(16 / 4) * 4 = 16

I的内存布局如下:

![I内存布局](https://img-blog.csdn.net/20170919194032996?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 10. 虚拟继承

** 虚拟继承偏于复杂,一般也不推荐使用,讨论虚拟继承相关的内存布局实际来看意义不大,仅供有兴趣的朋友参考 :) **

一般C++的教科书中都会提一下虚拟继承,并说明一下虚拟继承的目的是为了解决[菱形继承导致的重复基类问题](https://en.wikipedia.org/wiki/Virtual_inheritance),如果我们想要计算虚拟继承类型的内存大小,就必须首先了解一下编译器对于虚拟类型的内存布局方法. 

这里首先要说明的一点是,就VC和GCC而言,两者对于虚拟继承类型的内存布局方法是有很大不同的,我们先说下VC的布局方法:

一个类型如果定义了虚拟函数,VC便会为该类型创建虚函数表,同样的,如果定义了虚拟继承,VC便会为该类型创建**虚基类表**,并在类型实例中添加**虚基类表指针(vbptr)**,一般而言,vbptr会被放置在vptr之后,如果类型没有vptr,则vbptr会被放置于实例首部,另外的,虚拟基类的成员也会被放置在继承类的尾部,而不是像普通继承那样从继承类的头部开始(细节可以参考上面小节).

考虑下面的类型定义:

```
struct b1
{
    int m_1;
};

struct b2 : public virtual b1
{
    char m_2;
    virtual ~b2() {};
};

struct b3 : public virtual b1
{
    short m_3;
    virtual ~b3() {};
};

struct I : public b2, public b3
{
    int m_4;
    virtual ~I() {};
};
```

我们还是使用之前的公式来计算一下I的大小:

首先对齐各个成员:

###### sizeof’(I, 0) = 0

###### sizeof’(I, 1) = ceil(sizeof’(I, 0) / alignof(b2.vptr)) * alignof(b2.vptr) + sizeof(b2.vptr) = ceil(0 / 4) * 4 + 4 = 4 (b2的虚函数表指针,I会复用该指针)

###### sizeof’(I, 2) = ceil(sizeof’(I, 1) / alignof(b2.vbptr)) * alignof(b2.vbptr) + sizeof(b2.vbptr) = ceil(4 / 4) * 4 + 4 = 8 (b2的虚基类指针,用以索引b1)

###### sizeof’(I, 3) = ceil(sizeof’(I, 2) / alignof(b2.m_2)) * alignof(b2.m_2) + sizeof(b2.m_2) = ceil(8 / 1) * 1 + 1 = 9

###### sizeof’(I, 4) = ceil(sizeof’(I, 3) / alignof(b3.vptr)) * alignof(b3.vptr) + sizeof(b3.vptr) = ceil(9 / 4) * 4 + 4 = 16 (b3的虚函数表指针)

###### sizeof’(I, 5) = ceil(sizeof’(I, 4) / alignof(b3.vbptr)) * alignof(b3.vbptr) + sizeof(b3.vbptr) = ceil(16 / 4) * 4 + 4 = 20 (b3的虚基类指针,用以索引b1)

###### sizeof’(I, 6) = ceil(sizeof’(I, 5) / alignof(b3.m_3)) * alignof(b3.m_3) + sizeof(b3.m_3) = ceil(20 / 2) * 2 + 2 = 22

###### sizeof’(I, 7) = ceil(sizeof’(I, 6) / alignof(I.m_4)) * alignof(I.m_4) + sizeof(I.m_4) = ceil(22 / 4) * 4 + 4 = 28

###### sizeof’(I, 8) = ceil(sizeof’(I, 7) / alignof(b1.m_1)) * alignof(b1.m_1) + sizeof(b1.m_1) = ceil(28 / 4) * 4 + 4 = 32 (b1被放置在了尾部)

然后做一次整体填充:

###### maxalignof = max(alignof(b2.vptr), alignof(b2.vbptr), alignof(b2.m_2), alignof(b3.vptr), alignof(b3.vbptr), alignof(b3.m_3), alignof(I.m_4), alignof(b1.m_1)) = max(4, 4, 1, 4, 4, 2, 4, 4) = 4

###### sizeof(I) = ceil(sizeof’(I, 8) / maxalignof) * maxalignof = ceil(32 / 4) * 4 = 32

I的内存布局如下:

![I内存布局](https://img-blog.csdn.net/20170919194058450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而GCC采用了不同的方法来实现虚拟继承机制,之前提到VC会为虚拟继承类型生成虚基类表,并在实例中插入虚基类表指针,GCC同样也会为虚拟继承类型生成虚基类表,但是GCC并不会在实例中插入虚基类表指针,相反,GCC”合并”了虚函数表指针(vptr)和虚基类表指针(vbptr), 

或者说GCC只使用了vptr来实现虚函数的重载和虚基类的索引,方法是通过正向索引vptr来定位虚函数(vptr + offset),通过负向索引vptr来定位虚基类(vptr - offset),所以在内存布局上会比VC生成的内存布局小一些,这里我们同样来计算一下GCC为上面的类型I生成的内存布局大小:

首先对齐各个成员:

###### sizeof’(I, 0) = 0

###### sizeof’(I, 1) = ceil(sizeof’(I, 0) / alignof(b2.vptr)) * alignof(b2.vptr) + sizeof(b2.vptr) = ceil(0 / 4) * 4 + 4 = 4 (b2的虚函数表指针,I会复用该指针)

###### sizeof’(I, 2) = ceil(sizeof’(I, 1) / alignof(b2.m_2)) * alignof(b2.m_2) + sizeof(b2.m_2) = ceil(4 / 1) * 1 + 1 = 5 (b2不包含vbptr)

###### sizeof’(I, 3) = ceil(sizeof’(I, 2) / alignof(b3.vptr)) * alignof(b3.vptr) + sizeof(b3.vptr) = ceil(5 / 4) * 4 + 4 = 12 (b3的虚函数表指针)

###### sizeof’(I, 4) = ceil(sizeof’(I, 3) / alignof(b3.m_3)) * alignof(b3.m_3) + sizeof(b3.m_3) = ceil(12 / 2) * 2 + 2 = 14 (b3不包含vbptr)

###### sizeof’(I, 5) = ceil(sizeof’(I, 4) / alignof(I.m_4)) * alignof(I.m_4) + sizeof(I.m_4) = ceil(14 / 4) * 4 + 4 = 20

###### sizeof’(I, 6) = ceil(sizeof’(I, 5) / alignof(b1.m_1)) * alignof(b1.m_1) + sizeof(b1.m_1) = ceil(20 / 4) * 4 + 4 = 24 (b1被放置在了尾部)

然后做一次整体填充:

###### maxalignof = max(alignof(b2.vptr), alignof(b2.m_2), alignof(b3.vptr), alignof(b3.m_3), alignof(I.m_4), alignof(b1.m_1)) = max(4, 1, 4, 2, 4, 4) = 4

###### sizeof(I) = ceil(sizeof’(I, 6) / maxalignof) * maxalignof = ceil(24 / 4) * 4 = 24

I的内存布局如下:

![I内存布局](https://img-blog.csdn.net/20170919194125179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGtva29mMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 11. 杂项

有个关于sizeof的有趣的点可以再提一下,那就是空类型的内存大小:

```
struct E
{
};
```

空类型的大小一般为1,之所以不为0,是为了支持空类型实例的取址,所以我们可以把空类型看做是一种大小为1,对齐值也为1的类型,这样就可以使用之前的公式来计算一些包含空类型的复合结构的内存大小:

```
struct s7
{
    E m_1;
    int m_2;
    E m_3;
    short m_4;
};
```

我们来计算一下s7的内存大小:

首先对齐各个成员:

###### sizeof’(s7, 0) = 0

###### sizeof’(s7, 1) = ceil(sizeof’(s7, 0) / alignof(s7.m_1)) * alignof(s7.m_1) + sizeof(s7.m_1) = ceil(0 / 1) * 1 + 1 = 1

###### sizeof’(s7, 2) = ceil(sizeof’(s7, 1) / alignof(s7.m_2)) * alignof(s7.m_2) + sizeof(s7.m_2) = ceil(1 / 4) * 4 + 4 = 8

###### sizeof’(s7, 3) = ceil(sizeof’(s7, 2) / alignof(s7.m_3)) * alignof(s7.m_3) + sizeof(s7.m_3) = ceil(8 / 1) * 1 + 1 = 9

###### sizeof’(s7, 4) = ceil(sizeof’(s7, 3) / alignof(s7.m_4)) * alignof(s7.m_4) + sizeof(s7.m_4) = ceil(9 / 2) * 2 + 2 = 12

然后做一次整体填充:

###### maxalignof = max(alignof(s7.m_1), alignof(s7.m_2), alignof(s7.m_3), alignof(s7.m_4)) = max(1, 4, 1, 2) = 4

###### sizeof(s7) = ceil(sizeof’(s7, 4) / maxalignof) * maxalignof = ceil(12 / 4) * 4 = 12

有个注意点就是如果类型是继承于空类型而不是包含空类型时,编译器往往会把空类型的占位空间(那一个字节)给优化掉,考虑以下定义:

```
struct s8 : public E
{
    int m_1;
};
```

按照之前公式的计算,你会得出s8的内存大小为8,但是由于有之前所说的编译器优化(即[空基类优化](http://zh.cppreference.com/w/cpp/language/ebo)),所以实际上s8的大小一般为4,当然,如果你把此时的空类型看做一种大小为0,对齐值为1的结构的话,仍然可以使用之前的公式计算得出正确答案:

首先对齐各个成员:

###### sizeof’(s8, 0) = 0

###### sizeof’(s8, 1) = ceil(sizeof’(s8, 0) / alignof(E)) * alignof(E) + sizeof(E) = ceil(0 / 1) * 1 + 0 = 0 (注意此处E类型的特殊处理)

###### sizeof’(s8, 2) = ceil(sizeof’(s8, 1) / alignof(s8.m_1)) * alignof(s8.m_1) + sizeof(s8.m_1) = ceil(0 / 4) * 4 + 4 = 4

然后做一次整体填充:

###### maxalignof = max(alignof(E), alignof(s8.m_1)) = max(1, 4) = 4

###### sizeof(s8) = ceil(sizeof’(s8, 2) / maxalignof) * maxalignof = ceil(4 / 4) * 4 = 4

OK,关于sizeof的内容写了好多,也应该停一下了,那么下次再见吧~

