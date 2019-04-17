# [LeetCode] Flatten 2D Vector 压平二维向量 - Grandyang - 博客园







# [[LeetCode] Flatten 2D Vector 压平二维向量](https://www.cnblogs.com/grandyang/p/5209621.html)







Implement an iterator to flatten a 2d vector.

For example,
Given 2d vector =
[
  [1,2],
  [3],
  [4,5,6]
]



By calling *next* repeatedly until *hasNext* returns false, the order of elements returned by *next* should be: `[1,2,3,4,5,6]`.

Hint:
- How many variables do you need to keep track?
- Two variables is all you need. Try with `x` and `y`.
- Beware of empty rows. It could be the first few rows.
- To write correct code, think about the [invariant](https://en.wikipedia.org/wiki/Invariant_(computer_science)) to maintain. What is it?
- The invariant is `x` and `y` must always point to a valid point in the 2d vector. Should you maintain your invariant *ahead of time* or *right when you need it*?
- Not sure? Think about how you would implement `hasNext()`. Which is more complex?
- Common logic in two different places should be refactored into a common method.

Follow up:
As an added challenge, try to code it using only [iterators in C++](http://www.cplusplus.com/reference/iterator/iterator/) or [iterators in Java](http://docs.oracle.com/javase/7/docs/api/java/util/Iterator.html).



这道题让我们压平一个二维向量数组，并且实现一个iterator的功能，包括next和hasNext函数，那么最简单的方法就是将二维数组按顺序先存入到一个一维数组里，然后此时只要维护一个变量i来记录当前遍历到的位置，hasNext函数看当前坐标是否小于元素总数，next函数即为取出当前位置元素，坐标后移一位，参见代码如下：                      



解法一：

```
class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec2d) {
        for (auto a : vec2d) {
            v.insert(v.end(), a.begin(), a.end());
        }    
    }
    int next() {
        return v[i++];
    }
    bool hasNext() {
        return i < v.size();
    }
private:
    vector<int> v;
    int i = 0;
};
```



下面我们来看另一种解法，不直接转换为一维数组，而是维护两个变量x和y，我们将x和y初始化为0，对于hasNext函数，我们检查当前x是否小于总行数，y是否和当前行的列数相同，如果相同，说明要转到下一行，则x自增1，y初始化为0，若此时x还是小于总行数，说明下一个值可以被取出来，那么在next函数就可以直接取出行为x，列为y的数字，并将y自增1，参见代码如下：



解法二：

```
class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec2d) {
        v = vec2d;
        x = y = 0;
    }
    int next() {
        return v[x][y++];
    }
    bool hasNext() {
        while (x < v.size() && y == v[x].size()) {
            ++x; 
            y = 0;
        }
        return x < v.size();
    }    
private:
    vector<vector<int>> v;
    int x, y;
};
```



题目中的Follow up让我们用interator来做，C++中iterator不像Java中的那么强大，自己本身并没有包含next和hasNext函数，所以我们得自己来实现，我们将x定义为行的iterator，再用个end指向二维数组的末尾，定义一个整型变量y来指向列位置，实现思路和上一种解法完全相同，只是写法略有不同，参见代码如下：



解法三：

```
class Vector2D {
public:
    Vector2D(vector<vector<int>>& vec2d) {
        x = vec2d.begin();
        end = vec2d.end();
    }
    int next() {
        return (*x)[y++];
    }
    bool hasNext() {
        while (x != end && y == (*x).size()) {
            ++x; 
            y = 0;
        }
        return x != end;
    }
private:
    vector<vector<int>>::iterator x, end;
    int y = 0;
};
```



参考资料：

[https://leetcode.com/discuss/52614/2-short-c-solution](https://leetcode.com/discuss/52614/2-short-c-solution)

[https://leetcode.com/discuss/50292/7-9-lines-added-java-and-c-o-1-space](https://leetcode.com/discuss/50292/7-9-lines-added-java-and-c-o-1-space)

[https://leetcode.com/discuss/50259/20ms-c-solution-with-explanations?show=50295#a50295](https://leetcode.com/discuss/50259/20ms-c-solution-with-explanations?show=50295#a50295)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












