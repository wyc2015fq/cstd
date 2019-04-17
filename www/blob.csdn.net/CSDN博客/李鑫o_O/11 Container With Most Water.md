# 11. Container With Most Water - 李鑫o_O - CSDN博客





2016年03月16日 20:50:39[hustlx](https://me.csdn.net/HUSTLX)阅读数：190








从最两边的开始，因为最边上的线围起来的面积拥有最大的宽度，然后如果相邻的边小于等于外面的边则一直往中间找比较高的边，下面的算法比较巧妙的是每次不管怎么样都会向中间缩小，因为两个边势必有一个边等于两个边的最小值，这样就找出了最大面积






```cpp
int maxArea(vector<int>& height) {
    int water = 0;
    int i = 0, j = height.size() - 1;
    while (i < j) {
        int h = min(height[i], height[j]);
        water = max(water, (j - i) * h);
        while (height[i] <= h && i < j) i++;
        while (height[j] <= h && i < j) j--;
    }
    return water;
}
```





