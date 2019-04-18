# 1040 有几个PAT——C/C++实现 - 心纯净，行致远 - CSDN博客





2018年11月02日 01:24:47[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：93








> 
1040 有几个PAT （25 分）

字符串 `APPAPT` 中包含了两个单词 `PAT`，其中第一个 `PAT` 是第 2 位(`P`)，第 4 位(`A`)，第 6 位(`T`)；第二个 `PAT` 是第 3 位(`P`)，第 4 位(`A`)，第 6 位(`T`)。

现给定字符串，问一共可以形成多少个 `PAT`？

### 输入格式：

输入只有一行，包含一个字符串，长度不超过10​5​​，只包含 `P`、`A`、`T` 三种字母。

### 输出格式：

在一行中输出给定字符串中包含多少个 `PAT`。由于结果可能比较大，只输出对 1000000007 取余数的结果。

### 输入样例：

```
APPAPT
```

### 输出样例：

```
2
```


**思路：**

**思路1：这道题可以倒着看，T之前的A的数量可以得到AT的数量（这里注意A前一定要有P），A之前P的数量可以得到PA的数量，因此PAT的数量应该是A之前P的数量之和和T之前A的数量之和的积。**

思路2：[PAT Basic 1040. 有几个PAT（25）（C语言实现）](https://www.jianshu.com/p/65291de1906b)

从前向后扫描：
- 每个**A对应的PA组合数量**是A之前**P的数量**，
- 每个**T对应的PAT组合数量**是T之前所有**A对应的PA组合数量**的累加，
- **所有的PAT组合数量**是所有**T对应的PAT组合数量**的累加

```
#include <stdio.h> 
#define LIM 1000000007 
int main() { 
  unsigned int P = 0, PA = 0, PAT = 0; 
  char c; 
  while((c = getchar()) != '\n') { 
    if(c == 'P') P++; 
    if(c == 'A') PA = (PA + P) % LIM; 
    if(c == 'T') PAT = (PAT + PA) % LIM; 
  } 
  printf("%d", PAT); 
  return 0; }
```

思路3：[1040. 有几个PAT（25）-PAT乙级真题](https://www.liuchuo.net/archives/573)

累加每个A两边P和T的个数之积。

**分析：要想知道构成多少个PAT，那么遍历字符串后对于每一A，它前面的P的个数和它后面的T的个数的乘积就是能构成的PAT的个数。然后把对于每一个A的结果相加即可～～辣么就简单啦，只需要先遍历字符串数一数有多少个T～～然后每遇到一个T呢～～countt-- ；每遇到一个P呢，countp++;然后一遇到字母A呢就countt * countp～～把这个结果累加到result中～～最后输出结果就好啦~~对了别忘记要对10000000007取余哦～～**

```cpp
#include <iostream>
#include <string>
using namespace std;
int main() {
    string s;
    cin >> s;
    int len = s.length(), result = 0, countp = 0, countt = 0;
    for (int i = 0; i < len; i++) {
        if (s[i] == 'T')
            countt++;
    }
    for (int i = 0; i < len; i++) {
        if (s[i] == 'P') countp++;
        if (s[i] == 'T') countt--;
        if (s[i] == 'A') result = (result + countp * countt) % 1000000007;
    }
    cout << result;
    return 0;
}
```

关于为什么要对**10000000007取余，请戳：[为什么要对1000000007取模（取余）](https://blog.csdn.net/zhanshen112/article/details/84871155)**



