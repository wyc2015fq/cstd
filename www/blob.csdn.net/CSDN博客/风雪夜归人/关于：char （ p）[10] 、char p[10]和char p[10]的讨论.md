# 关于：char （*p）[10] 、char p[10]和char *p[10]的讨论 - 风雪夜归人 - CSDN博客
2017年09月23日 14:52:33[cdjccio](https://me.csdn.net/qq_34624951)阅读数：346
直观理解：
        1.  char（*p）[10] 是指向数组的指针。
        2. char p [10]是一个数组p。
        3. char *p [10] 是指针数组，数组元素是char *指针。
具体的不同点：
**1.指向数组的指针**
         char（*p）[10] 中，p指向的是一个10个char大小的数组，p+1 和 p 的差距是 sizeof（char p [10]），p+1已指到另一个相同大小的数组了。
** 2.数组指针**
         char p[10] 中，p也是一个指针，不过是常量指针，指向的是数组的第一个元素的地址（不是数组的首地址），见如下代码：
**[cpp]**[view
 plain](http://blog.csdn.net/oyyj42/article/details/39925099#)[copy](http://blog.csdn.net/oyyj42/article/details/39925099#)
[print](http://blog.csdn.net/oyyj42/article/details/39925099#)[?](http://blog.csdn.net/oyyj42/article/details/39925099#)
- #include <stdio.h>
- int main()  
- {  
- char (*p)[10] ;  
- char a[10];  
- p=a;  
- }  
为什么提示
[root@localhost ~]# cc test.c
test.c: 在函数 ‘main’ 中：
test.c:6: 警告：从不兼容的指针类型赋值呢?
你把p=a 改成 p=&a就可以了。
这是c语言迷惑人的地方。很多书都告诉大家数组名就是数组的地址，但其实它是的数组第一个元素的地址，类型是char *。而&a 表示的才是数组的地址，类型是char (*)[10]。由于数组的地址在值上面和数组第一个元素地址的值是相同的，所以大家平时就认为&a和a是一样的，产生了误会。
所以，p=a的警告在于你把一个char型的地址赋给了一个char (*)[10]的指针。当然，这样做的结果没错，不过多了个警告。
**3.指针数组**
        char * p[10],表示的是一个数组，数组中每个元素都是指针，这里sizeof(p)在32位机器上应该是40，=10 * sizeof(char*)，这儿是数组的长度，而不是指针的长度，也说明他是个数组，跟指向指针的指针（char **p）也是不一样的
**此外：**
**[cpp]**[view
 plain](http://blog.csdn.net/oyyj42/article/details/39925099#)[copy](http://blog.csdn.net/oyyj42/article/details/39925099#)
[print](http://blog.csdn.net/oyyj42/article/details/39925099#)[?](http://blog.csdn.net/oyyj42/article/details/39925099#)
- char *p；  
- char a[10]；  
- p = a；  
这里p = a的意思是p指向a[0]的地址，p指向的是一个char。
若改成p = &a ，则表示p指向数组a，p此时指向的是一个a[10]数组！
p 仅仅是指向一个元素，而&a是整个数组的地址（&a实际上是指向数组a的指针），一个元素是不能指向整个数组的，因为它们类型不一致，编译器会警告！！！
虽然编译时会提醒警告：不兼容的指针类型，但还是能运行，实际上这是不对的。
正确的代码应该是：
int main()
{
    char (*p)[12] ; // p的类型是 char (*)[12]
    char a[12];  // a 的类型是 char
    char *p1 = "aaddffgg";
    strcpy(a,"hello,world");
    p=&a;  // &a 表示数组a的地址，类型是 char (*)[12]
    printf("p=%s\n "
    "the second char in p is %c\n",*p,(*p)[1]);
    printf("%s \n",p);// 警告  类型错误
    printf("%p \n",p);// OK 打印p指针的地址
    printf("%s \n",*p);// p是指向数组的指针，所以使用*p间接引用操作符
    printf("%s \n",p1);// OK 原因是p1指向的是字符串的元素首地址
    return 0;
}
