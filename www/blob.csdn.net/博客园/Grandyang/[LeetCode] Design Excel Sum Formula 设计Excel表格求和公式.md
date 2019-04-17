# [LeetCode] Design Excel Sum Formula 设计Excel表格求和公式 - Grandyang - 博客园







# [[LeetCode] Design Excel Sum Formula 设计Excel表格求和公式](https://www.cnblogs.com/grandyang/p/7170238.html)







Your task is to design the basic function of Excel and implement the function of sum formula. Specifically, you need to implement the following functions:

`Excel(int H, char W):` This is the constructor. The inputs represents the height and width of the Excel form. His a positive integer, range from 1 to 26. It represents the height. W is a character range from 'A' to 'Z'. It represents that the width is the number of characters from 'A' to W. The Excel form content is represented by a height * width 2D integer array `C`, it should be initialized to zero. You should assume that the first row of `C` starts from 1, and the first column of `C` starts from 'A'.



`void Set(int row, char column, int val):` Change the value at `C(row, column)` to be val.



`int Get(int row, char column):` Return the value at `C(row, column)`.



`int Sum(int row, char column, List of Strings : numbers):` This function calculate and set the value at `C(row, column)`, where the value should be the sum of cells represented by `numbers`. This function return the sum result at `C(row, column)`. This sum formula should exist until this cell is overlapped by another value or another sum formula.

`numbers` is a list of strings that each string represent a cell or a range of cells. If the string represent a single cell, then it has the following format : `ColRow`. For example, "F7" represents the cell at (7, F).

If the string represent a range of cells, then it has the following format : `ColRow1:ColRow2`. The range will always be a rectangle, and ColRow1 represent the position of the top-left cell, and ColRow2 represents the position of the bottom-right cell.



Example 1:
Excel(3,"C"); 
// construct a 3*3 2D array with all zero.
//   A B C
// 1 0 0 0
// 2 0 0 0
// 3 0 0 0

Set(1, "A", 2);
// set C(1,"A") to be 2.
//   A B C
// 1 2 0 0
// 2 0 0 0
// 3 0 0 0

Sum(3, "C", ["A1", "A1:B2"]);
// set C(3,"C") to be the sum of value at C(1,"A") and the values sum of the rectangle range whose top-left cell is C(1,"A") and bottom-right cell is C(2,"B"). Return 4. 
//   A B C
// 1 2 0 0
// 2 0 0 0
// 3 0 0 4

Set(2, "B", 2);
// set C(2,"B") to be 2. Note C(3, "C") should also be changed.
//   A B C
// 1 2 0 0
// 2 0 2 0
// 3 0 0 6



