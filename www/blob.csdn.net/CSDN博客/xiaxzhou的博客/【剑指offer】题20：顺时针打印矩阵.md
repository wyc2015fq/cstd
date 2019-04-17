# 【剑指offer】题20：顺时针打印矩阵 - xiaxzhou的博客 - CSDN博客





2017年07月02日 09:46:24[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：158








```cpp
vector<int> printMatrix(vector<vector<int> > matrix)
{
    vector<int> result;
    int startx(0), starty(0), endx(matrix.size()-1), endy(matrix[0].size()-1);
    while (startx <= endx && starty <= endy)
    {
        for (auto y = starty ; y <= endy; ++y)
        {
            result.push_back(matrix[startx][y]);
            //cout << matrix[x][y] << " ";
        }
        if (startx < endx)
        {
            for (auto x = startx+1; x <= endx; ++x)
            {
                result.push_back(matrix[x][endy]);
                //cout << matrix[x][y] << " ";
            }
        }
        if (starty < endy&&startx < endx)
        {
            for (auto y = endy-1; y >= starty; --y)
            {
                result.push_back(matrix[endx][y]);
                //cout << matrix[x][y] << " ";
            }
        }
        if (startx + 1 < endx &&starty < endy)
        {
            for (auto x = endx-1; x > startx; --x)
            {
                result.push_back(matrix[x][starty]);
                //cout << matrix[x][y] << " ";
            }
        }
        startx++;
        starty++;
        endx--;
        endy--;
    }
    return result;
}
```



