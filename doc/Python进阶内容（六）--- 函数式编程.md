# Python进阶内容（六）--- 函数式编程



 

斐波那契数列(Fibonacci)的递归与非递归实现

费波那契数列由0和1开始，之后的数就由之前的两数相加 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584,……….

#### 递归算法

用递归算法来求值,非常好理解.伪代码：

```
f(n) = 0                      (n=0)
f(n) = 1                      (n=1)
f(n) = f(n-1) + f(n-2)        (n>1)
```

代码实现：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
def f(n):
    if n==0:
        return 0
    elif n==1:
        return 1
    elif n>1:
        return f(n-1) + f(n-2)
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

#### 非递归算法

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
def f(n):
    if n == 0:
        return 0
    if n == 1:
        return 1
    if n>1:

        prev = 1    #第n-1项的值
        p_prev = 0  #第n-2项的值
        result = 1  #第n项的值

        for i in range(1,n):
           result = prev+p_prev 
           p_prev = prev
           prev = result
        return result
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

**Recursing 递归** 最大的好处就简化代码，他可以把一个复杂的问题用很简单的代码描述出来。注意：递归的精髓是描述问题，而这正是函数式编程的精髓。

 