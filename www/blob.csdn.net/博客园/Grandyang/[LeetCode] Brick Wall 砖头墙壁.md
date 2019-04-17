# [LeetCode] Brick Wall 砖头墙壁 - Grandyang - 博客园







# [[LeetCode] Brick Wall 砖头墙壁](https://www.cnblogs.com/grandyang/p/6697067.html)







There is a brick wall in front of you. The wall is rectangular and has several rows of bricks. The bricks have the same height but different width. You want to draw a vertical line from the top to the bottom and cross the leastbricks.

The brick wall is represented by a list of rows. Each row is a list of integers representing the width of each brick in this row from left to right.

If your line go through the edge of a brick, then the brick is not considered as crossed. You need to find out how to draw the line to cross the least bricks and return the number of crossed bricks.

You cannot draw a line just along one of the two vertical edges of the wall, in which case the line will obviously cross no bricks.

Example:
Input: 
[[1,2,2,1],
 [3,1,2],
 [1,3,2],
 [2,4],
 [3,1,2],
 [1,3,1,1]]
Output: 2
Explanation: 

![](https://leetcode.com/static/images/problemset/brick_wall.png)



Note:
- The width sum of bricks in different rows are the same and won't exceed INT_MAX.
- The number of bricks in each row is in range [1,10,000]. The height of wall is in range [1,10,000]. Total number of bricks of the wall won't exceed 20,000.



这道题给了我们一个砖头墙壁，上面由不同的长度的砖头组成，让我们选个地方从上往下把墙劈开，使得被劈开的砖头个数最少，前提是不能从墙壁的两边劈，这样没有什么意义。我们使用一个哈希表来建立每一个断点的长度和其出现频率之间的映射，这样只要我们从断点频率出现最多的地方劈墙，损坏的板砖一定最少，参见代码如下：



```
class Solution {
public:
    int leastBricks(vector<vector<int>>& wall) {
        int mx = 0;
        unordered_map<int, int> m;
        for (auto a : wall) {
            int sum = 0;
            for (int i = 0; i < a.size() - 1; ++i) {
                sum += a[i];
                ++m[sum];
                mx = max(mx, m[sum]);
            }
        }
        return wall.size() - mx;
    }
};
```



下面想说的内容和本题无关，是博主关于前日发生的亚裔医生被暴力拖下飞机恶性事件的看法，不明真相的观众可以自行搜索事件的来龙去脉。说实话，一位年近七旬的医生居然遭受如此的屈辱，身为华人的博主有种感同身受的感觉。一直以来，亚裔在大家的心中就是随和，不爱闹事，不惹事的老好人的形象，说难听一些，就是软弱好欺负的沉默人群。为什么犯罪分子喜欢抢劫亚裔人群，为什么机组人员喜欢‘随机’挑选亚裔人员，为什么执法人员对于手无寸铁的老汉毫无顾忌的暴力拖拽？还不是因为在他们的认知与印象中，亚裔的刻板印象Stereotype就是软弱可欺，欺负了他们也不会怎么样。他们敢随意这么对待其他族群吗？除了我们亚裔遇事喜欢忍气吞声之外，还有有种事不关己高高挂起的态度，而且在外面的华人有时也并不一定会得到国内同胞的理解与支持，从一些国内新闻平台下的留言就可以感觉到，这使得原本就不是铁板一块的亚裔团体的处境更加艰难。所以，我们需要发声，我们不能再做沉默的羔羊！从事后UA的声明以及其CEO的一系列言行，我完全感觉不到这家航空公司对于自己的反思和对受害者的歉意。博主只是一介草民，并不是能呼风唤雨的大人物，但是仍然义愤填膺，仍然感到深深的愤怒与不满，博主已于昨天开始停止使用Mileageplus卡了，并且会近期内注销掉，里面几万点的积分也打算放弃了，并且以后会尽可能的不坐UA。博主知道就算用题目中板砖墙中的所有板砖也拍不醒装睡的人。韩寒说过，小孩才分对错，大人只看利弊。今天UA的股价已经开始跌了，祝其一跌至底，不跌到肉疼他们是不会承认自己的过失的。博主不会强求大家去抵制UA，每个人都有自己的消费选择权，博主只是希望所有亚裔都可以为改变刻板印象做出些许努力，那么未来是值得期待的～



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












