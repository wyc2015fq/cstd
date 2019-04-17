# STL 的基本使用 —— unique() 的使用 - Alex_McAvoy的博客 - CSDN博客





2019年01月18日 21:49:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[STL 的使用——算法](https://blog.csdn.net/u011815404/article/category/8793970)









unique() 位于 <algorithm> 头文件中，其作用是元素去重，即删除序列中所有相邻的重复元素。

其删除元素，只是一个伪去除，只是重新排列了序列中的元素，将相邻的重复元素添加到容器末尾，简单来说，使用 unique() 后，容器的长度没有变，只是元素顺序改变了。

其返回值是去重之后的尾地址，如下，减去数组 a 的起始地址后所得到的就是 a 中去重后的元素个数。

```cpp
int a[10]={1,1,2,2,2,3,4,5,5,5};
int res=unique(a,a+10)-a;
```

由于其返回值的特性，因此在使用 unique() 前，一般会将目标序列进行排序。

```cpp
sort(a,a+n);//用unique前需要sort
res=unique(a,a+n)-a;//unique去重
```





