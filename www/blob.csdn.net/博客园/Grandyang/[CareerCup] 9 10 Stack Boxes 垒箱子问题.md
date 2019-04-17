# [CareerCup] 9.10 Stack Boxes 垒箱子问题 - Grandyang - 博客园







# [[CareerCup] 9.10 Stack Boxes 垒箱子问题](https://www.cnblogs.com/grandyang/p/4847695.html)







9.10 You have a stack of n boxes, with widths w., heights hir and depths drThe boxes cannot be rotated and can only be stacked on top of one another if each box in the stack is strictly larger than the box above it in width, height, and depth. Implement a method to build the tallest stack possible, where the height of a stack is the sum of the heights of each box.



开始看到这题时，以为是[3.4 Towers of Hanoi 汉诺塔](http://www.cnblogs.com/grandyang/p/4677404.html)，其实不太一样，这道题只是单纯的让我们垒箱子而已，大的在最底下，问我们能垒出的最大高度是多少。也是一道用递归来解的题，首先我们要先实现箱子类Box，里面包含了箱子的尺寸宽，高和深度，然后还要有一个成员函数canBeAbove，用来判断当前箱子能否放到另一个箱子的上面，还有一个静态函数，是求一摞箱子的总高度的。然后我们对每个箱子都调用一次递归，然后维护一个最大值，每次递归都更新这个最大值，那么最终递归结束后这个最大值就是所求，参见代码如下：



解法一：

```
class Box {
public:
    int _width, _depth, _height;
    Box(int w, int d, int h): _width(w), _depth(d), _height(h) {}
    bool canBeAbove(Box *bottom) {
        if (bottom == nullptr) return true;
        return _width > bottom->_width && _depth > bottom->_depth && _height > bottom->_height;
    }
    static int stackHeight(vector<Box*> stack) {
        int res = 0;
        for (auto &a : stack) {
            res += a->_height;
        }
        return res;
    }
};

class Solution {
public:
    vector<Box*> createStack(vector<Box*> boxes) {
        return createStack(boxes, nullptr);
    }
    vector<Box*> createStack(vector<Box*> boxes, Box *bottom) {
        vector<Box*> res;
        int max_height = 0;
        for (auto &a : boxes) {
            if (a->canBeAbove(bottom)) {
                vector<Box*> new_stack = createStack(boxes, a);
                int new_height = Box::stackHeight(new_stack);
                if (new_height > max_height) {
                    res = new_stack;
                    max_height = new_height;
                }
            }
        }
        if (bottom != nullptr) res.push_back(bottom);
        return res;
    }
};
```



上述代码虽然正确，但是不高效，像之前那道[9.8 Represent N Cents 美分的组成](http://www.cnblogs.com/grandyang/p/4840713.html)一样，我们也可以用哈希表来优化，保存我们之前算过的最优解，那么在递归调用需要相同的结果时，就可以直接从哈希表中调用，参见代码如下：



解法二：

```
class Solution {
public:
    vector<Box*> createStack(vector<Box*> boxes) {
        unordered_map<Box*, vector<Box*> > m;
        return createStack(boxes, nullptr, m);
    }
    vector<Box*> createStack(vector<Box*> &boxes, Box *bottom, unordered_map<Box*, vector<Box*>  > &m) {
        if (bottom != nullptr && m.find(bottom) != m.end()) {
            return m[bottom];
        }
        vector<Box*> res;
        int max_height = 0;
        for (auto &a : boxes) {
            if (a->canBeAbove(bottom)) {
                vector<Box*> new_stack = createStack(boxes, a, m);
                int new_height = Box::stackHeight(new_stack);
                if (new_height > max_height) {
                    res = new_stack;
                    max_height = new_height;
                }
            }
        }
        if (bottom != nullptr) {
            res.push_back(bottom);
            m[bottom] = res;
        }
        return res;
    }
};
```














