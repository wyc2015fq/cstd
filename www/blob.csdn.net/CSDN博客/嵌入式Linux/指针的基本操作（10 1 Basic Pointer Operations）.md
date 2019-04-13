
# 指针的基本操作（10.1 Basic Pointer Operations） - 嵌入式Linux - CSDN博客

2014年03月10日 17:31:17[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1106


[This section corresponds to K&R Sec. 5.1]
The first things to do with pointers are to declare a pointer variable, set it to point somewhere, and finally manipulate the value that it points to. A simple pointer declaration looks like this:
*对指针的第一步操作就是声明一个指针变量，让它指向某个地方，最后操作指针指向的值，一个简单的指针声明如下：*
int *ip;This declaration looks like our earlier declarations, with one obvious difference: that asterisk. The asterisk means thatip, the variable we're declaring,
 is not of typeint, but rather of type pointer-to-int. (Another way of looking at it is that*ip,
 which as we'll see is the value pointed to byip, will be anint.)
*这个声明看起来非常像我们之前的变量声明，但是有一个明显的不同之处，就是那个星号（*），帅帅的星号跟着ip，这个我们声明的ip变量，不是int类型，它是一个指向整型的指针。（用另一种方法来看这个表达式是，先看*p做一个整体，*p的类型是一个int类型，然后*p分开，把*看到是取地址符号，那ip就是一个指针，也即一个地址，*ip取这个地址的值，这个地址的值是一个int类型）*
We may think of setting a pointer variable to point to another variable as a two-step process: first we generate a pointer to that other variable, then we assign this new pointer to the pointer variable. We can say
 (but we have to be careful when we're saying it) that a pointer variable has a value, and that its value is ``pointer to that other variable''. This will make more sense when we see how to generate pointer values.
*我们会想设置一个指针指向另外一个变量要用两个步骤，首先，我们声明一个存放在其他地方的变量，然后我们把这个变量的地址赋值给这个指针。我们可以这样说（但必须注意我们的描述）指针变量保存有一个值，并且它的值是指向其他变量的。这会使得我们对如何生成一个指针变量更加直观。*
Pointers (that is, pointer values) are generated with the ``address-of'' operator&, which we can also think of as the ``pointer-to'' operator. We demonstrate this by declaring (and initializing) anintvariablei,
 and then settingipto point to it:
*Pointers这个指指针的值，通常我们会用到取地址符号（&），当然我们也可以把它认为是“pointer-to”操作。我们声明一个int变量i,然后把ip指向这个变量i.*
int i = 5;
	ip = &i;The assignment expressionip = &i;contains both parts of the ``two-step process'':&igenerates
 a pointer toi, and the assignment operator assigns the new pointer to (that is, places it ``in'') the variableip. Nowip``points
 to''i, which we can illustrate with this picture:
![](http://www.eskimo.com/~scs/cclass/notes/p1.gif)
*ip=&i;这个表达包含两个步骤，&i是一个指向i的指针，然后把新声明的指针ip指向这个地方，也即这个指针，这个地址。好了。现在ip就指向了i,我们可以用如下图示来表示：*
iis a variable of typeint, so the value in its box is a number, 5.ipis a variable
 of type pointer-to-int, so the ``value'' in its box is an arrow pointing at another box. Referring once again back to the ``two-step process'' for setting a pointer variable: the&operator
 draws us the arrowhead pointing ati's box, and the assignment operator=, with the pointer variableipon
 its left, anchors the other end of the arrow inip's box.
i是一个整型变量，所以在框框里的是一个数字5，ip是一个指向整型变量的指针，所以ip保存的值是一个地址，并且这个地址保存的是一个整型变量。重新回来看一下两步操作法：为了设置一个指针变量，&操作取得指向框框里的地址，然后是“=”操作符，把指针变量ip放在左边，最后重点是i的地址保存在了ip的框框里。
We discover the value pointed to by a pointer using the ``contents-of'' operator,*. Placed in front of a pointer, the*operator accesses the value pointed to by that pointer. In other words, ifipis
 a pointer, then the expression*ipgives us whatever it is that's in the variable or location pointed to byip. For example, we could write something like
*我们发现要取得指针指向的值用（*）操作符，放在指针的前面，（*）操作取得指针指向地址的值，换句话说，如果ip是一个指针，*ip表达式告诉我们ip这个地址的值保存的值是多少，例如，我们可以这样写一行代码：*
printf("%d\n", *ip);which would print 5, sinceippoints toi, andiis
 (at the moment) 5.
*当然这会打印5，由于ip指向i,i的值（此时）是5.*
(You may wonder how the asterisk*can be the pointer contents-of operator when it is also the multiplication operator. There is no ambiguity here: it is the multiplication operator when it sits between two
 variables, and it is the contents-of operator when it sits in front of a single variable. The situation is analogous to the minus sign: between two variables or expressions it's the subtraction operator, but in front of a single operator or expression it's
 the negation operator. Technical terms you may hear for these distinct roles areunaryandbinary: abinaryoperator applies to two operands, usually on either side of it, while aunaryoperator applies to a single
 operand.)
*你一定想知道（*）取地址值的操作，与此同时，他也是乘法操作符。在这里是没有一点模糊的，乘法操作要具有两个变量，当它作为取地址值操作符时，它放在变量的前面，它就像减号（-）一样，当它在两个变量或者堂之间时，它是减号的功能，当它在一个常量前面时，它被当作负号。这就像一个操作符具有两种功能，简单叫它作二次元吧。哈哈！*
The contents-of operator*does not merely fetch values through pointers; it can also*set*values through pointers. We can write something like
*取地址值符（*）不仅仅是取得地址的值，也可以通过这个方法来设置一个地址的值。像下面一样：*
*ip = 7;which means ``set whateverippoints to to 7.'' Again, the*tells us to go to the location
 pointed to byip, but this time, the location isn't the one to fetch from--we're on the left-hand sign of an assignment operator, so*iptells
 us the location to storeto. (The situation is no different from array subscripting expressions such asa[3]which
 we've already seen appearing on both sides of assignments.)
*这是把ip指向的值设置为7.再次，（*）符号告诉我们它是取得（ip）地址的值的。但是这次，这个地址不是一个取得的值，左边的表达式（*ip）是取得ip指向的位置。（这就像数组里面的a[3]取得数组里面第2个元素的值一样）*
The result of the assignment*ip = 7is thati's value is changed to 7, and the picture changes to:
**ip=7这个表达式的结果是把ip这个地址的值变为7.上面的图改成下面：*
![](http://www.eskimo.com/~scs/cclass/notes/p2.gif)
If we calledprintf("%d\n", *ip)again, it would now print 7.
*我们当然可以用打印来看到*ip的值。它会打印7.*
At this point, you may be wondering why we're going through this rigamarole--if we wanted to setito 7, why didn't we do it directly? We'll begin to explore that next, but first let's notice the difference
 between changing a pointer (that is, changing what variable it points to) and changing the value at the location it points to. When we wrote*ip = 7, we changed the value pointed to byip, but if we declare another variablej:
int j = 3;and writeip = &j;we've changedipitself. The picture now looks like this:
*指针，我们可能想知道为啥我们写这么多这么啰嗦。如果我们想把它设置成7.为什么我们不直接操作，我们下面会探讨这个，但是首先让我们注意改变指针的值和改变指向指向位置的值的不同之处。当我们写*ip=7我们是改变指针指向地址的值的。但是如果我们声明另一个变量j.*
*int j=3;*
*然后写*
*ip=&j;*
*我们改变了ip的值，如下图：*
![](http://www.eskimo.com/~scs/cclass/notes/p3.gif)*我自己试了一下。要这样写才会正确：如果不要int i=8;int *ip=&i;只写成int*ip;可能会造成ip指向的地方不合法，会出错。*
```python
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{	
	int i=8;
	int *ip=&i;
	*ip=7;
	printf("%d\n",*ip);
	return 0;
}
```

We have to be careful when we say that a pointer assignment changes ``what the pointer points to.'' Our earlier assignment
*我们一定要注意，当我们说一个指针指向改变的时候，“指针指向什么？”我们先前的分配是这样的。*
*ip = 7;changed the value pointed to byip, but this more recent assignment
*改变ip指向地址的值，但是我们大多数是这样声明的。*
ip = &j;has changed whatvariableippoints to. It's
 true that ``whatippoints to'' has changed, but this time, it has changed for a different reason. Neitheri(which is still 7) norj(which
 is still 3) has changed. (What has changed isip's value.) If we again call
*ip=&j通过改变ip来达到改变ip指向的值。它是让ip指向的值发生了改变，但是这时，它因为另一个原因发生了改变。不是i的值让ip发生了变化，我们可以再来一次打印。*
printf("%d\n", *ip);
this time it will print 3.
*这时候它会打印出3来。*
We can also assign pointer values to other pointer variables. If we declare a second pointer variable:
*我们也可以把指针赋值给另一个指针*
int *ip2;then we can say
*这时我们可以这样写*
ip2 = ip;Nowip2points whereipdoes; we've essentially made a ``copy'' of the arrow:
*这时ip2指向的地方也是ip指向的地址，它本质上只是一个复制操作*
![](http://www.eskimo.com/~scs/cclass/notes/p4.gif)
Now, if we setipto point back toiagain:
*现在我们把ip又改回i的地址*
ip = &i;the two arrows point to different places:
*这时两个指针指向了不同的地方*
![](http://www.eskimo.com/~scs/cclass/notes/p5.gif)
We can now see that the two assignments
*这时我们可以看到下面两个表达式*
ip2 = ip;and*ip2 = *ip;do two very different things. The first would makeip2again point to whereippoints (in
 other words, back toiagain). The second would store, at the location pointed to byip2, a copy of the value pointed to byip;
 in other words (ifipandip2still point toiandjrespectively)
 it would setjtoi's value, or 7.
*是两个不同的东西。第一个是把ip和ip2是同一个地址。第二种是保存的值，*ip2是ip2地址的值，*ip是ip地址的值，换言之(如果ip 和ip2仍然指向i和j),它会把i的值赋给j.*
It's important to keep very clear in your mind the distinction between*a pointer*and*what it points to*. The two are like apples and oranges (or perhaps oil and water); you can't mix them. You can't
 ``setipto 5'' by writing something like
*在里心里，指针和指针指向的值你一定要有一个明确的区分，他们两个就像苹果和橘子一样。你一定不能混肴他。你不能把指针赋值5.像下面一样，是错误的。*
ip = 5;			/* WRONG */5 is an integer, butipis a pointer. You probably wanted to ``setthe value pointed to byipto
 5,'' which you express by writing
*5是一个整型，但是ip是一个指针，你大概像把ip指向的值设置成5.你可以这样写。*
*ip = 5;Similarly, you can't ``see whatipis'' by writing
*你也不能通过这样来查看ip的值。*
printf("%d\n", ip);	/* WRONG */Again,ipis a pointer-to-int, but%dexpects
 anint. To printwhatippoints
 to, use
再说一次，ip是一个指向整型的指针，但是%d期望是整型的，打针ip指向的值，通过下面的打印。
printf("%d\n", *ip);
Finally, a few more notes about pointer declarations. The*in a pointer declaration is related to, but different from, the contents-of operator*. After we declare a pointer variable
*最后，一小点笔记关于指针声明，（*）在声明指针中，但是有区别于操作指针，在我们声明指针后*
int *ip;the expression*下面的表达式*ip = &isets whatippoints to (that is, which location it points to), while the expression
*指导ip指向的值设置为5*
*ip = 5sets the value of the location pointed to byip. On the other hand, if we declare a pointer variable and include an initializer:
*设置ip指向的值，另一方面，我们可以在声明的时候包含初始化指针的值。*
int *ip3 = &i;we're setting the initial value forip3, which is whereip3will point, so that initial
 value is a pointer. (In other words, the*in the declarationint *ip3 = &i;is not the contents-of operator, it's the indicator thatip3is
 a pointer.)
*在这里（*）不是说取得ip3指向的值，而是告诉编译器，ip3是一个指针，然后把这个指针初始化为i的地址*
If you have a pointer declaration containing an initialization, and you ever have occasion to break it up into a simple declaration and a conventional assignment, do it like this:
*你有一个指针声明如下，并用第二步来对他进行初始化*
int *ip3;
	ip3 = &i;Don't write
*但是不可以这样写*
int *ip3;
	*ip3 = &i;or you'll be trying to mix oil and water again.
*否则，你又把油和水弄乱了*
Also, when we write
*当然我们也可以这样写。*
int *ip;although the asterisk affectsip's type, it goes with the identifier nameip, not with
 the typeinton the left. To declare two pointers at once, the declaration looks like
**告诉编译器，后面的变量是一个指针，我们可以像下面一样来声明两个指针*
int *ip1, *ip2;Some people write pointer declarations like this:
*一些人这样来声明一个指针。*
int* ip;This works for one pointer, because C essentially ignores whitespace. But if you ever write
*它在声明一个指针是正确的，但是C本质上忽视空格，但是你绝不能这样写*
int* ip1, ip2;		/* PROBABLY WRONG */it will declare one pointer-to-intip1and oneplainintip2,
 which is probably not what you meant.
*他会声明一个指向整型的指针，和一个整型变量ip2.当然不是你想要的意思*



