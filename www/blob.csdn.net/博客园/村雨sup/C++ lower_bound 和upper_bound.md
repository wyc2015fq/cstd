# C++ lower_bound 和upper_bound - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





二分查找的函数有 3 个：

1.lower_bound(起始地址，结束地址，要查找的数值) 返回的是数值 第一个 出现的位置。

2.upper_bound(起始地址，结束地址，要查找的数值) 返回的是数值 最后一个 出现的位置。

3.binary_search(起始地址，结束地址，要查找的数值)  返回的是是否存在这么一个数，是一个bool值。

举例：

数组 a(下标从1开始) : 1  2  3  3  3  4  5

查找 3 ： 

```
int position1 = lower_bound(a+1,a+n,3) - a; //position1 = 3
```

```
int position2 = upper_bound(a+1,a+n,3) - a; //position2 = 5
```



需要注意的是：如果数组中没有找到所求元素，函数就会返回一个假想的插入位置。

```
int a[6] = {0,1,2,4,5,7};
    int position1 = lower_bound(a+1,a+6,3) - a;
    int position2 = upper_bound(a+1,a+6,3) - a;
    cout << position1 << endl;
    cout << position2 << endl;
// position1 = position2 = 3
```

还有二分查找的首要条件是数列有序！











