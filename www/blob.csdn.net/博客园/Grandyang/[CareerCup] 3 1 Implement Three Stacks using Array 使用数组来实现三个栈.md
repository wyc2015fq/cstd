# [CareerCup] 3.1 Implement Three Stacks using Array 使用数组来实现三个栈 - Grandyang - 博客园







# [[CareerCup] 3.1 Implement Three Stacks using Array 使用数组来实现三个栈](https://www.cnblogs.com/grandyang/p/4675640.html)







3.1 Describe how you could use a single array to implement three stacks.



这道题让我们用一个数组来实现三个栈，书上给了两种方法，第一种方法是定长分割 Fixed Division，就是每个栈的长度相同，用一个公用的一位数组buffer来保存三个栈的内容，前三分之一为第一个栈，中间三分之一为第二个栈，后三分之一为第三个栈，然后还要分别记录各个栈当前元素的个数，然后再分别实现栈的基本操作push, pop, top 和 empty，参见代码如下：



```
class ThreeStacks {
public:
    ThreeStacks(int size) : _stackSize(size) {
        _buffer.resize(size * 3, 0);
        _stackCur.resize(3, -1);
    }
    
    void push(int stackIdx, int val) {
        if (_stackCur[stackIdx] + 1 >= _stackSize) {
            cout << "Stack " << stackIdx << " is full!" << endl;
        }
        ++_stackCur[stackIdx];
        _buffer[stackIdx * _stackSize + _stackCur[stackIdx]] = val;
    }
    
    void pop(int stackIdx) {
        if (empty(stackIdx)) {
            cout << "Stack " << stackIdx << " is empty!" << endl;
        }
        _buffer[stackIdx * _stackSize + _stackCur[stackIdx]] = 0;
        --_stackCur[stackIdx];
    }
    
    int top(int stackIdx) {
        if (empty(stackIdx)) {
            cout << "Stack " << stackIdx << " is empty!" << endl;
        }
        return _buffer[stackIdx * _stackSize + _stackCur[stackIdx]];
    }
    
    bool empty(int stackIdx) {
        return _stackCur[stackIdx] == -1;
    }
    
private:
    int _stackSize;
    vector<int> _buffer;
    vector<int> _stackCur;
};
```



第二种方法是灵活分割 Flexible Divisions，这种方法较为复杂，这里就不细写了。














