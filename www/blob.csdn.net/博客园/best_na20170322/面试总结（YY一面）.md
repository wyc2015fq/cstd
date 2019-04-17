# 面试总结（YY一面） - best_na20170322 - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/believe-in-me/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/best_na20170322)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/believe-in-me/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [面试总结（YY一面）](https://www.cnblogs.com/believe-in-me/p/6607208.html)





YY(欢聚时代)bingo部门

(一面   2017-03-23下午四点)

面试官语气挺好的，只不过上来就是问问题。开始之前先问了我能够实习多长时间（我说到九月），以及为什么选择windows程序开发以及后台开发（我说只做过这个啊）。

首先是问了我一个字节对齐的问题，类中有一个字符和一个整型，问类的大小是多少（这时，我问他系统是32还是64，以及有没有明确要求按几字节对齐），如果类中什么也没没有呢？

这类问题注意一下几点：

（1.类的大小为类的非静态成员数据的类型大小之和，也 就是说静态成员数据不作考虑。

（2.普通成员函数与sizeof无关。

（3.虚函数由于要维护在虚函数表，所以要占据一个指针大小，也就是4字节。

（4.类的总大小也遵守类似class字节对齐的，调整规则。

（5.因为一个空类也要实例化，所谓类的实例化就是在内存中分配一块地址，每个实例在内存中都有独一无二的地址。同样空类也会被实例化，所以编译器会给空类隐含 的添加一个字节，这样空类实例化之后就有了独一无二的地址了。所以空类的sizeof为1。





第二个问题是排序，你知道的排序有哪些，plusplusplus，说了一通，然后面试官说那你给我说说快排的大致过程是怎样的吧，我说了后他又问怎么优化呢，我说从标志位的选择上，随机的方法应该好一些；

快排的优化：http://blog.csdn.net/wangjin123456789/article/details/50772220

第三个问题是：给你一个字符串如何找出第一次只出现一次的字符；

接下来是网络的部分:

一个HTTP的完整流程：

（1.打开HTTP连接。因为HTTP是一种无状态协议，所以每个请求都需要建立一个新连接。

（2.初始化方法请求。包含一些类型的方法提示符，它们用来描述调用什么方法和需要什么参数。

（3.设置HTTP请求头。包含要传输的数据类型和数据长度。

（4.发送请求。即将二进制流写入服务器。

（5.读取请求。调用目标servlet程序，并接受HTTP请求数据。如果该次请求为客户端第一次请求，则需要建立一个新的服务器对象实例。

（6.调用方法。提高服务器端调用对象的方法。

（7.初始化响应方法。如果调用的方法出现异常，客户将会收到出错信息；否则，发送返回类型。

（8.设置HTTP响应头。响应头中设置待发送的数据类型和长度。

（9.发送响应。服务器端发送二进制数据流给客户端作为响应。

（10.关闭连接。当响应结束后，与服务器必须断开连接，以保证其他请求能够与服务器建立连接。

DNS：



那么我们的DNS是怎么解析一个域名的呢？







1.现在我有一台计算机，通过ISP接入了互联网，那么ISP就会给我分配一个DNS服务器，**这个DNS服务器不是权威服务器**，而是相当于一个代理的dns解析服务器，他会帮你迭代权威服务器返回的应答，然后把最终查到IP返回给你。



2.现在的我计算机要向这台ISPDNS发起请求查询www.baidu.com这个域名了，(经网友提醒：这里其实准确来说不是ISPDNS，而应该是用户自己电脑网络设置里的DNS，并不一定是ISPDNS。比如也有可能你手工设置了8.8.8.8)



3.ISPDNS拿到请求后，先检查一下自己的缓存中有没有这个地址，有的话就直接返回。这个时候拿到的ip地址，会被标记为**非权威服务器的应答**。



4.如果缓存中没有的话，ISPDNS会从**配置文件**里面读取13个根域名服务器的地址（这些地址是不变的，直接在BIND的配置文件中），



5.然后像其中一台发起请求。



