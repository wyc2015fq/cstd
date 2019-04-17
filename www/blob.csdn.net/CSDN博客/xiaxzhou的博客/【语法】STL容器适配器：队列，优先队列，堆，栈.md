# 【语法】STL容器适配器：队列，优先队列，堆，栈 - xiaxzhou的博客 - CSDN博客





2017年05月26日 19:24:12[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：265








# deque

> 
deque在分段的连续内存空间维护其整体连续的假象


```cpp
queue<double> values;  

 //使用push函数，将元素添加到队列中  
 values.push(3.2);  
 values.push(9.8);  
 values.push(5.4);  

 cout<<"popping from values: ";  

 //使用empty函数，判断队列是否为空  
 while (!values.empty())  
 {  
  cout<<values.front()<<' ';  //当队列还有其他元素时，使用front函数读取（但不删除）队列的第一个元素，用于输出  
  values.pop();              //用pop函数，删除队列的第一个元素  
 }
```

# 优先队列

> 
优先队列缺省使用最大堆实现，后者缺省利用vecotr实现


```cpp
priority_queue<int> p_q_max;
    priority_queue < int, vector<int>, greater<int>> p_q_min;
    srand(1);
    for (auto i = 0; i < 10;++i)
    {
        int tmp = rand();
        p_q_min.push(tmp);
        p_q_max.push(tmp);
    }
    while (!p_q_max.empty())
    {
        cout<<p_q_max.top()<<" ";
        p_q_max.pop();
    }
    cout << endl;
    while (!p_q_min.empty())
    {
        cout << p_q_min.top() << " ";
        p_q_min.pop();
    }
    cout << endl;
```

# 堆

> 
不是容器组件，幕后英雄，一般利用vector为实现


```cpp
int array[9] = {0,3,2,1,4,8,5,7,6};
    vector<int> vec(array,array+9);

    make_heap(vec.begin(), vec.end());//建堆，STL提供的是max-heap

    vec.push_back(10);
    push_heap(vec.begin(), vec.end());//维护堆
    //将最后一个元素视为新增元素，将其余元素视为完整的heap

    pop_heap(vec.begin(),vec.end());//弹出堆顶
    //堆顶移到最后一位
```

# 栈

```cpp
stack<int, list<int>> mystack;
    mystack.push(1);
    mystack.push(2);
    mystack.push(3);
    cout << mystack.top() << endl;
    mystack.pop();
    cout << mystack.top() << endl;
    mystack.pop(); cout << mystack.top() << endl;
    mystack.pop();
```



