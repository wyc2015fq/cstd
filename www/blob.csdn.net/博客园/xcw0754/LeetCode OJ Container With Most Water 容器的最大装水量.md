# LeetCode OJ Container With Most Water  容器的最大装水量 - xcw0754 - 博客园
# [LeetCode OJ Container With Most Water  容器的最大装水量](https://www.cnblogs.com/xcw0754/p/4348996.html)
题意：在坐标轴的x轴上的0，1，2，3，4、、、、n处有n+1块木板，长度不一，任两块加上x轴即可构成一个容器，其装水面积为两板的间距与较短板长之积，以vector容器给出一系列值，分别代表在0，1，2，3，4、、、vector.size()-1共size个短板，是连续的，不排除有板长为0的可能性，但是至少有两块板。根据所给板长，求任两板与x轴所能构成的最大面积并返回。
代码：
```
1 class Solution {
 2 public:
 3     int maxArea(vector<int> &height) {
 4        int maxArea = 0, area;
 5        int left = 0, right = height.size() - 1;
 6        while (left < right) {
 7             area = (right - left) * (height[left] < height[right] ? height[left] : height[right]);
 8             maxArea = (maxArea > area) ? maxArea : area;
 9             if (height[left] < height[right])
10                 ++left;
11             else
12                 --right;
13         }
14         return maxArea;
15     }
16 };
Container With Most Water
```
思路：
　　i, j分别从头尾开始遍历，面积 area = min(height[j], height[i]) * (j-i)，当height[i] < height[j]时，此时面积 area = height[i] * (j-i); 由于i是短板，不管跟其右边的哪块板组合，它能达到的最大面积取决于 j-i（即两板之间的距离），而此时的j-i的值是最大的，因此，此面积即为以i为左边界、以j为右边界的当前最大面积，然后++i（即将短的边界往中移，寻找更高的且能使面积更大的板，更新area，继续++i，直到i板比j板高，才开始移动右边界j板）；同理得j的变化。因为对于i, j，总有一个是短板（相等则随机取一个即可），每次是短板的就发生变化，因此覆盖了所有情况。
　　从式子area = height[i] * (j-i)开始分析更容易理解，当前面积已经是area，若要使其更大，必须改变i或j的值（式子中也就两个自变量），改变了j的值，（j-i）只会更小，而height[i]没变，那么area就变小。如果改变了i的值，（j-i）也会更小，但是height[i]会变化了，只要height[i]的值比构成当前最大面积的短板更大就有可能使area变大。现在问题转化成寻找一块比i更长的板以试图扩大area，那么从i+1开始往右逐个遍历，首先寻找一个更大的板B，判断height[B]*(j-B)是否大于area，若是，i=B，若否，继续遍历。在每次寻找到一块更高的板时，需要判断是否需要换方向遍历，因为从一开始的假设就是i为短板，若j为短板，则需要将j往左遍历了，理同i（一样是用式子来分析）。
　　每个板最多才遍历一次，所以算法复杂度O(n)，其他算法不一定可行，会超时。若不考虑超时，可以穷举任两板来构成最大面积，当然还可以进行剪枝，比如：当以第i块板来与其左边所有板配合，想要找一个更大的面积，必须板间距必须大于当前area/height[i]，可以剪掉距离i板为area/height[i]以下的一些板的计算。

