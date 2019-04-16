# 递归函数及其R实现 - littlely_ll的博客 - CSDN博客





2016年12月27日 21:56:33[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2480








今天看了几个递归的小例子，觉得不错，自己就试着写了两个。先看看 
**斐波那契数列：**

```php
fibs <- function(n){
     if(n==1 | n==2){
         return(1)
     }
     else{
         return(fibs(n-1)+fibs(n-2))
     }
 }
 fibs(6) ##得到8
```

打印前20个:

```
for(i in 1:20){
     print(fibs(i))
 }
```

[1] 1 

[1] 1 

[1] 2 

[1] 3 

[1] 5 

[1] 8 

[1] 13 

[1] 21 

[1] 34 

[1] 55 

[1] 89 

[1] 144 

[1] 233 

[1] 377 

[1] 610 

[1] 987 

[1] 1597 

[1] 2584 

[1] 4181 

[1] 6765 
**阶乘函数：**
```php
fac <- function(n){
     if(n==0){
         return(1)
     }
     else{
         return(n*fac(n-1))
     }
 }
 fac(5) ##结果为120
```

**排序算法：**

```
a = c(3,1,4,7,2,5)
sorted <- function(a){
  if(length(a)==1){
    return(a)
    break
  }
  b = a[which.min(a)]
  a[which.min(a)] = a[1]
  a[1] = b
  return(c(a[1],sorted(a[-1]))) ##递归
}
sorted(a)
```

[1] 1 2 3 4 5 7

**翻转序列：**

```
char <- c('a','b','c','d')
rec_char <- function(x){
       if (length(x)==1) return(x)
       return(c(tail(x,1),rec_char(x[-length(x)])))
}
rec_char(char)
```

[1] “d” “c” “b” “a”




