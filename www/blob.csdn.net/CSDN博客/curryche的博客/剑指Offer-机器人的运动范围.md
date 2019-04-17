# 剑指Offer - 机器人的运动范围 - curryche的博客 - CSDN博客





2018年08月08日 21:57:16[curryche](https://me.csdn.net/whwan11)阅读数：72








## 题目

机器人的运动范围 

时间限制：1秒 空间限制：32768K 热度指数：90945

题目描述 

地上有一个m行和n列的方格。一个机器人从坐标0,0的格子开始移动，每一次只能向左，右，上，下四个方向移动一格，但是不能进入行坐标和列坐标的数位之和大于k的格子。 例如，当k为18时，机器人能够进入方格（35,37），因为3+5+3+7 = 18。但是，它不能进入方格（35,38），因为3+5+3+8 = 19。请问该机器人能够达到多少个格子？

## 解法

代码

```
class Solution {
public:
    int movingCount(int threshold, int rows, int cols)
{
    bool* visit = new bool[rows*cols];
    for (int i = 0; i<rows*cols; i++)
        visit[i] = false;

    return move(threshold, rows, cols, 0, 0, visit);

}

int move(int threshold, int rows, int cols, int i, int j, bool* visit)
{
    int count = 0;
    if (rows > 0 && cols > 0 && i >= 0 && i < rows&&j >= 0 && j < cols&&visit&&threshold >= 0 && 
        get_digits_sum(i) + get_digits_sum(j) <= threshold&&!visit[cols*i+j])
    {
        visit[cols*i + j] = true;
        count = 1 + move(threshold, rows, cols, i + 1, j, visit)
            + move(threshold, rows, cols, i-1, j, visit)
            + move(threshold, rows, cols, i , j-1, visit)
            + move(threshold, rows, cols, i , j+1, visit);
    }
    return count;
}

int get_digits_sum(int num)
{
    cout << "num: " << num << endl;
    int result = 0;
    while (num)
    {
        result += num % 10;
        num /= 10;
    }
    cout << "result: " << result << endl;
    return result;
}
};
```



