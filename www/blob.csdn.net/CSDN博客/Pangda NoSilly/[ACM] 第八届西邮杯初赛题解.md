# [ACM] 第八届西邮杯初赛题解 - Pangda NoSilly - CSDN博客

2017年06月18日 14:28:43[hepangda](https://me.csdn.net/hepangda)阅读数：1085


# Problem A: 加法变乘法
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
已知X可以写成从1开始连续若干个整数的和， 现在要求把其中两个不相邻的加号变成乘号，使得结果为Y。
找出所有满足条件的可能答案并输出（把两个乘号左边的数字用小括号括起来，中间用英文逗号间隔，两个括号之间不空格）；
若找不到满足的条件，则输出“NONE”字样。编写程序，完成n组数据的判定。

例如：当X为1225，Y为2015时
因为：1+2+3+ ... + 49 = 1225
     且1+2+3+...+10*11+12+...+27*28+29+...+49 = 2015
所以：一个解为(10,27)。
```

### Input
`第一行为n值，接下来依次n行的第一个数据是加法结果X，第二个数据是变乘法后的结果Y，以空格间隔。`
### Output

```
输出n行，每一行的格式为“(***,***)(***,***)”（或者“NONE”）。请严格按照
格式书写，不能出现其它文字或符号。
```

### Sample Input

```
3
1225 2015
1224 2015
1275 2065
```

### Sample Output

```
(10,27)(16,24)
NONE
(10,27)(16,24)
```

### Analysis

`简单题，枚举尝试即可。`

### Code

```cpp
#include<iostream>
#include<vector>
#include<map>
using namespace std;

int main() {
    int cases;
    cin >> cases;

    auto solve = [](int x, int y) -> vector<pair<int, int> > {
        int tmp = 0, buf = 0;
        vector<pair<int, int> > ret;

        for (int i = 2; i < 47; i++) {
            tmp = buf = x - (i * 2 - 1) + i * (i - 1);
            for (int j = i + 2; j < 50; j++) {
                buf = tmp - (j * 2 - 1) + j * (j - 1);
                if (buf == y) {
                    ret.push_back(make_pair<int, int>(i - 1, j - 1));
                }
            }
        }
        return ret;
    };

    while (cases--) {
        int x, y;
        cin >> x >> y;
        auto solutions = solve(x, y);
        if (solutions.size() == 0) {
            cout << "NONE" << endl;
        } else {
            for (auto i : solutions) {
                cout << '(' << i.first << ',' << i.second << ')';
            }
            cout << endl;
        }
    }
    return 0;
}
```

# Problem B: 核桃的数量
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
小张是软件项目经理，他带领3个开发组。工期紧，今天都在加班呢。
为鼓舞士气，小张打算给每个组发一袋核桃（据传言能补脑）。
他的要求是：
（1）各组的核桃数量必须相同；
（2）各组内必须能平分核桃（当然是不能打碎的）
（3）尽量提供满足1,2条件的最小数量(节约闹[和谐]嘛)
```

### Input
`输入包含三个正整数a,b,c，表示每个组正在加班的人数，用空格分开 (a,b,c均小于30)。`
### Output
`输出一个正整数，表示每袋中核桃的数量，不要输出多余的信息。`
### Sample Input
`30 12 9`
### Sample Output
`180`
### Analysis

简单题，求三个数的最小公倍数。

### Code

```cpp
#include<iostream>
#include<algorithm>
using namespace std;

int main() {
    int a, b, c;

    auto lcm = [](const int a, const int b) -> int {
        int larger = max(a, b), smaller = min(a, b);
        int ret = larger;
        while (ret % smaller != 0)
            ret += larger;
        return ret;
    };

    cin >> a >> b >> c;
    cout << lcm(lcm(a, b), c) << endl;
    return 0;
}
```

# Problem C: 移动距离

找规律题，略。

# Problem D: 翻硬币
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
小明正在玩一个“翻硬币”的游戏，桌上放着排成一排的若干硬币。
我们用 * 表示正面， 用 o 表示反面（是小写字母，不是零）。
例如，可能情形是：**oo***oooo，
如果同时翻转左边的两个硬币，则变为：oooo***oooo。

现在小明的问题是：如果已知了初始状态和要达到的目标状态，每次只能同时翻转相邻的两个硬币
那么对特定的局面，最少要翻动多少次呢？
我们约定：把翻动相邻的两个硬币叫做一步操作。
```

### Input
`两行等长的字符串，分别表示初始状态和要达到的目标状态，每行的长度<1000。`
### Output
`一个整数，表示最小操作步数。`
### Sample Input

```
**********
o****o****
```

### Sample Output
`5`
### Analysis

简单题，每次只翻最左侧的那一个，模拟计数即可。

### Code

```
#include<iostream>
#include<string>

using namespace std;

int main() {
    int res = 0;
    string from, to;
    getline(cin, from);
    getline(cin, to);

    for (int i = 0; i < to.size() - 1; i++) {
        if (from[i] != to[i]) {
            from[i] = to[i];
            if (from[i + 1] == '*') {
                from[i + 1] = 'o';
            } else {
                from[i + 1] = '*';
            }
            res++;
        }
    }
    cout << res << endl;
    return 0;
}
```

# Problem E: 字符统计

简单题，只用判断有多少标点就可以判定出有多少单词，略。

# Problem F: 格式打印

坑题，题目描述不清，没有任何价值，略。

# Problem G: 可逆素数
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
若将某一素数的各位数字顺序颠倒后得到的数仍然是素数，则次素数称为可逆素数。
判断给定的n个数据是否是可逆素数。
```

### Input
`第一行为n值，第二行输入n个数字，以空格间隔。`
### Output

```
输出n行，每一行的格式为【***是可逆素数】
（或者【***是素数，但不是可逆素数】，用中文逗号隔开，或者【***不是素数】）。
请严格按照格式书写，不能出现其它文字或符号。
特别说明：待判断的数据个数不超过10个。
```

### Sample Input

```
3
23 31 18
```

### Sample Output

```
23是素数，但不是可逆素数
31是可逆素数
18不是素数
```

### Analysis

```cpp
很好玩的一个题，由于基本上没有时间复杂度限制，利用stringstream实现数字倒转再判断即可。
```

### Code

```cpp
#include<iostream>
#include<sstream>
#include<string>

using namespace std;
const size_t MAX = 1000000;

bool isprime[MAX];

void init() {
    for (int i = 2; i < MAX; i++) {
        isprime[i] = true;
    }
    isprime[0] = isprime[1] = false;

    for (int i = 2; i < MAX; i++) {
        if (isprime[i]) {
            for (int j = i + i; j < MAX; j += i) {
                isprime[j] = false;
            }
        }
    }
}

int judge(int i) {
    int revnum;
    string buf, tmp;
    stringstream io;

    io << i;
    io >> buf;
    for (auto iter = buf.rbegin(); iter != buf.rend(); iter++) {
        tmp += *iter;
    }
    io.clear();
    io << tmp;
    io >> revnum;

    return isprime[revnum];
}

int main() {
    init();
    int cases;
    cin >> cases;
    while (cases--) {
        int t;
        cin >> t;
        if (isprime[t]) {
            cout << t << ((judge(t)) ? "是可逆素数" : "是素数，但不是可逆素数") << endl;
        } else {
            cout << t << "不是素数" << endl;
        }
    }
    return 0;
}
```

# Problem H: 星系炸弹
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
在X星系的广袤空间中漂浮着n个X星人造“炸弹”，
每个炸弹都可以设定多少天之后爆炸。
例如：阿尔法炸弹2015年1月1日放置，定时为15天，则它在2015年1月16日，星期五爆炸。
```

### Input
`第一行为n值，以后连续n行为炸弹放置日期（格式为 年-月-日）和定时天数（整型）。`
### Output

```
输出n行，每行为爆炸的准确日期（格式为 yyyy年mm月dd日 星期几），
日期和星期之间用一个空格隔开。
请严格按照格式书写，不能出现其它文字或符号。

提示信息：星期的数据集合是
【星期日、星期一、星期二、星期三、星期四、星期五、星期六】。
```

### Sample Input

```
2
1999-9-9 800
2014-11-9 1000
```

### Sample Output

```
2001年11月17日 星期六
2017年08月05日 星期六
```

### Analysis

日期加减，简单模拟即可

### Code

```cpp
#include<iostream>
#include<iomanip>
#include<string>

using namespace std;
static const string week[7] = {
    "星期一", "星期二", "星期三", "星期四", "星期五", "星期六", "星期日"
};
static const int days[2][13] = {
    {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} ,
    {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

struct dates {
    int y, m, d;
    const string dayofweek() {
        int cy = y, cm = m, cd = d;
        if (cm == 1 || cm == 2)
           cm += 12, cy--;
        int ret = (cd + 2 * cm + 3 * (cm + 1) / 5 + cy + cy / 4 - cy / 100 + cy / 400) % 7;
        return week[ret];
    }
};

void adddate(dates &it, const int how) {
    for (int i = 0; i < how; i++) {
        it.d++;
        int run = (it.y % 400 == 0 || (it.y % 4 == 0 && it.y % 100 != 0)) ? 1 : 0;
        if (it.d > days[run][it.m])
            it.d = 1, it.m++;
        if (it.m > 12)
            it.m = 1, it.y++;
    }
}

int main() {
    int cases;
    cin >> cases;
    while (cases--) {
        dates it;
        char trash;
        int day;
        cin >> it.y >> trash >> it.m >> trash >> it.d >> day;
        adddate(it, day);
        cout << it.y << "年" << setw(2) << setfill('0') << it.m << "月" << setw(2) << setfill('0') << it.d << "日 " << it.dayofweek() << endl;
    }
    return 0;
}
```

# Problem I: 特殊回文数
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
123321是一个非常特殊的数，它从左边读和从右边读是一样的。
输入一个正整数n， 编程求所有这样的五位和六位十进制数，满足各位数字之和等于n(1<=n<=54)。
```

### Input
`输入一个正整数n。`
### Output

```
若特殊回文数的个数<=10，则按从小到大的顺序输出满足条件的特殊回文数，每个数字占一行。
若特殊回文数的个数>10，则仅输出总个数。
```

### Sample Input
`52`
### Sample Output

```
899998
989989
998899
```

### Analysis
`简单题，模拟即可。`
### Code

```cpp
#include<iostream>

using namespace std;

int main() {
    int n, cnt = 0, num[200] { 0 };
    cin >> n;
    for (int i = 1; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int t = n - (2 * i) - (2 * j);
            if (t >= 0 && t <= 9) {
                num[cnt++] = i * 10000 + j * 1000 + t * 100 + j * 10 + i;
            }
        }
    }

    for (int i = 1; i< 10; i++) {
        for (int j = 0; j < 10; j++) {
            int t = n - (2 * i) - (2 * j);
            if (t / 2 >= 0 && t / 2 <= 9 && t % 2 == 0) {
                num[cnt++] = i * 100000 + j * 10000 + (t / 2) * 1000 + (t / 2) * 100 + j * 10 + i;
            }
        }
    }

    if (cnt > 10) {
        cout << cnt << endl;
    } else {
        for (int i = 0; i < cnt; i++) {
            cout << num[i] << endl;
        }
    }
}
```

# Problem J: 特大整数的精确相加和相减
`Time Limit: 10 Sec              Memory Limit: 256 MB`
### Description

```
特大整数用长整型也存不下，如果用双精度实型存储则会造成误差，
可以用字符数组存储所有位，再按十进制由低到高逐位相加，同时考虑进位。
特别提示：假设特大整数不超过30位。参与操作的数据中，被减数>减数。
```

### Input
`第一行待运算的表达式个数n，之后连续的2n行每相邻得两行为一组。`
### Output

```
依次输出运算结果，共输出2n行。
前n行为相加的运算结果；
后n行为相减的运算结果，每个结果独占一行。
```

### Sample Input

```
3
123456789
23456789
999999999
999999999
1000000000
9999
```

### Sample Output

```
146913578
1999999998
1000009999
100000000
0
999990001
```

### Analysis
`大数加减题，Java破之。`
### Code

```java
import java.math.BigInteger;
import java.util.*;

public class Main {
    public static void main(String args[]) {
        Scanner cin = new Scanner(System.in);
        int cases = cin.nextInt();
        BigInteger[] addResult = new BigInteger[1000],
                subResult = new BigInteger[1000];

        for (int i = 0; i < cases; i++) {
            BigInteger a = cin.nextBigInteger(),
                    b = cin.nextBigInteger();
            addResult[i] = a.add(b);
            subResult[i] = a.subtract(b);
        }

        for (int i = 0; i < cases; i++) {
            System.out.println(addResult[i]);
        }

        for (int i = 0; i < cases; i++) {
            System.out.println(subResult[i]);
        }

        cin.close();
    }
}
```

### 友情链接

还可以参看 [http://blog.csdn.net/YinJianxiang/article/details/73409777](http://blog.csdn.net/YinJianxiang/article/details/73409777) 的题解～

