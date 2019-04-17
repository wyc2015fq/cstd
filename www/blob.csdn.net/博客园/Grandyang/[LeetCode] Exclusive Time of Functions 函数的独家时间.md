# [LeetCode] Exclusive Time of Functions 函数的独家时间 - Grandyang - 博客园







# [[LeetCode] Exclusive Time of Functions 函数的独家时间](https://www.cnblogs.com/grandyang/p/7244767.html)







Given the running logs of n functions that are executed in a nonpreemptive single threaded CPU, find the exclusive time of these functions.

Each function has a unique id, start from 0 to n-1. A function may be called recursively or by another function.

A log is a string has this format : `function_id:start_or_end:timestamp`. For example, `"0:start:0"` means function 0 starts from the very beginning of time 0. `"0:end:0"` means function 0 ends to the very end of time 0.

Exclusive time of a function is defined as the time spent within this function, the time spent by calling other functions should not be considered as this function's exclusive time. You should return the exclusive time of each function sorted by their function id.

Example 1:
Input:
n = 2
logs = 
["0:start:0",
 "1:start:2",
 "1:end:5",
 "0:end:6"]
Output:[3, 4]
Explanation:
Function 0 starts at time 0, then it executes 2 units of time and reaches the end of time 1. 
Now function 0 calls function 1, function 1 starts at time 2, executes 4 units of time and end at time 5.
Function 0 is running again at time 6, and also end at the time 6, thus executes 1 unit of time. 
So function 0 totally execute 2 + 1 = 3 units of time, and function 1 totally execute 4 units of time.



Note:
- Input logs will be sorted by timestamp, NOT log id.
- Your output should be sorted by function id, which means the 0th element of your output corresponds to the exclusive time of function 0.
- Two functions won't start or end at the same time.
- Functions could be called recursively, and will always end.
- 1 <= n <= 100



这道题让我们函数的独家运行的时间，没错，exclusive就是要翻译成独家，要让每个函数都成为码农的独家记忆～哈～根据题目中给的例子，我们可以看出来，当一个函数start了之后，并不需要必须有end，可以直接被另一个程序start的时候强行关闭。而且，在某个时间点上调用end时，也不需要前面非得调用start，可以直接在某个时间点来个end，这样也算执行了1秒，得+1秒～咳咳，本站禁“苟”，请勿轻易吟诗。博主自以为了解了这个题的逻辑，自己写了一个，结果跪在了下面这个test case：

2
["0:start:0","0:start:2","0:end:5","1:start:7","1:end:7","0:end:8"]

Expected:
[8,1]

这个结果很confusing啊，你想啊，函数0运行了时间点0，1，2，3，4，5，8，共7秒，函数1运行了时间点7，共1秒，为啥答案不是[7,1]而是[8,1]呢？

根据分析网上大神们的解法，貌似时间点6还是函数0在执行。这是为啥呢，说明博主之前的理解有误，当函数0在时间点2时再次开启时，前面那个函数0应该没有被强制关闭，所以现在实际上有两个函数0在执行，所以当我们在时间点5关掉一个函数0时，还有另一个函数0在跑，所以时间点6还是函数0的，还得给函数0续1秒。这样才能解释的通这个case啊。这样的话用栈stack就比较合适了，函数开启了就压入栈，结束了就出栈，不会有函数被漏掉。这样的我们可以遍历每个log，然后把三部分分开，函数idx，类型type，时间点time。如果此时栈不空，说明之前肯定有函数在跑，那么不管当前时start还是end，之前函数时间都得增加，增加的值为time - preTime，这里的preTime是上一个时间点。然后我们更新preTime为当前时间点time。然后我们判断log的类型，如果是start，我们将当前函数压入栈；如果是end，那么我们将栈顶元素取出，对其加1秒，并且preTime也要加1秒，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> res(n, 0);
        stack<int> st;
        int preTime = 0;
        for (string log : logs) {
            int found1 = log.find(":");
            int found2 = log.find_last_of(":");
            int idx = stoi(log.substr(0, found1));
            string type = log.substr(found1 + 1, found2 - found1 - 1);
            int time = stoi(log.substr(found2 + 1));
            if (!st.empty()) {
                res[st.top()] += time - preTime;
            }
            preTime = time;
            if (type == "start") st.push(idx);
            else {
                auto t = st.top(); st.pop();
                ++res[t];
                ++preTime;
            }
        }
        return res;
    }
};
```



下面这种方法比较叼的地方是在于使用了C语言的sscanf函数来一步读取了三个变量，注意这里面的"[^:]"，表示copy所有字符，直到遇到':'，这样就能把中间的start或者end拷到type中去了。而且接下来的写法跟上面也不太相同，这里先判断了type的类型，如果是start，那么再看如果栈不为空，那么栈顶函数加上时间差，这个上面讲过了，然后将当前函数压入栈；如果是end，那么栈顶元素加上时间差，还要再加1秒，这个在上面也提到了加了1秒的事，然后再将栈顶元素出栈。最后更新preTime为当前时间点。讲解中加了这么多秒，博主已经尽力了。。。



解法二：

```
class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> res(n, 0);
        stack<int> st;
        int preTime = 0, idx = 0, time = 0;
        char type[10];
        for (string log : logs) {
            sscanf(log.c_str(), "%d:%[^:]:%d", &idx, type, &time);
            if (type[0] == 's') {
                if (!st.empty()) {
                    res[st.top()] += time - preTime;
                }
                st.push(idx);
            } else {
                res[st.top()] += ++time - preTime;
                st.pop();
            }
            preTime = time;
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/96120/simple-c-using-stack](https://discuss.leetcode.com/topic/96120/simple-c-using-stack)

[https://discuss.leetcode.com/topic/96068/java-stack-solution-o-n-time-o-n-space](https://discuss.leetcode.com/topic/96068/java-stack-solution-o-n-time-o-n-space)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