6.根服务器拿到这个请求后，知道他是com.这个顶级域名下的，所以就会返回com域中的NS记录，一般来说是13台主机名和IP。



7.然后ISPDNS向其中一台再次发起请求，com域的服务器发现你这请求是baidu.com这个域的，我一查发现了这个域的NS，那我就返回给你，你再去查。



（目前百度有4台baidu.com的顶级域名服务器）。



8.ISPDNS不厌其烦的再次向baidu.com这个域的权威服务器发起请求，baidu.com收到之后，查了下有www的这台主机，就把这个IP返回给你了，



9.然后ISPDNS拿到了之后，将其返回给了客户端，并且把这个保存在高速缓存中。

然后问我会编译原理吗，我说大学的时候学了，不过都忘的差不多了。

然后问我const修饰指针时放的位置不同有什么差异，如果在前面再加一个static呢？staic int *p2; /*p2是指向静态整型变量的指针变量

还问了函数指针的原理：



**   定义**



每一个函数都占用一段内存单元，它们有一个起始地址，指向函数入口地址的指针称为函数指针。



**   语法**



指向函数的指针变量的一般定义形式为：



数据类型 (*指针变量名)(参数表);

**   说明**



1） 函数指针的定义形式中的数据类型是指函数的返回值的类型。



2） 区分下面两个语句：



int (*p)(int a, int b); //p是一个指向函数的指针变量，所指函数的返回值类型为整型



int *p(int a, int b); //p是函数名，此函数的返回值类型为整型指针



3） 指向函数的指针变量不是固定指向哪一个函数的，而只是表示定义了一个这样类型的变量，它是专门用来存放函数的入口地址的；在程序中把哪一个函数的地址赋给它，它就指向哪一个函数。



4） 在给函数指针变量赋值时，只需给出函数名，而不必给出参数。



如函数max的原型为：int max(int x, int y); 指针p的定义为：int (*p)(int a, int b); 则p = max;的作用是将函数max的入口地址赋给指针变量p。这时，p就是指向函数max的指针变量，也就是p和max都指向函数的开头。



5） 在一个程序中，指针变量p可以先后指向不同的函数，但一个函数不能赋给一个不一致的函数指针（即不能让一个函数指针指向与其类型不一致的函数）。



如有如下的函数：int fn1(int x, int y); int fn2(int x);



定义如下的函数指针：int (*p1)(int a, int b); int (*p2)(int a);



则



p1 = fn1; //正确



p2 = fn2; //正确



p1 = fn2; //产生编译错误



6） 定义了一个函数指针并让它指向了一个函数后，对函数的调用可以通过函数名调用，也可以通过函数指针调用（即用指向函数的指针变量调用）。



如语句：c = (*p)(a, b); //表示调用由p指向的函数(max)，实参为a,b，函数调用结束后得到的函数值赋给c。



7） 函数指针只能指向函数的入口处，而不可能指向函数中间的某一条指令。不能用*(p+1)来表示函数的下一条指令。



8） 函数指针变量常用的用途之一是把指针作为参数传递到其他函数。

### 妙用函数指针一： 函数指针数组



**[cpp]**[view plain](http://blog.csdn.net/touch_2011/article/details/6984029#)[copy](http://blog.csdn.net/touch_2011/article/details/6984029#)



- <span style="font-size:16px;">void main()  
- {  
- int i;  
- void (*p[])()={Touch,DuanJiong,MeiKai,YinJun,JiangHaiLong};  
-     scanf("%d",&i);  
-     p[i]();  
- }  
- </span>  




void (*p[])()={Touch,DuanJiong,MeiKai,YinJun,JiangHaiLong};声明了一个函数指针数组并赋值。把每个函数的入口地址存入这个数组，这样就不需要用switch语句了，根据下标i直接找到函数入口，省去了判断的时间。

### 妙用函数指针二： 回调函数

什么是回调函数，来着百度百科的解释：回调函数就是一个通过函数指针调用的函数。如果你把函数的指针(地址)作为参数传递给另一个函数，当这个指针被用为调用它所指向的函数时，我们就说这是回调函数。这里函数指针是作为参数传递给另一个函数。

- <span style="font-size:16px;">//回调函数对多种数据类型数组进行冒泡排序  
- //a表示待排序数组  
- //n表示数组长度  
- //size表示数组元素大小(即每个数组元素占用的字节数)  
- //int (*compare)(void *,void *) 声明了一个函数指针，在此作为参数  
- //void *类型的指针表示指向未知类型的指针,编译器并不会给void类型的指针分配空间,但我们可以把它进行强制类型转换  
- void bubbleSort(void *a,int n,int size,int (*compare)(void *,void *))  
- {  
- int i,j,k;  
- char *p,*q;  
- char temp;//交换时暂存一个字节的数据  
- for(i=0;i<n;i++)  
- for(j=0;j<n-i-1;j++){  
- //注意p,q都是字符类型的指针，加一都只移动一个字节  
-             </span><span style="font-size:16px;"><span style="color:#cc0000;">p=(char*)a+j*size;  
- </span>         </span><span style="font-size:16px;"><span style="color:#cc0000;">q=(char*)a+(j+1)*size;  
- </span>         if(compare(p,q)>0){  
- //一个一个字节的交换,从而实现了一个数据类型数据的交换  
- for(k=0;k<size;k++){  
-                     temp=*p;  
-                     *p=*q;  
-                     *q=temp;  
-                     </span><span style="font-size:16px;"><span style="color:#cc0000;">p++;  
- </span>                 </span><span style="font-size:16px;"><span style="color:#cc0000;">q++;  
- </span>             }  
-             }  
-         }  
- }</span>  

![](http://static.blog.csdn.net/images/save_snippets.png)


请注意代码中红色部分代码，要看懂这段代码需明确两个问题：（1）void*类型的指针未分配空间的，我们可以把它进行强制类型转换成char*。（2）对数组元素进行交换时，并不是一次就把两个数交换了，因为我们并不知道数据的确切类型。但知道数组元素的大小，这样就可以逐个字节进行交换。比如对int类型（占用四个字节）的值a、b进行交换，先交换a、b的第一个字节，然后第二个字节...

理解了这个代码，该怎么用呢？参数要传入一个函数指针，于是必须要写一个比较两个数大小的函数，且函数原型必须与int (*compare)(void *,void *)相匹配。下面是测试各种类型数组排序的代码：





**[cpp]**[view plain](http://blog.csdn.net/touch_2011/article/details/6984029#)[copy](http://blog.csdn.net/touch_2011/article/details/6984029#)



- <span style="font-size:16px;">#include<stdio.h>  
- 
- typedef struct{  
- int data;  
- }Node;  
- 
- //函数声明  
- int charCompare(void *a,void *b);  
- int intCompare(void *a,void *b);  
- int floatCompare(void *a,void *b);  
- int doubleCompare(void *a,void *b);  
- int nodeCompare(void *a,void *b);  
- void bubbleSort(void *a,int n,int size,int (*compare)(void *,void *));  
- 
- //比较两个char类型的数据的大小，a>b返回1，a<b返回-1，a==b返回0  
- int charCompare(void *a,void *b)  
- {  
- if(*(char*)a==*(char*)b)  
- return 0;  
- return *(char*)a>*(char*)b?1:-1;  
- }  
- //比较两个int类型的数据的大小  
- int intCompare(void *a,void *b)  
- {  
- if(*(int*)a==*(int*)b)  
- return 0;  
- return *(int*)a>*(int*)b?1:-1;  
- }  
- //比较两个float类型的数据的大小  
- int floatCompare(void *a,void *b)  
- {  
- if(*(float*)a==*(float*)b)  
- return 0;  
- return *(float*)a>*(float*)b?1:-1;  
- }  
- //比较两个double类型的数据的大小  
- int doubleCompare(void *a,void *b)  
- {  
- if(*(double*)a==*(double*)b)  
- return 0;  
- return *(double*)a>*(double*)b?1:-1;  
- }  
- //比较两个结构体类型（Node）的数据的大小  
- int nodeCompare(void *a,void *b)  
- {  
- if(((Node*)a)->data == ((Node*)b)->data)  
- return 0;  
- return ((Node*)a)->data > ((Node*)b)->data ? 1 : -1;  
- }  
- 
- void main()  
- {  
- int i=0;  
- //用于测试的各种类型数组  
- char c[]={'d','a','c','e','b'};  
- int a[]={3,2,4,0,1};  
- float f[]={4.4,5.5,3.3,0,1};  
- double b[]={4.4,5.5,3.3,0,1};  
-     Node n[]={{2},{0},{1},{4},{3}};  
- 
- //对各种数组进行排序  
-     puts("对char类型数组进行排序：");  
-     bubbleSort(c,5,sizeof(char),charCompare);  
- for(i=0;i<5;i++)  
-         printf("%c ",c[i]);  
-     puts("");  
- 
-     puts("对int类型数组进行排序：");  
-     bubbleSort(a,5,sizeof(int),intCompare);  
- for(i=0;i<5;i++)  
-         printf("%d ",a[i]);  
-     puts("");  
- 
-     puts("对float类型数组进行排序：");  
-     bubbleSort(f,5,sizeof(float),floatCompare);  
- for(i=0;i<5;i++)  
-         printf("%.2f ",f[i]);  
-     puts("");  
- 
-     puts("对double类型数组进行排序：");  
-     bubbleSort(b,5,sizeof(double),doubleCompare);  
- for(i=0;i<5;i++)  
-         printf("%.2lf ",b[i]);  
-     puts("");  
- 
-     puts("对结构体（Node）类型数组进行排序：");  
-     bubbleSort(n,5,sizeof(Node),nodeCompare);  
- for(i=0;i<5;i++)  
-         printf("%d ",n[i].data);  
-     puts("");  
- }  
- 
- 
- //回调函数对多种数据类型数组进行冒泡排序  
- //a表示待排序数组  
- //n表示数组长度  
- //size表示数组元素大小(即每个数组元素占用的字节数)  
- //int (*compare)(void *,void *) 声明了一个函数指针，在此作为参数  
- //void *类型的指针表示指向未知类型的指针,编译器并不会给void类型的指针分配空间,但我们可以把它进行强制类型转换  
- void bubbleSort(void *a,int n,int size,int (*compare)(void *,void *))  
- {  
- int i,j,k;  
- char *p,*q;  
- char temp;//交换时暂存一个字节的数据  
- for(i=0;i<n;i++)  
- for(j=0;j<n-i-1;j++){  
- //注意p,q都是字符类型的指针，加一都只移动一个字节  
-             p=(char*)a+j*size;  
-             q=(char*)a+(j+1)*size;  
- if(compare(p,q)>0){  
- //一个一个字节的交换,从而实现了一个数据类型数据的交换  
- for(k=0;k<size;k++){  
-                     temp=*p;  
-                     *p=*q;  
-                     *q=temp;  
-                     p++;  
-                     q++;  
-                 }  
-             }  
-         }  
- }</span>




类的静态成员函数和实例函数有什么区别，在什么时候创建的？静态成员和非静态成员？



（1）所有的函数都是存放在代码区的,不管是全局函数，还是成员函数。要是成员函数占用类的对象空间,那么将是多么可怕的事情:定义一次类对象就有成员函数占用一段空间。 我们再来补充一下静态成员函数的存放问题吧：静态成员函数与一般成员函数的唯一区别就是没有this指针，因此不能访问非静态数据成员，就像我前面提到的，所有函数都存放在代码区，静态函数也不例外。所有有人一看到 static 这个单词就主观的认为是存放在全局数据区，那是不对的

（2）类中的静态成员变量是类的所有对象都共用的成员变量。分配在内存中的数据区。
要用“类名+静态变量名”来访问。
不管new多少个对象都共用这个固定的变量，即使不new对象，也会存在于数据区内，也可以使用。



静态变量存储在静态存储区，程序启动时就分配空间，程序退出时释放。
普通成员变量在类实例化时分配空间，释放类的时候释放空间，存储在栈或堆中。














