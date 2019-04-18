# C语言指针类型重要性 - 心纯净，行致远 - CSDN博客





2018年06月23日 20:04:50[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：643








C语言中，带上类型运算是一个很好的习惯。

我们来看一道题目。




若定义 char a[3][3]={“ad”, “ce” , “fb”}, *s = (char *)a; 那么下列表达式语法正确，并且其值与 a[2][1]相等的表达式是_______。

  A．*(a+3) B．*(*a+5) C．s[2][1] D．*++s-2




答案是D

这题设计的挺巧妙，其中有个理解的重点： *s = (char *)a

其中s 这个指针指向什么？指向这个整个二维数组吗？肯定不是，不然答案就是C了。

那么试想我们能不能这么定义呢：





[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- *s = a

那么就会报出这样的错误：




[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- cannot convert 'char (*)[3]' to 'char*' in initialization  


这个问题我相信大家都明白怎么解决。那么为什么进行强制类型转换就能指向数组的首地址了呢？

我们先来看这样一个问题：



[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- int a, b;  
- a + b;  


我们应该怎么看待a+b？我们应该这样看这个表达式即两个int变量相加，而不应该单单看作两个变量相加。

我通过一个例子来讲解：





[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- int *p,a=1;  
-     p = &a;  
-     p++;  



这里如何理解p++？ 按照上面所说，运算应该带上类型。所以这里应该是p + sizeof(int),即地址加了4；

那么如果把p定义成char*型指针呢？如：



[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- char *p;  
- int a=256;  
- p = (char*)&a;  
- printf(“%d\n”, *p);  


那么输出的结果是什么呢？

结果是0。



[cpp][view plain](https://blog.csdn.net/jackmrzhou/article/details/54380949#)[copy](https://blog.csdn.net/jackmrzhou/article/details/54380949#)





- printf(“%d\n”, *(p + 1))；  

如果是上述输出呢？

结果是1。

我们来研究一下：int 在内存中存储占4个字节，所以十进制256用二进制表示应该是

00000000 00000000 00000001 00000000

每一个地址代表一个存储单元。所以如果我们用一个指向int的指针来指向a，那么这个指针其实是指向4个字节的，所以指针加一其实是加sizeof（int）。现在我们把一个char*型的指针指向被我们强制转换成char*型的a。而char在内存中以单个字节存储。那么虽然p存放的还是a的地址，但是p更精准的应该是指向a的第一个字节的内容。所以*p对应（int）a中的第一个字节 即00000000 ,所以输出0，*(p + 1)即*(p + sizeof(char))，对应的是（int）a中的第二个字节，即00000001，所以输出1.



这样理解上面的(char*)就简单多了。(char*)a将原先指向(char *[3])强制转换成(char *)，这样其实是缩小了步长（不知这么比喻是否合适），即原先指向3个字节强制转换成1个字节，那么p就能接纳数组的首地址了。那么*++s实际上指向的是a[0][1]，也就是“d”，而“d”-2 就是“b”。

所以养成带类型运算的习惯真的很重要。

下面是类似的一道题目：



若定义 

- chars[2][3]={“ab”, “cd”}, *p=(char*)s;

 那么下列表达式语法正确，并且其值与 s[1][1]相等的表达式（并非一定与其等价）是



A. *（s+3）

B. p[1][1]

C. *(p+3)

D. *++P+2

答案：D

分析：这道题很好。首先字符型指针p存放的是字符串s的首地址，而字符串s表示的是这样一个字符串：

ab_

cd_

为了便于理解，“_”表示空格，即没有元素。题中s[1][1]是d，所以要在ABCD中寻找表示s[1][1]的数。

先来看*（s+3），s表示数组首地址,不是指针型变量，所以s+3没有意义。p[1][1]同样由于p是指针，不存在这种表达。*（p+3）是是p表示的指针后移3位，因此*（p+3）是c。D选项有点复杂，*++p+2 这个比较有趣,p是一个char*指针,指向字符数组首地址,&s[0][0]。首先++的优先级更高执行++p得到的是&s[0][1],然后*运算符解引用,得到s[0][1]，s[0][1]的字符是'b',然后'b'+2,得到的就是'd'的ASCII码. 




