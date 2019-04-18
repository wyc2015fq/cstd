# 1097卡片游戏——队列queue的应用 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:02:15[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：621











queue类是为程序员提供了一个队列的功能的容器适配器，具体而言，一个FIFO（先入先出）的数据结构

在头文件<queue>中定义。

原型

template<

class T,

　　class Container =std::deque<T>

> class queue;

### 成员函数
- 
empty()判断队列空，当队列空时，返回true。

- 
size()访问队列中的元素个数。

- 
push()会将一个元素置入queue中。

- 
front()会返回queue内的第一个元素(也就是第一个被置入的元素)。

- 
back()会返回queue中最后一个元素(也就是最后被插入的元素)。

- 
pop()会从queue中移除一个元素。[1]

- 
注意：pop()虽然会移除下一个元素，但是并不返回它，front()和back()返回下一个元素但并不移除该元素。





```cpp
<span style="font-size:24px;">#include<iostream>
#include<queue>
using namespace std;

queue<int> cards;
int main()
{
     int n;
     cin>>n;
     for(int i=0; i<n; i++)
          cards.push(i+1);
     while(cards.front()!=cards.back())
     {
          cout<<cards.front()<<' ';
          cards.pop();
          cards.push(cards.front());  //把开头的发在最后
          cards.pop();  //因为上面的push front不会删除首字符，所以这里还是需要pop

     }
     cout<<cards.front()<<endl;

     return 0;
}
</span>
```







注意对最后一个元素的输出处理~





