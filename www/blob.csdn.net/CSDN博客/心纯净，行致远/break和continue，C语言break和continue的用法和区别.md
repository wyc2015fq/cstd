# break和continue，C语言break和continue的用法和区别 - 心纯净，行致远 - CSDN博客





2018年11月25日 11:01:31[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：59








break 语句很重要，用得非常多，初学者一定要掌握。continue 语句虽然没有 break 语句用得多，但也经常用到，而且不可或缺，需要用到 continue 的时候其他语句是无法替代的。


前面已经介绍过 break 语句，它不仅可以跳出“循环体”，还可以跳出 switch。但事实上，break 也只能用于这两种情况。break 语句不能用于循环语句和 switch 语句之外的任何其他语句中。


不管是 [for 循环](http://c.biancheng.net/view/172.html)，还是 [while 循环](http://c.biancheng.net/view/180.html)，或者是 do…while 循环，都可以用 break 跳出来，但是 break 只能跳出一层循环。当有多层循环嵌套的时候，break只能跳出“包裹”它的最里面的那一层循环，无法一次跳出所有循环。


同样，在多层 switch 嵌套的程序中，break 也只能跳出其所在的距离它最近的 switch。但多层 switch 嵌套实在是少见。


continue 的用法十分简单，其作用为结束本次循环，即跳过循环体中下面尚未执行的语句，然后进行下一次是否执行循环的判定。

## continue和break的区别

**continue 语句和 break 语句的区别是，continue 语句只结束本次循环，而不是终止整个循环。break 语句则是结束整个循环过程，不再判断执行循环的条件是否成立。而且，continue 只能在循环语句中使用，即只能在 for、while 和 do…while 中使用，除此之外 continue 不能在任何语句中使用。**


所以，再次强调：continue 不能在 switch 中使用，除非 switch 在循环体中。此时 continue 表示的也是结束循环体的本次循环，跟 switch 也没有关系。


例如，下面为使用switch模拟电梯运行的代码：

```cpp
# include <stdio.h>
    int main(void)
    {
    int val; //variable的缩写, “变量”的意思
    printf("请输入您想去的楼层:");
    while (1)
    {
    scanf("%d", &val);
    switch (val)
    {
    case 1:
    printf("1层开!\n");
    break; //跳出switch
    case 2:
    printf("2层开!\n");
    break; //跳出switch
    case 3:
    printf("3层开!\n");
    break; //跳出switch
    default:
    printf("该层不存在, 请重新输入:");
    continue; //结束本次while循环
    }
    break; //跳出while
    }
    return 0;
    }
```

输出结果是：

请输入您想去的楼层:4

该层不存在, 请重新输入:3

3层开!


此时在 default 中如果不使用 continue 的话，那么最后的“该层不存在，请重新输入：”仍然不起作用。它虽然能输出，但输出之后直接就执行后面的 break 了，用户仍然没有重新输入的机会。而加 continue 之后就会结束本次 while 循环，即后面的 break 语句本次就不会执行，而是直接进入 while 循环，这样用户就有“重新输入”的机会了。



