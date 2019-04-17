# 剑指offer 面试题41：数据流中的中位数 c++ - 别说话写代码的博客 - CSDN博客





2018年11月29日 12:46:46[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：40
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：如何得到一个数据流中的中位数？如果从数据流中读出奇数个数值，那么中位数就是所有数值排序之后位于中间的数值。如果从数据流中读出偶数个数值，那么中位数就是所有数值排序之后中间两个数的平均值。

思路：数据流的中位数最好使用两个堆，这样可以动态维护插入和获得中位数的过程。可以使用一个大顶堆和一个小顶堆， 中位数左边是 大顶堆，放比中位数小的元素，中位数右边放小顶堆，放比中位数大的元素。

如果当前元素个数为偶数个，可以插入小顶堆。若待插入元素小于大顶堆最大值，则需要将其插入大顶堆，然后将大顶堆最大值放入小顶堆，否则直接插入小顶堆。

如果为奇数个，插入大顶堆。若待插入元素大于小顶堆最小值，则先插入小顶堆， 然后将小顶堆的最小值放入最大堆，否则直接放入最大堆。

```cpp
class Solution {
private:
    vector<int> min;
    vector<int> max;
public:
    void Insert(int num)
    {
        if(((min.size()+max.size())&1) == 0)  //偶数
        {
            if(max.size()>0 && num<max[0])  //带插入元素小于左边大顶堆最大值
            {
                max.push_back(num);
                push_heap(max.begin(),max.end(),less<int>()); //大顶堆
                num = max[0];
                pop_heap(max.begin(),max.end(),less<int>());
                max.pop_back();
            }
            min.push_back(num);
            push_heap(min.begin(),min.end(),greater<int>()); //小顶堆
        }else   //奇数
        {
            if(min.size()>0 && num>min[0])
            {
                min.push_back(num);
                push_heap(min.begin(),min.end(),greater<int>());
                num = min[0];
                pop_heap(min.begin(),min.end(),greater<int>());
                min.pop_back();
            }
            max.push_back(num);
            push_heap(max.begin(),max.end(),less<int>());
        }
    }

    double GetMedian()
    { 
        int len = min.size() + max.size();
        if(len <=0) return 0;
        double m = 0;
        if((len & 1) == 0) m = (double)(min[0]+max[0])/2;
        else m = (double)min[0];
        return m;
    }
};
```





