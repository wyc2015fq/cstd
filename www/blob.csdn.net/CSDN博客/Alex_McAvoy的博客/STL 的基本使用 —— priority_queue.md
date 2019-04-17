# STL 的基本使用 —— priority_queue - Alex_McAvoy的博客 - CSDN博客





2019年01月13日 16:10:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72
个人分类：[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)









# 【概述】

priority_queue（优先队列）是定义在 <queue> 头文件中的一个模板类，与队列相比，优先队列不是按照入队顺序出队，而是按照队列中元素的优先权出队。默认情况下，按照大者优先的顺序出队，也可以指定算子来指定所需的优先顺序。

# 【定义】

定义：** pair<elemType, conType, greater/less> pairName**

参数：第一个是元素类型，第二个是容器类型，第三个是比较算子，后两个均可省略，默认容器为 vector，默认算子为 less（小的向前排，大的向后排，出队时序列尾元素先出队）

```cpp
priority_queue<int> q1;  //定义数据类型为int，默认大的先出队
priority_queue< pair<int,int> > q2;  //定义数据类型为pair，默认大的先出队
priority_queue<int, vector<int>, greater<int> > q3; //定义小的先出队
```

# 【比较算子的定义】

如果要定义自己的比较算子，方法有很多种，最常见的一种是：重载比较运算符。

将两个元素 x 和 y 代入比较运算符 (对 less 算子，调用 x<y，对 greater 算子，调用 x>y)，若结果为真，则 x 排在 y 前面，y 将先于 x 出队，反之，则将 y 排在 x 前面，x 将先出队。

例如：

1）按照 z 的顺序从大到小出队：

```cpp
/*
    运行结果：
    3 3 6
    2 2 5
    1 5 4
    4 4 3
*/
#include<iostream>
#include<queue>
using namespace std;
struct T{
    int x,y,z;
    T(int a, int b, int c):x(a), y(b), z(c){}
    bool operator < (const T &t1, const T &t2) const {
        return t1.z<t2.z; //按照z的顺序来决定t1和t2的顺序
    }
};
int main(){
    priority_queue<T> q;
    q.push(T(4,4,3));
    q.push(T(2,2,5));
    q.push(T(1,5,4));
    q.push(T(3,3,6));
    while (!q.empty()){
        T t=q.top();
        q.pop();
        cout<<t.x<<" "<<t.y<<" "<<t.z<<endl;
    }
    return 0; 
}
```

2）按照 z 的顺序从小到大出队

```cpp
/*
    运行结果：
    4 4 3
    1 5 4
    2 2 5
    3 3 6
*/
#include<iostream>
#include<queue>
using namespace std;
struct T{
    int x,y,z;
    T(int a, int b, int c):x(a), y(b), z(c){}
    bool operator > (const T &t1, const T &t2) const {
        return t1.z>t2.z; //按照z的顺序来决定t1和t2的顺序
    }
};
int main(){
    priority_queue< T,vector<T>,greater<T> > q;
    q.push(T(4,4,3));
    q.push(T(2,2,5));
    q.push(T(1,5,4));
    q.push(T(3,3,6));
    while (!q.empty()){
        T t=q.top();
        q.pop();
        cout<<t.x<<" "<<t.y<<" "<<t.z<<endl;
    }
    return 0; 
}
```

# 【基本操作】

priority_queue 的基本操作与 queue 相同：
- q.push(x)：入队，将 x 存入队列末端
- q.pop()：出队，将队列的第一个元素弹出
- q.front()：访问队首元素
- q.back()：访问队尾元素
- q.empty()：判断队列是否为空，当队列为空时返回 true，否则返回 false
- q.size()：访问队列中元素的个数



