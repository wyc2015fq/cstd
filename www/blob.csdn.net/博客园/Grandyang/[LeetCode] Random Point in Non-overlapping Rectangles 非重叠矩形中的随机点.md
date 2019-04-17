# [LeetCode] Random Point in Non-overlapping Rectangles 非重叠矩形中的随机点 - Grandyang - 博客园







# [[LeetCode] Random Point in Non-overlapping Rectangles 非重叠矩形中的随机点](https://www.cnblogs.com/grandyang/p/9752145.html)







Given a list of non-overlapping axis-aligned rectangles `rects`, write a function `pick` which randomly and uniformily picks an integer point in the space covered by the rectangles.

Note:
- An integer point is a point that has integer coordinates. 
- A point on the perimeter of a rectangle is included in the space covered by the rectangles. 
- `i`th rectangle = `rects[i]` = `[x1,y1,x2,y2]`, where `[x1, y1]` are the integer coordinates of the bottom-left corner, and `[x2, y2]` are the integer coordinates of the top-right corner.
- length and width of each rectangle does not exceed `2000`.
- `1 <= rects.length <= 100`
- `pick` return a point as an array of integer coordinates `[p_x, p_y]`
- `pick` is called at most `10000` times.


Example 1:
Input: 
["Solution","pick","pick","pick"]
[[[[1,1,5,5]]],[],[],[]]
Output: 
[null,[4,1],[4,1],[3,3]]


Example 2:
Input: 
["Solution","pick","pick","pick","pick","pick"]
[[[[-2,-2,-1,-1],[1,0,3,0]]],[],[],[],[],[]]
Output: 
[null,[-1,-2],[2,0],[-2,-1],[3,0],[-2,-2]]


Explanation of Input Syntax:

The input is two lists: the subroutines called and their arguments. `Solution`'s constructor has one argument, the array of rectangles `rects`. `pick` has no arguments. Arguments are always wrapped with a list, even if there aren't any.





这道题给了我们一些非重叠的矩形，让我们返回一个这些矩形中的一个随机的点。那么博主的第一直觉就是首先要在这些矩形中随机挑出来一个，然后在这个随机的矩形中再随机生成一个点，通过随机生成一个长和宽即可。博主最开始想到的方法是用rand随机生成一个 [0, n) 范围内的数字，n为输入矩形的个数，这样就得到了一个随机的矩形。但是这种方法貌似行不通，会跪在一个很长的输入测试数据。这使得博主比较困惑了，没有想出原因是为何，有哪位看官大神们知道的，麻烦留言告知博主哈！哈，已经知道了，参见评论区二楼留言～ 论坛上的解法有一种是用[水塘抽样Reservoir Sampling](https://zh.wikipedia.org/wiki/%E6%B0%B4%E5%A1%98%E6%8A%BD%E6%A8%A3)的方法的，LeetCode之前有过几道需要用这种方法的题目 [Random Pick Index](https://www.cnblogs.com/grandyang/p/5875509.html)，[Shuffle an Array](http://www.cnblogs.com/grandyang/p/5783392.html) 和 [Linked List Random Node](http://www.cnblogs.com/grandyang/p/5759926.html)。这里我们使用其来随机出一个矩形，做法是遍历所有的矩形，用变量sumArea来计算当前遍历过的所有矩形面积之和，然后变量area是当前遍历的矩形的面积（注意这里不是严格意义上的面积，而是该区域内整数坐标的点的个数，所以长宽相乘的时候都要加1），然后我们在当前所有矩形面积之和内随机生成一个值，如果这个值小于area，那么选择当前的矩阵为随机矩形。这里相当于一个大小为area的水塘，在这个值之内的话，就更换selected。这个方法是没啥问题，但是博主还是没想通为啥不能直接随机生成矩形的index。当我们拿到随机矩形后，之后就随机出宽和高返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    Solution(vector<vector<int>> rects) {
        _rects = rects;
    }
    
    vector<int> pick() {
        int sumArea = 0;
        vector<int> selected;
        for (auto rect : _rects) {
            int area = (rect[2] - rect[0] + 1) * (rect[3] - rect[1] + 1);
            sumArea += area;
            if (rand() % sumArea < area) selected = rect;
        }
        int x = rand() % (selected[2] - selected[0] + 1) + selected[0];
        int y = rand() % (selected[3] - selected[1] + 1) + selected[1];
        return {x, y};
    }

private:
    vector<vector<int>> _rects;
};
```



这道题在论坛上的主流解法其实是这个，我们用TreeMap来建立当前遍历过的矩形面积之和跟该矩形位置之间的映射。然后当我们求出所有的矩形面积之和后，我们随机生成一个值，然后在TreeMap中找到第一个大于这个值的矩形，这里博主还是有疑问，为啥不能直接随机矩形的位置，而是非要跟面积扯上关系。之后的步骤就跟上面的没啥区别了，参见代码如下：



解法二：

```
class Solution {
public:
    Solution(vector<vector<int>> rects) {
        _rects = rects;
        _totalArea = 0;
        for (auto rect : rects) {
            _totalArea += (rect[2] - rect[0] + 1) * (rect[3] - rect[1] + 1);
            _areaToIdx.insert({_totalArea, _areaToIdx.size()});
        }
    }
    
    vector<int> pick() {
        int val = rand() % _totalArea;
        int idx = _areaToIdx.upper_bound(val)->second;
        int width = _rects[idx][2] - _rects[idx][0] + 1;
        int height = _rects[idx][3] - _rects[idx][1] + 1;
        return {rand() % width + _rects[idx][0], rand() % height + _rects[idx][1]};
    }

private:
    vector<vector<int>> _rects;
    int _totalArea;
    map<int, int> _areaToIdx;
};
```



类似题目：

[Random Pick with Weight](https://www.cnblogs.com/grandyang/p/9784690.html)

[Generate Random Point in a Circle](https://www.cnblogs.com/grandyang/p/9741220.html)



参考资料：

[https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/](https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/)

[https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/155005/C%2B%2B-single-rand()-call](https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/155005/C%2B%2B-single-rand()-call)

[https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/169185/Short-C%2B%2B-solution-with-upper_bound](https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/169185/Short-C%2B%2B-solution-with-upper_bound)

[https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/170503/C%2B%2B-solution-using-reservoir-sampling-with-explanation-concise-and-easy-to-understand](https://leetcode.com/problems/random-point-in-non-overlapping-rectangles/discuss/170503/C%2B%2B-solution-using-reservoir-sampling-with-explanation-concise-and-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












