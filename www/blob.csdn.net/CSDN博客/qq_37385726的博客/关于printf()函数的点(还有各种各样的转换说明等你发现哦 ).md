# 关于printf()函数的点(还有各种各样的转换说明等你发现哦~) - qq_37385726的博客 - CSDN博客





2017年03月04日 09:42:26[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：805








***一：在转换说明中指定精度输出***

（下面是引用的百度的回答~~）



![](https://gss0.baidu.com/9fo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=793d0b1cf31fbe091c0bcb125b50200b/1f178a82b9014a90f4f572d7a8773912b21bee94.jpg)

![](https://gss0.baidu.com/-vo3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=b6e679aaf636afc30e5937638329c7fc/4034970a304e251f2063d092a686c9177e3e53c7.jpg)

![](https://gss0.baidu.com/-4o3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=05de2065b90e7bec238f0be71f1e9500/472309f7905298224ed10da9d6ca7bcb0b46d4fd.jpg)

![](https://gss0.baidu.com/-Po3dSag_xI4khGko9WTAnF6hhy/zhidao/wh%3D600%2C800/sign=09def345bf096b63814c56563c03ab7c/8b82b9014a90f6037015a82d3812b31bb051ed36.jpg)


 从上面的几个结果，你可以看出来：


%m.ns:


m是整个输出的个数，比如上面的10（包括空格的输出）；

**//n是显示的个数；如果n大于m，按n的个数输出；n小于m，则前面输出空格（你是看不见空格的），后面输出字符，字符+空格的个数=m的值；**

**百度给我们的回答是这样的，但是更准确的来说，应该是先不看我规定的m,如果最后整个的输出结果大于m的话就按实际情况来判断，但是如果最后整个的输出结果小于m的话，那么就在前面用空格进行补充，是最后的精度达到我们规定的要求。**

**更简单的来说，m表示的是至少有的宽度，n表示的是至少有的精度。**

**如果是23.1415926   %4.3lf     那么输出结果就会是23.141，在小数点前面的数是通过这种方法改变不了的。**


s是字符的输出格式，整形的为d。即%m.nd

**注：**这中指定精度的输出还可以由另外一种方式完成




```
#include
int main(void)
{
	double a = 23.141592657;
	int m = 6, n =4 ;
	printf("%*.*lf", m, n, a);//不过这里还是要记得写lf不能写成d,不然就很尴尬啦==
	
	/*补充一个小知识==因为实在无处可放了*/
	printf("%.lf\n", a);//等价为%.0lf
	getchar();
	return 0;
}

//输出结果为23.1415
```





如上面的这个程序显示  （ ”%*.*lf“，m,n,a）则这种方法也是极好的~

***二：关于有危险已经被禁用的%n***



%n 可以将所输出字符串的长度值赋绐一个变量, 见下例:

int slen;

printf("hello world%n", &slen);

执行后变量被赋值为11。

又查了一下, 看到一篇文章(查看)说这种格式输出已经确认为一个安全隐患，并且已禁用。再搜搜果然这种用法都被用来搞什么溢出、漏洞之类的


***三:关于在不同编译器里输出结果可能”不一样“的%p***

%p格式用来以十六进制整数形式输出内存地址


============================================


printf函数族中对于%p一般以[十六进制](https://www.baidu.com/s?wd=%E5%8D%81%E5%85%AD%E8%BF%9B%E5%88%B6&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YkPWc4nhuhnjnsrAPhmhfL0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH6znWn4PWn3PW0YP1ckn1bYr0)整数方式输出指针的值，附加前缀0x。
        示例：
        int i = 1;
        printf("%p",&i);
        相当于
        int i = 1;
        printf("0x%x",&i);
        对于32位的指针，输出一般会是类似0xf0001234之类的结果。**==================================================****相信如果有人了解过就可能会查到这一种说法，但是在我的编译器上面的结果还是更符合下面这种说法，但是关于上面这种说法的对错程度，我不做判断，因为可能也确实存在有编译器的输出结果是这个样子的**%p 以16进制的形式输出内存地址,共8个字符，前2个为00 
%p 用来输出 地址 用，而不是用来输出 数值 用,输出中的ABCDEF 大写
例如：
int x=2; 
printf("%p",&x);
%x 用于输出无符号整数，默认，前面不加0。输出中的ABCDEF 小写





```
#include
int main(void)
{
	double a = 2333333.1;
	int m = 6, n = 5;
	printf("%x\n",&a);
	printf("%p\n", &a);
	getchar();
	return 0;
}


//输出结果为2afb04     002AFB04
```







***四：多个相连的字符串可以合并为一个字符串***



这个其实在这篇博客里面就说了http://blog.csdn.net/qq_37385726/article/details/59158411

这里就不细说了~~


**五：printf函数可以有条件的分行写出**




分行写出的条件是：①不在字符串内  ②不断开一个单词

使用的情况：在那些printf函数很长的时候，特别是影响观察，需要用滑杆拖动才能看到的时候，最后用这种方法来便于我们的观看。




```
#include
int main(void)
{
	int a = 3;
	printf("现在这么长还可以更长更长，长到你看不到后面是什么的时候，只能调滑杆的时候，这时候你突然想起了可以分行，于是a=%d",
		a);
	getchar();
	return 0;
}
```











**六：关于printf函数中逗号的作用**




之所以会觉得这是一个点，原因就是在于不理解，到底在printf函数里占了什么位置，起主要作用的到底是谁。




像一个函数的括号内的内容我们称为参数，而不同的参数之间是用“，”隔开的——这个就是，的唯一用处，仅仅是用来隔开参数的。这么说以后，我们就可以发现printf函数和scanf函数的一个共同的特点，就是它们的参数个数是可变的。但是这里又会有一个问题了，计算机必须要知道到底有几个参数才能正常工作，那么是怎么解决的呢？

这就取决于我们说的那个关键因素。

决定输出结果的就是第一个参数，因为像printf(),scanf()都是用第一个参数去告诉计算机，后面还有多少个参数，而告诉计算机的方式就是第一个参数内转换说明的数量，有一个，就还有一个参数，就再往后执行一个参数结束，有两个就还有两个参数，就再往后执行两个参数结束，没有就执行到第一个参数结束。

下面这个例子说明的就是第一个参数的关键作用，而逗号仅仅是炮灰而已~


```
#include
int main(void)
{
	int a = 3;
	printf("555", "%d", a);
	getchar();
	return 0;
}

//输出结果为：555
```








**七：printf函数中标记的作用**




```
#include
#include
#include
#include
#include
int main(void)
{
	char ch[20] = "jdbhjdsbj";
	double a = 2.33333333333;
	int b = -6666666;
	printf("      jdakfhdj\n");
	printf("%-20s", ch);//使用  -标记
	printf("2\n");
	printf("%20s", ch);
	printf("6\n");
	printf("a=%+.3lf\n", a);//使用  +标记
	printf("b=%+d\n", b);//使用  +标记
	printf("% lf\n", a);//使用  空格标记
	printf("%020lf\n", a);//使用 0标记
	printf("%#x\n", b);
	getchar();
	getchar();
	return 0;
}
```













