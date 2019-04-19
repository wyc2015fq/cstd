# C语言指针和数组基础 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [Hacker_YHJ](http://www.jobbole.com/members/ctcetl) 翻译。未经许可，禁止转载！
英文出处：[Dennis Kubes](http://t.cn/zQoXHQW)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【感谢[@Hacker_YHJ](http://weibo.com/HackerYangHJ) 的热心翻译。如果其他朋友也有不错的原创或译文，可以尝试[推荐给伯乐在线](http://blog.jobbole.com/tougao/)。】
关于C语言中指针和数组的争论就像是一场恶战。一方面，有些人觉得，所有人都必须承认指针与数组是不同的。而另一些人则认为数组被当成指针来处理，因此它们不应该有什么区别。这种现象让人迷惑。然而，这两种说法其实都是正确的。
数组不是指针，指针也不能说是数组。在C语言中，指针仅在内存中代表一个地址，而数组是许多连续的内存块，多个类型相似的元素存储在其中。更深入的解释，请参考我之前的博文《[C语言内存地址](http://blog.jobbole.com/44845/)》。在C语言的绝大多数情况下，数组被当作指针来处理，这也是使人困惑的地方。
### 数组表示法vs指针表示法
数组被当作指针来处理，具体指的下面两条：
- 数组名变量代表了数组中第一个元素的地址。它并不是一个指针，但却表现得像一个不能被修改的常指针一样。
- 程序在与数组交互的时候，用指针表示法代替数组表示法。
我们来看点代码吧。

C
```
// 初始化数组
int numbers[5] = {1,2,3,4,5};
// 标准数组表示法
int *ptr1 = numbers;
int val1 = numbers[0];
// 数组表示法取地址
int *ptr2 = &numbers[0];
int val2 = *(&numbers[0]);
// 指针加偏移表示法
int *ptr3 = numbers + 0;
int val3 = *(numbers + 0);
// 输出指针中的地址
printf("*ptr1 = %p\n", (void *)ptr1);
printf("*ptr2 = %p\n", (void *)ptr2);
printf("*ptr3 = %p\n", (void *)ptr3);
// 输出地址指向的int值
printf("val1 = %d\n", val1);
printf("val2 = %d\n", val1);
printf("val3 = %d\n", val1);
```
我们声明了一个包含5个int的数组，并将数组名变量numbers赋给了一个int指针，ptr1。numbers代表了这个数组第一个元素的地址，将其赋给ptr1正是把它当成了指针来使用。接着我们用数组表示法访问了第一个元素的值。
第二个例子中，我们用数组表示法取了数组中第一个元素的地址，之后我们用解引用第一个元素所在地址的方法访问了它。
第三个例子中，我们用指针运算将数组中第一个元素的地址赋值给ptr3，之后我们解引用相同的地址来得到它的值。
最后我们将所有存储在指针中的地址和所有在这些地址的int值输出到屏幕上。运行这段代码，你会得到类似下面的输出：

C
```
*ptr1 = 0x7fff6be1de60
 *ptr2 = 0x7fff6be1de60
 *ptr3 = 0x7fff6be1de60
 val1 = 1
 val2 = 1
 val3 = 1
```
所有值都是相同的。接下来再看看下面的代码：

C
```
// 初始化数组
int numbers[5] = {1,2,3,4,5};
int i = 0;
// 用数组表示法输出元素
for (i = 0; i < 5; i++ ) {
  int value = numbers[i];
  printf("numbers[%d] = %d\n", i, value);
}
// 用指针加偏移输出元素（真讨厌）
for (i = 0; i < 5; i++ ) {
  int value = *(numbers + i);
  printf("*(numbers + %d) = %d\n", i, value);
}
// 仅用一个指针输出元素
int *ptr = numbers;
for (i = 0; i < 5; i++ ) {
  int value = *ptr++;
  printf("%d, *ptr++ = %d\n", i, value);
}
```
运行它，你会得到以下输出：

C
```
numbers[0] = 1
numbers[1] = 2
numbers[2] = 3
numbers[3] = 4
numbers[4] = 5
*(numbers + 0) = 1
*(numbers + 1) = 2
*(numbers + 2) = 3
*(numbers + 3) = 4
*(numbers + 4) = 5
0, *ptr++ = 1
1, *ptr++ = 2
2, *ptr++ = 3
3, *ptr++ = 4
4, *ptr++ = 5
```
就像你看到的那样，所有过程得到了相同的结果。
数组表示法实际上就是指针运算。C语言标准只是将numbers[0]定义为*(numbers + 0)的语法糖。（译者注：语法糖，它意指那些没有给计算机语言添加新功能，而只是对人类来说更容易理解的语法。）无论何时，你写下一个数组表示法，比方说numbers[2]，都会被编译器转换为*(numbers + 2)。这里，numbers表示数组中第一个元素的地址，+2则表示用于指针运算的偏移量。
### 数组变量
我们已经展示了，数组常被当作指针来处理，而且对于C编译器而言数组表示法就是指针运算。一些人自然而然地就做出了这样的假设：既然数组能被当成指针，指针也应该能赋值给数组。这是不对的，数组名变量不能被改变。我们看看下面的代码吧。

C
```
// 初始化数组
 int numbers[5] = {1,2,3,4,5};
 int numbers2[5] = {6,7,8,9,0};
 int *ptr = numbers2;
// 这不能通过编译
 numbers = numbers2;
 numbers = &numbers2;
 numbers = ptr;
```
这段代码不能通过编译。试一试，你会得到以下输出。

C
```
incompatible types when assigning to type ‘int[5]’ from type ‘int *’
incompatible types when assigning to type ‘int[5]’ from type ‘int (*)[5]’
incompatible types when assigning to type ‘int[5]’ from type ‘int *’
```
虽然数组名变量代表了数组第一个元素的地址，它却表现得像一个不能被更改的常指针一样。它不能接受一个别的数组名变量或是指向另一个数组的指针的赋值。思考一下，如果你有一个数组名变量A代表一个数组，而且你能够改变A的地址，那被A指向的内存将会发生生么？
接下来看看这段能够编译的代码。

C
```
// 初始化数组
 int numbers[5] = {1,2,3,4,5};
 int numbers2[5] = {6,7,8,9,0};
 int *ptr1 = numbers;
 int *ptr2 = numbers2;
// 能通过编译
 ptr1 = ptr2;
// 输出地址
 printf("numbers = %p\n", (void *)numbers);
 printf("numbers2 = %p\n", (void *)numbers2);
 printf("ptr1 = %p\n", (void *)ptr1);
 printf("ptr2 = %p\n", (void *)ptr2);
 它会输出这样的结果：
numbers = 0x7fff5ea3d230
 numbers2 = 0x7fff5ea3d250
 ptr1 = 0x7fff5ea3d250
 ptr2 = 0x7fff5ea3d250
```
虽然不能直接改变数组名变量，我们仍然改变一个指向这个数组的指针。代码中，我们创建了两个数组，两个int指针。我们将numbers赋给了ptr1，将numbers2赋给了ptr2。接着我们将ptr2赋给了ptr1，最后输出结果。可以看到，ptr1和ptr2都指向了numbers2数组的第一个元素。
### 总结
我希望你们能够喜欢这篇对C语言中数组和指针的概述。我们没有囊括关于指针和数组的一切知识，但足以作为一个开始。跟往常一样，我非常愿意接受大家的评论和建议。
