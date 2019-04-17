# [CareerCup] 3.3 Set of Stacks 多个栈 - Grandyang - 博客园







# [[CareerCup] 3.3 Set of Stacks 多个栈](https://www.cnblogs.com/grandyang/p/4677072.html)







3.3 Imagine a (literal) stack of plates. If the stack gets too high, it might topple. Therefore, in real life, we would likely start a new stack when the previous stack exceeds some threshold. Implement a data structure SetOf Stacks that mimics this. SetOf Stacks should be composed of several stacks and should create a new stack once the previous one exceeds capacity. SetOf Stacks. push() and SetOf Stacks. pop() should behave identically to a single stack (that is, pop() should return the same values as it would if there were just a single stack).
FOLLOW UP
Implement a function popAt(int index) which performs a pop operation on a specific sub-stack.



这道题让我们实现一个多个栈的数据结构，这个多个栈顾名思义是由许多子栈组成的，每个子栈都有相同的大小，在压入栈的过程中，如果当前子栈满了，则新建一个栈，压入新值。在移除栈顶元素时，若最上面的子栈移除后为空时，移除该子栈。还是返回栈顶元素和判断栈是否为空，这些基本的栈操作都不是很难，只要注意需要的时候新建和销毁子栈就行了。难点在于Follow Up，让我们来实现一个popAt函数，这个函数是要对于任意一个子函数来移除栈顶元素，找到并移除子栈的站定元素并不难，难点在于如果我们默认子栈必须都存满的话，那么如果中间的子栈栈顶被移除了，上面紧挨的子栈的栈底元素就要移到下面，成为下面的子栈的栈顶元素，然后以此类推，每个中间的子栈都要装满，这个实现起来就比较复杂，我们需要用递归来做，我们需要实现一个leftShift的辅助函数，这个函数可以移除子栈的栈顶元素或者栈底元素，具体实现参见代码如下：



```
class SetOfStacks {
public:
    SetOfStacks(int capacity) : _capacity(capacity) {}
    
    void push(int x) {
        stack<int> last;
        if (!_stacks.empty() && _stacks.back().size() < _capacity) {
            last = _stacks.back();
            _stacks.pop_back();
        }
        last.push(x);
        _stacks.push_back(last);
    }
    
    void pop() {
        if (!_stacks.empty()) {
            stack<int> last = _stacks.back();
            last.pop();
            if (last.empty()) _stacks.pop_back();
        }
    }
    
    int top() {
        if (!_stacks.empty()) {
            stack<int> last = _stacks.back();
            return last.top();
        }
        return 0;
    }
    
    bool empty() {
        return _stacks.empty();
    }
    
    void popAt(int index) {
        leftShift(index, true);
    }
    
    int leftShift(int index, bool removeTop) {
        stack<int> *cur = &_stacks[index];
        int removed_item;
        if (removeTop) {
            removed_item = cur->top();
            cur->pop();
        } else {
            stack<int> tmp;
            while (!cur->empty() && cur->size() != 1) {
                tmp.push(cur->top());
                cur->pop();
            }
            removed_item = cur->top();
            cur->pop();
            while (!tmp.empty()) {
                cur->push(tmp.top());
                tmp.pop();
            }
        }
        if (cur->empty()) _stacks.erase(_stacks.begin() + index);
        else if (_stacks.size() > index + 1) {
            int val = leftShift(index + 1, false);
            cur->push(val);
        }
        return removed_item;
    }
    
private:
    vector<stack<int> > _stacks;
    int _capacity;
};
```














