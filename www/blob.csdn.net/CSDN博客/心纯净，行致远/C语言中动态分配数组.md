# C语言中动态分配数组 - 心纯净，行致远 - CSDN博客





2018年06月21日 12:57:42[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：12700标签：[动态数组																[C语言																[动态内存分配](https://so.csdn.net/so/search/s.do?q=动态内存分配&t=blog)
个人分类：[【C】																[【总结】](https://blog.csdn.net/zhanshen112/article/category/7192776)](https://blog.csdn.net/zhanshen112/article/category/7184374)





很多人在编写C语言代码的时候很少使用动态数组，不管什么情况下通通使用静态数组的方法来解决，在当初学习C语言的时候我就是一个典型的例子，但是现在发现这是一个相当不好的习惯，甚至可能导致编写的程序出现一些致命的错误。尤其对于搞嵌入式的人来所，嵌入式系统的内存是宝贵的，内存是否高效率的使用往往意味着嵌入式设备是否高质量和高性能，所以高效的使用内存对我们来说是很重要的。那么我们在自己编写C语言代码的时候就应该学会使用动态数组，这也就是我这篇博客要给大家讲的，我尽我所能的用一些简单的代码来讲解动态数组，希望我所讲的对你有所帮助。

那么我们首先来看看什么是动态数组，动态数组是相对于静态数组而言，从“动”字我们也可以看出它的灵活性，静态数组的长度是预先定义好的，在整个程序中，一旦给定大小后就无法改变。而动态数组则不然，它可以随程序需要而重新指定大小。动态数组的内存空间是从堆动态分配的。是通过执行代码而为其分配存储空间。当程序执行到我们编写的分配语句时，才为其分配。对于静态数组，其创建非常方便，使用完也无需释放，要引用也简单，但是创建后无法改变其大小是其致命弱点！对于动态数组，其创建麻烦，使用完必须由程序员自己释放，否则将会引起内存泄露。但其使用非常灵活，能根据程序需要动态分配大小。所以相对于静态数组的来说我们对于使用动态数组有很大的自由度。

在创建动态数组的过程中我们要遵循一个原则，那就是在创建的时候从外层往里层，逐层创建；而释放的时候从里层往外层，逐层释放。这个话你读了可能理解并不深刻，不过不要急，接下来我们看看两段代码。

一维动态数组的创建：

#include <stdio.h>
#include <stdlib.h>
int main()
{
int n1,i;
int *array;
printf("请输入所要创建的一维动态数组的长度：");
scanf("%d",&n1);
array=(int*)calloc(n1,sizeof(int));
for(i=0;i<n1;i++)
{
 printf("%d\t",array[i]);
}
printf("\n");
for(i=0;i<n1;i++)
{
 array[i]=i+1;
 printf("%d\t",array[i]);
}
 free(array);//释放第一维指针 
return 0;
}

运行结果为：

![](http://hi.csdn.net/attachment/201107/21/0_1311227550Aw1B.gif)

特此说明：在以后的运行结果部分，我均会附上文字结果，以防图片打开失败。

请输入所要创建的一维动态数组的长度：4
0       0       0       0
1       2       3       4       Press any key to continue

在此我使用的是calloc()函数来分配的，同时也使用两个for语句来打印数组元素，我们发现第一个打印输出的数组元素值均为0，在此也是为了加深读者对于calloc()函数的印象我特地使用了它来分配，如果对于calloc()、malloc()、realloc()函数的区别还是很清楚的读者可以去看看我的另外一篇博客------C语言的那些小秘密之内存分配。

二维数组的创建：

#include <stdio.h> 
#include <stdlib.h> 
int main() 
{ 
int n1,n2; 
int **array,i,j; 
printf("请输入所要创建的动态数组的第一维长度：");
scanf("%d",&n1);
printf("请输入所要创建的动态数组的第二维长度：");
scanf("%d",&n2); 
array=(int**)malloc(n1*sizeof(int*)); //第一维 
for(i=0;i<n1; i++) 
{ 
array[i]=(int*)malloc(n2* sizeof(int));//第二维 
}
for(i=0;i<n1;i++)
{
for(j=0;j<n2;j++) 
{ 
array[i][j]=i*n2+j+1; 
printf("%d\t",array[i][j]); 
} 
printf("\n");
}
for(i=0;i<n1;i++) 
{ 
free(array[i]);//释放第二维指针 
} 
free(array);//释放第一维指针 
return 0; 
}

运行结果为：

![](http://hi.csdn.net/attachment/201107/21/0_1311228373n12P.gif)

 请输入所要创建的动态数组的第一维长度：3
请输入所要创建的动态数组的第二维长度：3
1       2       3
4       5       6
7       8       9
Press any key to continue

有了上面的代码我们再来说动态数组的建立就简单了，以二维为例，先说创建，还记得我们上面说的创建的原则嘛：从外层往里层，逐层创建。

array=(int**)malloc(n1*sizeof(int*)); //第一维

以上是我们创建二维动态数组的最外层，创建好了最外层那么我们接下来就是要创建次外层了。

array[i]=(int*)malloc(n2* sizeof(int));//第二维

在创建次外层的过程中我们使用了一个for喜欢语句，千万别忘了使用for循环语句，这是绝大多数人的一个易错点。

创建好了接下来我们该讲到释放了，而释放的时候从里层往外层，逐层释放。刚刚与我们上面的创建相反，在以上代码中我们首先使用了下面一个for循环来释放里层。

for(i=0;i<n1;i++) 
{ 
free(array[i]);//释放第二维指针 
}

在通过以下语句来释放外层。
free(array);//释放第一维指针

如果出现多维的情况怎么做呢，我们接下来再来看看一个三维动态数组的创建和释放，以加深下读者的印象。代码如下：

#include <stdlib.h> 
#include <stdio.h> 
int main() 
{ 
int n1,n2,n3; 
int ***array; 
int i,j,k; 
printf("请输入所要创建的动态数组的第一维长度：");
scanf("%d",&n1); 
printf("请输入所要创建的动态数组的第二维长度：");
scanf("%d",&n2); 
printf("请输入所要创建的动态数组的第三维长度：");
scanf("%d",&n3); 
array=(int***)malloc(n1*sizeof(int**));//第一维 
for(i=0; i<n1; i++) 
{ 
array[i]=(int**)malloc(n2*sizeof(int*)); //第二维 
for(j=0;j<n2;j++) 
{ 
array[i][j]=(int*)malloc(n3*sizeof(int)); //第三维 
} 
} 
for(i=0;i<n1;i++)
{
for(j=0;j<n2;j++)
{
for(k=0;k<n3;k++) 
{ 
array[i][j][k]=i+j+k+1; 
printf("%d\t",array[i][j][k]); 
} 
printf("\n");
}
printf("\n");
}
for(i=0;i<n1;i++) 
{ 
for(j=0;j<n2;j++) 
{ 
free(array[i][j]);//释放第三维指针 
} 
} 
for(i=0;i<n1;i++) 
{ 
free(array[i]);//释放第二维指针 
} 
free(array);//释放第一维指针 
return 0; 
}

运行结果为：

![](http://hi.csdn.net/attachment/201107/21/0_1311231330a0bk.gif)


请输入所要创建的动态数组的第一维长度：3
请输入所要创建的动态数组的第二维长度：3
请输入所要创建的动态数组的第三维长度：3
1       2       3
2       3       4
3       4       5

2       3       4
3       4       5
4       5       6

3       4       5
4       5       6
5       6       7

Press any key to continue

看了以上三维动态数组的创建和释放代码以后，我想读者这个时候已经可以自己编写任意维的动态数组了。但是细心的读者可能发现了一个问题，那就是我们所讲的动态数组都是一次性创建好的，如果接下来在使用的过程中我们使用的数组需要扩展或者删减一些不再使用元素该怎么办呢？！接下来我们先看一段关于动态数组扩展的代码，在此以一维动态数组的扩展为例，其它的以此类推。

#include <stdio.h>
#include <stdlib.h>
int main()
{
int*n,*p;
int i,n1,n2;
printf("请输入所要创建的动态数组的长度：");
scanf("%d",&n1); 
n=(int*)calloc(n1,sizeof(int));
printf("请输入所要扩展的动态数组的长度：");
scanf("%d",&n2); 
p=(int*)realloc(n,(n2)*sizeof(int));//动态扩充数组
for(i=0;i<n2;i++)
{
p[i]=i+1;
if(i%5==0)
printf("\n");
printf("%d\t",p[i]);
}
free(p);
return 0;
}

运行结果如下：

![](http://hi.csdn.net/attachment/201107/21/0_1311233626WWC8.gif)

请输入所要创建的动态数组的长度：6
请输入所要扩展的动态数组的长度：25

1       2       3       4       5
6       7       8       9       10
11      12      13      14      15
16      17      18      19      20
21      22      23      24      25      Press any key to continue

看了上面的代码读者应该知道如何来扩展动态数组了，可能有的读者对于realloc()函数的使用有些陌生，如果有什么疑惑的话可以参考我之前写的一篇博文------C语言的那些小秘密之内存分配，在此我就不再做过多的讲解了。

接下来如何缩小动态数组。

#include <stdio.h>
#include <stdlib.h>
int main()
{
int*n,*p;
int i,n1,n2;
printf("请输入所要创建的动态数组的长度：");
scanf("%d",&n1); 
n=(int*)calloc(n1,sizeof(int));
for(i=0;i<n1;i++)
{
n[i]=i+1;
if(i%5==0)
printf("\n");
printf("%d\t",n[i]);
}
printf("\n请输入所要缩小的动态数组的长度：");
scanf("%d",&n2); 
p=(int*)realloc(n,(n2)*sizeof(int));
for(i=0;i<n2;i++)
{
if(i%5==0)
printf("\n");
printf("%d\t",p[i]);
}
printf("\n");
free(p);
return 0;
}

运行结果为：

![](http://hi.csdn.net/attachment/201107/21/0_1311234474ZTT4.gif)

请输入所要创建的动态数组的长度：25

1       2       3       4       5
6       7       8       9       10
11      12      13      14      15
16      17      18      19      20
21      22      23      24      25
请输入所要缩小的动态数组的长度：15

1       2       3       4       5
6       7       8       9       10
11      12      13      14      15
Press any key to continue

在这里值得注意的一点就是在缩减动态数组的时候，它是删除了后面的元素，而前面的元素保持不变。在使用realloc()函数的时候要由其注意它的使用规则。




讲到这儿就到了该说结束的时候了，由于本人水平有限，博客中的不妥或错误之处在所难免，殷切希望读者批评指正。同时也欢迎读者共同探讨相关的内容，如果乐意交流的话请留下你宝贵的意见。](https://so.csdn.net/so/search/s.do?q=C语言&t=blog)](https://so.csdn.net/so/search/s.do?q=动态数组&t=blog)




