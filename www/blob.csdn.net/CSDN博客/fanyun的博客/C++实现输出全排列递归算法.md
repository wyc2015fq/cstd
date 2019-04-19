# C++实现输出全排列递归算法 - fanyun的博客 - CSDN博客
2018年12月31日 17:54:19[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：266
1.要求，实现1,2,3,4的全排列
2.实现核心思想
总体实现就是递归思想的应用，设R={r1,r2,…,rn}是要进行排列的n个元素，Ri=R-{ri}. 
Perm(X)表示在全排列Perm(X)的每一个排列前加上前缀ri得到的排列。 
（1）当n=1时，Perm(R)=(r),其中r是集合R中唯一的元素； 
（2）当n>1时，Perm(R)可由(r1)+Perm(R1),(r2)+Perm(R2),…,(rn)+Perm(Rn)构成。
代码整体思路如下：
perm({1,2,3})=1perm({2,3})+2perm({1,3})+3perm({1,2}) 
perm({2,3})=2perm({3})+3perm({2}) 
perm({1,3})=1perm({3})+3perm({1}) 
perm({1,2})=1perm({2})+2perm({1})
3.实现代码如下：
```cpp
#include <iostream>
using namespace std;
void swap(int &a,int &b){
    int temp=a;
    a=b;
    b=temp;
}
void perm(int list[],int low,int high){
    if(low==high){   //当low==high时,此时list就是其中一个排列,输出list
        for(int i=0;i<=low;i++)
            cout<<list[i];
        cout<<endl;
    }else{
        for(int i=low;i<=high;i++){//每个元素与第一个元素交换
            swap(list[i],list[low]); 
            perm(list,low+1,high); //交换后,得到子序列,用函数perm得到子序列的全排列
            swap(list[i],list[low]);//最后,将元素交换回来,复原,然后交换另一个元素
        }
    }
}
int main()
{
int list[]={1,2,3};
perm(list,0,2);
return 0;
}
```
4.代码输出如下
![](https://img-blog.csdnimg.cn/2018123111040233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_16,color_FFFFFF,t_70)
