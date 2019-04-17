# 【剑指offer】题30：最小的k个数 - xiaxzhou的博客 - CSDN博客





2017年07月03日 14:49:29[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：115
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
vector<int> GetLeastNumbers_Solution(vector<int> input, int k)
{
    assert(input.size() >= k);
    vector<int> my_heap;
    my_heap.insert(my_heap.begin(),input.begin(),input.begin()+k);
    make_heap(my_heap.begin(),my_heap.end());
    for (auto i = k; i < input.size();++i)
    {
        if (my_heap[0]>input[i])
        {
            pop_heap(my_heap.begin(), my_heap.end());
            my_heap[k - 1] = input[i];
            push_heap(my_heap.begin(), my_heap.end());
        }
    }
    //sort_heap(my_heap.begin(), my_heap.end());
    return my_heap;
}
```

上述代码不知道为啥牛客网说段错误

改用优先队列实现：

```cpp
vector<int> vec;
    if (input.size()<k)
    {
        return vec;
    }
    priority_queue<int> p;
    for (auto i = 0; i < input.size();++i)
    {
        p.push(input[i]);
        if (p.size()>k)
        {
            p.pop();
        }
    }
    while (p.size())
    {
        vec.push_back(p.top());
        p.pop(); 
    }
    return vec;
```



