# nyoj——297（期望） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)






## GoroSort

时间限制：3000 ms  |  内存限制：65535 KB

难度：4



- 描述

Goro has 4 arms. Goro is very strong. You don't mess with Goro. Goro needs to sort an array of **N** different integers. Algorithms are not Goro's strength; strength is Goro's strength. Goro's plan is to use the fingers on two of his hands to hold down several elements of the array and hit the table with his third and fourth fists as hard as possible. This will make the unsecured elements of the array fly up into the air, get shuffled randomly, and fall back down into the empty array locations.

Goro wants to sort the array as quickly as possible. How many hits will it take Goro to sort the given array, on average, if he acts intelligently when choosing which elements of the array to hold down before each hit of the table? Goro has an infinite number of fingers on the two hands he uses to hold down the array.

More precisely, before each hit, Goro may choose any subset of the elements of the array to freeze in place. He may choose differently depending on the outcomes of previous hits. Each hit permutes the unfrozen elements uniformly at random. Each permutation is equally likely.


- 输入
The first line of the input gives the number of test cases, T. T test cases follow. Each one will consist of two lines. The first line will give the number N. The second line will list the N elements of the array in their initial order.
1 ≤ T ≤ 100;
The second line of each test case will contain a permutation of the N smallest positive integers.
1 ≤ N ≤ 1000;- 输出
For each test case, output one line containing "Case #x: y", where x is the case number (starting from 1) and y is the expected number of hit-the-table operations when following the best hold-down strategy. Answers with an absolute or relative error of at most 10-6 will be considered correct.- 样例输入
3
2
2 1
3
1 3 2
4
2 1 4 3- 样例输出
Case #1: 2.000000
Case #2: 2.000000
Case #3: 4.000000
- 提示
In test case #3, one possible strategy is to hold down the two leftmost elements first. Elements 3 and 4 will be free to move. After a table hit, they will land in the correct order [3, 4] with probability 1/2 and in the wrong order [4, 3] with probability 1/2. Therefore, on average it will take 2 hits to arrange them in the correct order. After that, Goro can hold down elements 3 and 4 and hit the table until 1 and 2 land in the correct order, which will take another 2 hits, on average. The total is then 2 + 2 = 4 hits.- 来源
[Google Code Jam 2011 资格赛](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=Google+Code+Jam+2011+%E8%B5%84%E6%A0%BC%E8%B5%9B)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)
```
#include "bits/stdc++.h"
using namespace std;

int main()
{

    int t;
    scanf("%d",&t);
    int k = 1;
    while(t--){
        int n;
        scanf("%d",&n);
        int cnt = 0;
        int x;
        for(int i=1;i <= n;i++){
            scanf("%d",&x);
            if(i != x) cnt++;             //如果位置不是本来的位置就加1
        }
        cout << "Case #" << k++ << ": " << cnt << ".000000" << endl;

    }
    return 0;
}
```

大概意思就是 假设N个数组，里面全部都是没有排序好的，那么拍一次，对于数组中任意的数字，拍一次，它落回正确位置的概率为1/N。假设，拍完一次，有I个数字落回了原来的位置，那么对于没有落回原来位置的数字肯定没有落在这I个数字的位置上，如果落在了这I个数字的上面，则这I个数字肯定就是错误的，因此概率为(N-I)/N，接下来，按住I个正确的，拍一次，落回原来位置的概率为1/N-I,两者相乘的概率依然为1/N，因此一个数组正确排序的期望为整个数组中没有正确排序的数字。











