# 剑指offer 面试题：圆圈中最后剩下的数字（约瑟夫环） - 别说话写代码的博客 - CSDN博客





2019年03月04日 10:29:39[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：48
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：每年六一儿童节,牛客都会准备一些小礼物去看望孤儿院的小朋友,今年亦是如此。HF作为牛客的资深元老,自然也准备了一些小游戏。其中,有个游戏是这样的:首先,让小朋友们围成一个大圈。然后,他随机指定一个数m,让编号为0的小朋友开始报数。每次喊到m-1的那个小朋友要出列唱首歌,然后可以在礼品箱中任意的挑选礼物,并且不再回到圈中,从他的下一个小朋友开始,继续0...m-1报数....这样下去....直到剩下最后一个小朋友,可以不用表演,并且拿到牛客名贵的“名侦探柯南”典藏版(名额有限哦!!^_^)。请你试着想下,哪个小朋友会得到这份礼品呢？(注：小朋友的编号是从0到n-1)

思路：

**第一种**： 用链表，可以直接使用自带链表list。每次删除第m个直到剩下最后一个，遍历的时候 如果碰到最后 一个元素那就从第一个开始。

```cpp
class Solution {
public:
    int LastRemaining_Solution(int n, int m)
    {
        if(n<1 || m<1) return -1;
        list<int> num;
        for(int i=0;i<n;++i) num.push_back(i);
        auto cur = num.begin();   //遍历指针
        while(num.size()>1)      
        {
            for(int i=1;i<m;++i)    //找要删的是哪一个元素        
            {
                cur++;
                if(cur == num.end()) cur = num.begin();
            }
            auto next = ++cur;
            if(next == num.end()) next = num.begin();  //要删除的元素是最后一个
            --cur; 
            num.erase(cur);
            cur=next;
        }
        return *cur;
    }
};
```

第二种： 利用递推公式

![](https://img-blog.csdnimg.cn/20190304102717546.png)

```cpp
int LastRemaining_Solution(int n, int m)
    {
        if(n<1 || m<1) return -1;
        int res =0;
        for(int i=2;i<=n;++i)
            res = (res + m) % i;
        return res;
    }
```





