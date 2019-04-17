# [LeetCode] Implement Rand10() Using Rand7() 使用Rand7()来实现Rand10() - Grandyang - 博客园







# [[LeetCode] Implement Rand10() Using Rand7() 使用Rand7()来实现Rand10()](https://www.cnblogs.com/grandyang/p/9727206.html)







Given a function `rand7` which generates a uniform random integer in the range 1 to 7, write a function `rand10` which generates a uniform random integer in the range 1 to 10.

Do NOT use system's `Math.random()`.




Example 1:
Input: 1
Output: [7]


Example 2:
Input: 2
Output: [8,4]


Example 3:
Input: 3
Output: [8,1,10]



Note:
- `rand7` is predefined.
- Each testcase has one argument: `n`, the number of times that `rand10` is called.



Follow up:
- What is the [expected value](https://en.wikipedia.org/wiki/Expected_value) for the number of calls to `rand7()` function?
- Could you minimize the number of calls to `rand7()`?






这道题给了我们一个随机生成 [1, 7] 内数字的函数rand7()，让我们利用其来生成一个能随机生成 [1, 10] 内数字的函数rand10()，注意这里的随机生成的意思是等概率生成范围内的数字。这是一道很有意思的题目，由于rand7()只能生成1到7之间的数字，所以8，9，10这三个没法生成，那么怎么办？大多数人可能第一个想法就是，再用一个呗，然后把两次的结果加起来，范围不就扩大了么，扩大成了 [2, 14] 之间，然后我们如果再减去1，范围不就是[1, 13]了么。想法不错，但是有个问题，这个范围内的每个数字生成的概率不是都相等的，为啥这么说呢，我们来举个简单的例子看下，就比如说rand2()，那么我们知道其可以生成两个数字1和2，且每个的概率都是1/2。那么对于 (rand2() - 1) + rand2()呢，我们看一下：

```
rand2() - 1 + rand()2  =   ?
   1            1          1
   1            2          2
   2            1          2
   2            2          3
```

我们发现，生成数字范围 [1, 3] 之间的数字并不是等概率大，其中2出现的概率为1/2，1和3分别为1/4。那么这就不随机了。问题出在哪里了呢，如果直接相加，那么不同组合可能会产生相同的数字，比如 1+2 和 2+1 都是3。所以我们需要给第一个rand2() 升一个维度，让其乘上一个数字，再相加。比如对于 (rand2() - 1) * 2 + rand2()，如下：

```
（rand2() - 1） * 2 + rand()2  =   ?
     1                  1         1
     1                  2         2
     2                  1         3
     2                  2         4
```

这时右边生成的1，2，3，4就是等概率出现的了。这样我们就通过使用rand2()，来生成rand4()了。那么反过来想一下，我们可以通过rand4()来生成rand2()，其实更加简单，我们只需通过 rand4() % 2 + 1 即可，如下：

```
rand4() % 2 + 1 =  ?
   1               2
   2               1
   3               2
   4               1
```

同理，我们也可以通过 rand6() 来生成 rand2()，我们只需通过 rand6() % 2 + 1 即可，如下：

```
rand6() % 2 + 1 =  ?
   1               2
   2               1
   3               2
   4               1
   5               2
   6               1
```

所以，回到这道题，我们可以先凑出 rand10*N()，然后再通过 rand10*N() % 10 + 1 来获得rand10()。那么，我们只需要将rand7()转化为rand10*N()即可，根据前面的讲解，我们转化也必须要保持等概率，那么就可以变化为 (rand7() - 1) * 7 + rand7()，就转为了 rand49()。但是49不是10的倍数，不过49包括好几个10的倍数，比如40，30，20，10等。这里，我们需要把rand49() 转为 rand40()，需要用到[拒绝采样Rejection Sampling](https://en.wikipedia.org/wiki/Rejection_sampling)，总感觉名字很奇怪，之前都没有听说过这个采样方法，刷题也是个不停学习新东西的过程呢。简单来说，这种采样方法就是随机到需要的数字就接受，不是需要的就拒绝，并重新采样，这样还能保持等概率，具体的证明这里就不讲解了，博主也不会，有兴趣的童鞋们可以去Google一下～ 这里我们直接用结论就好啦，当我们用 rand49() 生成一个 [1, 49] 范围内的随机数，如果其在 [1, 40] 范围内，我们就将其转为rand10()范围内的数字，直接对10去余并加1，返回即可。如果不是，则继续循环即可，参见代码如下：



解法一：

```
class Solution {
public:
    int rand10() {
        while (true) {
            int num = (rand7() - 1) * 7 + rand7();
            if (num <= 40) return num % 10 + 1;
        }
    }
};
```



我们可以不用while循环，而采用调用递归函数，从而两行就搞定，叼不叼～



解法二：

```
class Solution {
public:
    int rand10() {
        int num = (rand7() - 1) * 7 + rand7();
        return (num <= 40) ? (num % 10 + 1) : rand10();
    }
};
```



我们还可以对上面的解法进行一下优化，因为说实话在 [1, 49] 的范围内随机到 [41, 49] 内的数字概率还是挺高的，我们可以做进一步的处理，就是当循环到这九个数字的时候，我们不重新采样，而是做进一步的处理，我们将采样到的数字减去40，这样就相当于有了个 rand9()，那么我们通过 (rand9() - 1) * 7 + rand7()，可以变成rand63()，对 rand63() 进行拒绝采样，得到 rand60()，从而又可以得到 rand10()了，此时还会多余出3个数字，[61, 63]，我们通过减去60，得到 rand3()，再通过变换 (rand3() - 1) * 7 + rand7() 得到 rand21()，此时再次调用拒绝采样，得到 rand20()，进而得到 rand10()，此时就只多余出一个21，重复整个循环的概率就变的很小了，参见代码如下：



解法三：

```
class Solution {
public:
    int rand10() {
        while (true) {
            int a = rand7(), b = rand7();
            int num = (a - 1) * 7 + b;
            if (num <= 40) return num % 10 + 1;
            a = num - 40, b = rand7();
            num = (a - 1) * 7 + b;
            if (num <= 60) return num % 10 + 1;
            a = num - 60, b = rand7();
            num = (a - 1) * 7 + b;
            if (num <= 20) return num % 10 + 1;
        }
    }
};
```



类似题目：

[Generate Random Point in a Circle](https://www.cnblogs.com/grandyang/p/9741220.html)



参考资料：

[https://leetcode.com/problems/implement-rand10-using-rand7/](https://leetcode.com/problems/implement-rand10-using-rand7/)

[https://leetcode.com/problems/implement-rand10-using-rand7/discuss/152282/C%2B%2B-2-line](https://leetcode.com/problems/implement-rand10-using-rand7/discuss/152282/C%2B%2B-2-line)

[https://leetcode.com/problems/implement-rand10-using-rand7/discuss/175450/Java-Solution-explain-this-problem-with-2D-matrix](https://leetcode.com/problems/implement-rand10-using-rand7/discuss/175450/Java-Solution-explain-this-problem-with-2D-matrix)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












