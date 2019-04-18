# PAT乙级踩过的坑 - 心纯净，行致远 - CSDN博客





2018年10月24日 08:12:33[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：104








1、判断多个数字是否相等？

不能使用下面这样的代码：

```
if(a==b==c==d){
……
}
```

编译器看不懂这样人类的表示方法，只能每两个进行判断：

```
if(a==b&&b==c&&c==d){
……
}
```

2、DEVc++ 中scanf和printf 的double类型与int不同

```
int i;
scanf("%d",&i);
printf("%d",i);

double i;
scanf("%lf",&i);
printf("%lf",i);
```

3、数组定义之后必须初始化，否则会出现你想不到的数字。

```cpp
int sum[100001]={0};
```

4、动态数组如果不用malloc，根据用户输入确定数组大小的话。需要注意：

```
int n,i;
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++){
    scanf("%d",&a[i]);
}
```

如果有两个都需要赋值，其中一个又被另一个使用，必须分开赋值。同时赋值会出现错误。下面就是错误的。

```
int n,i,j;
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++){
    scanf("%d %d",&j,&a[j]);
}
```

应该先赋值给j,再赋值给a[j]。

```
int n,i,j;
scanf("%d",&n);
int a[n];
for(i=0;i<n;i++){
    scanf("%d",&j);
    scanf("%d",&a[j]);
}
```

















