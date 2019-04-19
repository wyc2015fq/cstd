# Java 通过continue/break/标签  实现 goto - Simple 专栏 - CSDN博客
2012年01月31日 11:26:11[Simple_Zz](https://me.csdn.net/love284969214)阅读数：1272标签：[java																[Java基础																[Java面试](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=Java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
goto虽然是Java中保留的keyword，但是对于跳转这个语法对新手来说这个确实好用。为了提高程序的可靠性和可读性，Java语言目前是不支持无条件跳转的goto语句！！
幸亏Java中有高仿跳转的功能但是有个局限性**（该方法仅限于循环嵌套跳转到外层循环的情况）**
**首先了解一下****continue、break、标签 ，都是干嘛的。 **
```java
label1: 
for (;;) {      // 外部循环
    for (;;) {  // 内部循环
        // ...
        break;          // break中断内部循环，并在外部循环结束。
        // ...
        continue;       // continue 移回内部循环的起始处。
        // ...
        continue label1;// continue label1 却同时中断内部循环以及外部循环，并移至 label1 处。
                        // 随后，它实际是继续循环，但却从外部循环开始。
        // ...
        break label1;   // break label1 也会中断所有循环，并回到 label1 处。
                        // 但并不重新进入循环。也就是说，它实际是完全中止了两个循环。
    }
}
```
下面我再做一个例子，做进一步的了解，代码如下：
```java
private void testGoto() {
    outer: while (true) {
        go: for (int i = 0; i < 10; i++) {
            System.out.println("i值为：" + i);
            if (i == 2) {
                inter:
                for (int j = 0; i < 3; j++) {
                    System.out.println("j值为：" + j);
                    if (j == 1) {
                        System.out.println("j==1跳出inter");
                        break inter;
                    }
                }
            }
            if (i == 3) {
                System.out.println("i==3，继续循环go");
                continue go;
            }
            if (i == 4) {
                System.out.println("i==4，跳出outer");
                break outer;
            }
        }
    }
}
/** 输出结果为
    i值为：0
    i值为：1
    i值为：2
    j值为：0
    j值为：1
    j==1跳出inter
    i值为：3
    i==3，继续循环go
    i值为：4
    i==4，跳出outer
 */
```
