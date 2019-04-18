# 编程小知识之switch语句 - tkokof1的专栏 - CSDN博客

2018年12月22日 12:03:17[tkokof1](https://me.csdn.net/tkokof1)阅读数：57


> 
本文简述了几种 switch 语句的实现方式

#### 基础

switch 语句大家都不陌生,平时在遇到较多相同的 if 语句判断时就会想到他,举个简单的例子:

以下的代码

```
// val is int
if (val == 1)
{
    Func1();
}
else if (val == 2)
{
    Func2();
}
else if (val == 3)
{
    Func3();
}
else
{
    Func4();
}
```

改写为 switch 语句效率会更高.

```
// val is int
switch (val)
{
    case 1:
        Func1();
        break;
    case 2:
        Func2();
        break;
    case 3:
        Func3();
        break;
    default:
        Func4();
        break;
}
```

至于 switch 语句效率更高的原因,不少同学可能都知道是因为 switch 语句使用了跳转表,拿上面的 switch 语句举例,编译器会首先生成一张跳转表:

![跳转表示意图](https://img-blog.csdnimg.cn/20181222115604258.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

然后对 val 执行一次减法操作来获取 val 所对应的跳转表索引:

$$index = val - base$$

公式中的 $base$ 其实就是 switch 语句中第一个 case 条件的数值(即数值 1).

接着我们就可以根据计算得到的跳转表索引去执行对应的分支代码了:

![跳转表示意图,图中展示的是 index == 2 的情况](https://img-blog.csdnimg.cn/20181222115630821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

当然,这里还有一个细节,就是我们计算所得的的跳转表索引可能是非法的(譬如 val == 4),所以我们还要在执行实际的跳转之前进行一次范围判断.

综上,示例中的 switch 语句经过编译器的翻译,大概会变成下面的样子(伪代码,其中的 table 即跳转表):

```
base = 1;
index = val - base;
if (index > 2)
{
    jump to default;
}
else
{
    jump to table[index];
}
```

实际上, switch 语句的实现方式会更复杂一些,考虑下面的代码:

```
// val is int
switch (val)
{
    case 1:
        Func1();
        break;
    case 3:
        Func2();
        break;
    case 5:
        Func3();
        break;
    default:
        Func4();
        break;
}
```

可以看到,各个分支的整数值并不连续,而我们之前使用的跳转表的索引却是连续的,似乎没办法直接沿用跳转表来实现上面的 switch 语句了.

这里运用一下添加中间层的编程法则,我们再额外添加一个表来做跳转表索引的映射,通过这种两级跳转表的方式,我们便可以解决 switch 语句中离散整数分支的问题(之前的方法仅使用了一张跳转表,所以也叫一级跳转表):

![二级跳转表,itable中灰色表格位置没有指向跳转索引](https://img-blog.csdnimg.cn/20181222115655405.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3Rrb2tvZjE=,size_16,color_FFFFFF,t_70)

二级跳转表的实现原理如下(伪代码,其中 itable 为跳转索引表, table 为跳转表)

```
base = 1;
iindex = val - base;
if (iindex > 4)
{
    jump to default;
}
else
{
    index = itable[iindex];
    jump to table[index2];
}
```

虽然二级跳转表能够解决离散整数分支的问题,但是我们也注意到,其中第一级的跳转索引表大小却正比于各个分支的整数值范围,这会导致当整数分支较为离散时,跳转索引表的大小会非常大.

考虑下面的 switch 语句:

```
// val is int
switch (val)
{
    case 100:
        Func1();
        break;
    case 300:
        Func2();
        break;
    case 500:
        Func3();
        break;
    case 700:
        Func4();
        break;
    case 900:
        Func5();
        break;
    default:
        Func6();
        break;
}
```

可以看到上述代码中整数分支较为离散,各个分支的整数值范围很大(900 - 100 = 800),直接使用二级跳转表的话,跳转索引表的空间消耗比较大,此时我们可以将各个分支整数排序,然后使用二分查找的思想来实现 switch 语句,拿上面的代码举例,相关的实现伪码如下:

```
if (val == 500)
{
    jump to case 500;
}
else if (val < 500)
{
    // < 500
    if (val == 300)
    {
        jump to case 300;
    }
    else
    {
        if (val == 100)
        {
            jump to case 100;
        }
        else
        {
            jump to default;
        }
    }
}
else
{
    // > 500
    if (val == 900)
    {
        jump to case 900;
    }
    else
    {
        if (val == 700)
        {
            jump to case 700;
        }
        else
        {
            jump to default;
        }
    }
}
```

除了上面介绍的几种方式, switch 语句还有更多的实现方法,譬如直接使用 if 语句逐个判断(当分支较少时),或者混合使用跳转表和二分查找(当整数分支一部分连续一部分离散时)等等,有兴趣的朋友可以继续探索.

#### 扩展

C/C++ 的 switch 语句必须使用整型变量(*之前所讲的各个 switch 语句实现大部分都基于这个前提*),但在 C# 中,switch 语句的使用变的更加自由,一个实用的例子就是你可以在 switch 语句中使用字符串:

```
// val is string
switch (val)
{
    case "aaa":
        Func1();
        break;
    case "bbb":
        Func2();
        break;
    case "ccc":
        Func3();
        break;
    default:
        Func4();
        break;
}
```

之前我们介绍的各种 switch 语句实现,基本都是基于整型值的,那么 C# 中这种字符串形式的 switch 语句又是如何实现的呢?

让我们来看下上述 switch 语句的 IL 代码(部分):

```
// compare with "aaa"
ldloc.1
ldstr "aaa"
call bool [mscorlib]System.String::op_Equality(string, string)
brtrue.s IL_0031 // case "aaa"

// compare with "bbb"
ldloc.1
ldstr "bbb"
call bool [mscorlib]System.String::op_Equality(string, string)
brtrue.s IL_0039 // case "bbb"

// compare with "ccc"
ldloc.1
ldstr "ccc"
call bool [mscorlib]System.String::op_Equality(string, string)
brtrue.s IL_0041 // case "ccc"

br.s IL_0049 // default
```

可以看到,C# 编译器直接使用了逐条 if 语句件判断的方法来实现字符串形式的 switch 语句.

这就引出了一个问题,当字符串分支较多时,性能会比较差(与直接使用 if 语句判断效率是一致的,但是不及整型形式的 switch 语句),我们能否进一步优化呢?

其实思路也比较简单,就是将问题规约,即将字符串形式的 switch 语句转换为整数形式的 switch 语句:

$$f(str) \implies int$$

转换的方式不少,一种简单的方法便是使用字典,将字符串和其对应的整数存储起来,转换时直接从字典取值即可,相关代码如下:

```
// val is string
var dict = new Dictionary<string, int>
{
    { "aaa", 1 },
    { "bbb", 2 },
    { "ccc", 3 },
};

var tval = 0;
dict.TryGetValue(val, out tval);

switch (tval)
{
    case 1:
        Func1();
        break;
    case 2:
        Func2();
        break;
    case 3:
        Func3();
        break;
    default:
        Func4();
        break;
}
```

其实之前介绍的二级跳转表方法也是使用了转换的思想,借助额外的跳转索引表,其将离散的整数分支映射为了连续的整数分支.

实际上,C# 编译器已经为我们实现了类似的方法,让我们把上面 switch 语句的分支增加一些(>=7个分支):

```
// val is string
switch (val)
{
    case "aaa":
        Func1();
        break;
    case "bbb":
        Func2();
        break;
    case "ccc":
        Func3();
        break;
    case "ddd":
        Func4();
        break;
    case "eee":
        Func5();
        break;
    case "fff":
        Func6();
        break;
    default:
        Func7();
        break;
}
```

接着看下对应的 IL 代码(部分):

```
// add { string, int } pairs to dictionary
ldc.i4.6
newobj instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::.ctor(int32)
dup
ldstr "aaa"
ldc.i4.0
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)
dup
ldstr "bbb"
ldc.i4.1
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)
dup
ldstr "ccc"
ldc.i4.2
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)
dup
ldstr "ddd"
ldc.i4.3
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)
dup
ldstr "eee"
ldc.i4.4
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)
dup
ldstr "fff"
ldc.i4.5
call instance void class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::Add(!0, !1)

// ...

// get string index from dictionary and do switch jump
call instance bool class [mscorlib]System.Collections.Generic.Dictionary`2<string, int32>::TryGetValue(!0, !1&)
brfalse.s IL_00ca

ldloc.2
// IL_009a case "aaa"
// IL_00a2 case "bbb"
// IL_00aa case "ccc"
// IL_00b2 case "ddd"
// IL_00ba case "eee"
// IL_00c2 case "fff"
switch (IL_009a, IL_00a2, IL_00aa, IL_00b2, IL_00ba, IL_00c2)

br.s IL_00ca // default
```

可以看到,相关的 switch 实现逻辑和我们之前所说的优化方法是一致的(细节上略有不同).

值得一提的是,不支持 switch 语句的脚本语言(譬如 Lua),也可以使用字典映射方法来实现类似的 switch 功能以提高代码执行效率.

**小结 : 多多使用 switch 语句吧**

#### 参考资料
- [C/C++中switch语句实现的介绍](https://zhuanlan.zhihu.com/p/38139553)
- [C/C++中switch语句实现更深入的介绍](https://www.codeproject.com/Articles/100473/Something-You-May-Not-Know-About-the-Switch-Statem)