Note:
- You could assume that there won't be any circular sum reference. For example, A1 = sum(B1) and B1 = sum(A1).
- The test cases are using double-quotes to represent a character.
- Please remember to RESET your class variables declared in class Excel, as static/class variables are persisted across multiple test cases. Please see [here](https://leetcode.com/faq/#different-output) for more details.



这道题让我们设计Excel表格的求和公式，Excel表格想必大家都用过，还是比较熟悉的，这里让我们对单元格进行求和运算。由于这道题里要求二维数组的局部和，而且又会经常更新数组的值，博主第一反应觉得应该用之前那题[Range Sum Query 2D - Mutable](http://www.cnblogs.com/grandyang/p/5300458.html)中的树状数组来做，结果哼哼哧哧的写完后，发现下面这个test case没通过：

["Excel","sum","set","get"]
[[3,"C"],[1,"A",["A2"]],[2,"A",1],[1,"A"]]
Expected:
[null,0,null,1]

仔细分析一下发现，这个case先把A2的值赋给了A1，此时A1和A2都是0，然后给A2赋值为1，求A1的值。大家的第一印象肯定是觉得A1还是0啊，其实在Excel中，相当于已经把A1和A2关联起来了，只要A2点值发生了改变，A1的值也会跟着变，所以A1的值此时也为1。而树状数组的主要功能的优化区域和的计算速度，并没有建立关联的步骤，难怪不能通过OJ呢。这道题标记为Hard还是有道理的，我们要模拟出Excel表中的这种关联方式，这里参考的是[yupinglu大神的帖子](https://discuss.leetcode.com/topic/93819/c-3-ms-solution-easy-to-understand)，首先我们肯定需要一个二维数组mat来保存数据，然后需要一个map来建立单元格和区域和之间的映射，这里的区域和就是sum函数中的字符串数组表示的内容，可参见题目中的例子，有可能单个单元格或者多个。

我们来看set函数，如果我们改变了某个单元格的内容，那么如果作为结果单元格，那么对应的链接就会断开。比如我们有三个单元格A1, B1, C1，我们设置的关联是A1 + B1 = C1，那么我们改变A1和B1的值都是OK的，C1的值会自动更新。但如果我们改变了C1的值，那么这个关联就不复存在了，Excel中也是这样的。所以我们在改变某个单元格的时候，要将其的关联删除。

我们再来看get函数，我们在获取某个单元格的值的时候，一定要先看其有没有和其他单元格关联，如果有的话，要重新计算一下关联，有可能关联的单元格的值已经发生改变了，那么当前作为结果单元格的值也需要改变；如果该单元格没有任何关联，那么就直接从数组mat中取值即可。

最后看本题的难点sum函数，要根据关联格求出结果格的值，首先这个字符串数组可能有多个字符串，每个字符串有两个可能，一种是单个的单元格，一种是两个单元格中间用冒号隔开。那么我们需要分情况讨论，区别这两种情况的方法就是看冒号是否存在，如果不存在，就说明只有一个单元格，我们将其数字和字母都提取出来，调用get函数，将该位置的值加入结果res中；如果冒号存在，我们根据冒号的位置，分别将两个单元格的字母和数字提取出来，然后遍历这两个单元格之间所有的单元格，调用get函数并将返回值加入结果res中。这个遍历相加的过程可能可以用树状数组来优化，但由于这不是此题的考察重点，所以直接遍历就OK。最后别忘了建立目标单元格和区域字符串数组之间的映射，并返回结果res即可。



```
class Excel {
public:
    Excel(int H, char W) {
        m.clear();
        mat.resize(H, vector<int>(W - 'A', 0));
    }
    
    void set(int r, char c, int v) {
        if (m.count({r, c})) m.erase({r, c});
        mat[r - 1][c - 'A'] = v;
    }
    
    int get(int r, char c) {
        if (m.count({r, c})) return sum(r, c, m[{r, c}]);
        return mat[r - 1][c - 'A'];
    }
    
    int sum(int r, char c, vector<string> strs) {
        int res = 0;
        for (string str : strs) {
            auto found = str.find_last_of(":");
            if (found == string::npos) {
                char y = str[0];
                int x = stoi(str.substr(1));
                res += get(x, y);
            } else {
                int x1 = stoi(str.substr(1, (int)found - 1)), y1 = str[0] - 'A';
                int x2 = stoi(str.substr(found + 2)), y2 = str[found + 1] - 'A';
                for (int i = x1; i <= x2; ++i) {
                    for (int j = y1; j <= y2; ++j) {
                        res += get(i, j + 'A');
                    }
                }
            }
        }
        m[{r, c}] = strs;
        return res;
    }

private:
    vector<vector<int>> mat;
    map<pair<int, char>, vector<string>> m;
};
```



参考资料：

[https://discuss.leetcode.com/topic/93819/c-3-ms-solution-easy-to-understand](https://discuss.leetcode.com/topic/93819/c-3-ms-solution-easy-to-understand)

[https://discuss.leetcode.com/topic/93812/c-3-ms-concise-and-easy-to-understand](https://discuss.leetcode.com/topic/93812/c-3-ms-concise-and-easy-to-understand)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












