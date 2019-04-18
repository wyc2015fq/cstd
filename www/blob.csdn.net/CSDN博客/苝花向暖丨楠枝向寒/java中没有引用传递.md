# java中没有引用传递 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月19日 09:14:44[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：76标签：[值传递和引用传递](https://so.csdn.net/so/search/s.do?q=值传递和引用传递&t=blog)
个人分类：[参数传递](https://blog.csdn.net/weixin_40247263/article/category/7518737)


  1说明:转自https://www.cnblogs.com/kaililikai/p/5885744.html
  2  3今天在论坛上有人提了一个关于java中调用函数时有没有引用传递的问题,可谓是吵的不可开交。有人说java只有值传递,也有人说java既有值传递也有引用传递,那么java中到底有没有引用传递呢，下面我来分析一下。
  4  5 一、首先来明确一下"值传递"和"引用传递的"区别
  6  7值传递：是对所传递参数进行一次副本拷贝，对参数的修改只是对副本的修改，函数调用结束，副本丢弃，原来的变量不变(即实参不变)
  8引用传递：参数被传递到函数时，不复制副本，而是直接将参数自身传入到函数，函数内对参数的任何改变都将反映到原来的变量上。
  9二、java中引用的含义
 10 11 C++和java中都有引用的概念，但在这两种语言中却有完全不同的含义。C++中我们可以用形如"int &b=a”的形式来定义变量a的一个引用b，b只是a的一个别名，b和a在内存中占同一个存储单元，利用引用机制我们可以在调用函数时实现值的双向传递——即引用传递，看下面代码:
 12 13示例一
 14 15 #include <iostream>
 16 17using namespace std;
 18 19int main()
 20 21{
 22 23void swap(int &,int &);
 24 25int i=3,j=5;
 26 27  swap(i,j);
 28 29   cout<<"i="<<i<<"j="<<j<<endl;
 30 31return 0;
 32 33}
 34 35 36 37void swap(int &a,int &b)
 38 39{
 40 41int temp;
 42 43   temp=a;
 44 45   a=b;
 46 47   b=temp;
 48 49}
 50 51 执行上面的程序输出的是i=5 j=3，a和b传递给swap()函数的时候，是传递的他们本身的地址，不是他们的拷贝，所以在函数中对他们的改变可以直接影响到实参a和b，这就是引用传递。
 52 53 java中的引用更像C++中的指针，当我们定义一个对象时(比如Person p=new Person())，定义的对象实例会放到java堆中，而变量p(即引用)会放到java栈中，p指向堆中的Person对象实例。
 54 55三、对引用传递的认识误区
 56 57为什么有很多人认为java有引用传递呢？一种情况是有人认为调用函数时其参数有可能是引用(如上面的p)，所以java有引用传递，这部分人对引用传递根本没有正确的认识；而另一种情况看似有道理，但是仔细分析也是不正确的的，他们往往会用如下的代码来证明他们的观点:
 58 59实例二：
 60 61复制代码
 62class Demo{  
 63int a;  
 64public Demo(int a){  
 65this.a=a;  
 66     }  
 67 }  
 68publicclass TestQuote{  
 69publicstaticvoid main(String args[]){  
 70          Demo d1=new Demo(1);  
 71          Demo d2=new Demo(2);  
 72         System.out.println(d1.a);  
 73         System.out.println(d2.a);  
 74         function(d1,d2);  
 75         System.out.println(d1.a);  
 76         System.out.println(d2.a);  
 77     }  
 78privatestaticvoid function(Demo d1,Demo d2){  
 79int a;  
 80          a=d1.a;  
 81          d1.a=d2.a;  
 82          d2.a=a;  
 83     }  
 84 }  
 85复制代码
 86 他们的观点如下:执行上面的代码，调用function()函数以前输出的结果是1、2，调用function()函数之后输出的结果会是2、1，可见在函数内对d1和d2的改变反映到了原来的变量上，要不是不会输出2、1的。
 87 88这种解释是很迷惑人的，看上去好像很正确，下面的代码会很好的反驳上面的观点:
 89 90示例三:
 91 92复制代码
 93class Demo{  
 94int a;  
 95public Demo(int a){  
 96this.a=a;  
 97     }  
 98 }  
 99publicclass TestQuote{  
100publicstaticvoid main(String args[]){  
101          Demo d1=new Demo(1);  
102          Demo d2=new Demo(2);  
103         System.out.println(d1.a);  
104         System.out.println(d2.a);  
105         function(d1,d2);  
106         System.out.println(d1.a);  
107         System.out.println(d2.a);  
108     }  
109privatestaticvoid function(Demo d1,Demo d2){  
110         Demo temp;  
111          temp=d1;  
112          d1=d2;  
113          d2=temp;  
114     }  
115 } 
116复制代码
117 执行上面的代码，调用function()前后程序输出的都是1、2，此程序试图通过调用function()交换d1和d2，但是没有成功，为什么呢？因为d1和d2是值传递，function()中的d1和d2是main()函数中d1和d2的副本，调用完function()不会对main()中的变量产生影响。再看示例二中，function()函数内改变的并不是d1和d2本身的值，而是d1和d2指向的对象的值，调用完function()后d1和d2仍然指向函数调用前的堆地址，即函数参数是栈中的d1和d2，而不是堆中d1和d2指向的对象，即使你在函数中改变了堆中的对象，但没有改变函数参数的值。所以示例二并不是什么引用传递；可见java中只有值传递。
118119120121 但是网上有很多针对"java值传递和引用传递进行的区别"的文章，如果读者看到的话一定要清楚，其中所说的引用传递是不正确的，他们所说的引用传递就是示例二中的那种情况。不幸的是，网上有很多文章都把示例二中的那样的传递看做引用传递，如果读者看到知道是什么意思就行了。

