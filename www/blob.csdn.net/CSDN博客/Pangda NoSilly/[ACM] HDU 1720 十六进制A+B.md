# [ACM] HDU 1720 十六进制A+B - Pangda NoSilly - CSDN博客

2017年01月17日 17:22:41[hepangda](https://me.csdn.net/hepangda)阅读数：397


Update 3 

整个程序可以改写为以下更加简单的形式

```cpp
#include<iostream>
#include<iomanip>

using namespace std;

int main() {
    int a, b;
    while (cin >> hex >> a >> b) {
        cout << a + b << endl;
    }
    return 0;
}
```

—————————————————————————————— 

Update 2 

bit_hextodec可以改写为以下更简单的形式

```
int bit_hextodec(char s) {
    if ('0' <= s && s <= '9')
        return s - '0';
    else if ('a' <= s && s <= 'z')
        return s - 'a' + 10;
    else
        return s - 'A' + 10;
}
```

——————————————————————————————

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
using namespace std;

int bit_hextodec(char s);
int hextodec(char *st);

int main(void)
{
    char *st1 = (char*)malloc(sizeof(char*)), *st2 = (char*)malloc(sizeof(char*));
    int a, b;
    while (~scanf("%s %s", st1, st2)) {
        a = hextodec(st1);
        b = hextodec(st2);
        printf("%d\n", a + b);
    }
}
int hextodec(char *st)
{
    int len = strlen(st), i,j;
    int s = 0;
    for (i = len,j=0; i > 0; i--,j++) {
        s += pow(16, i-1) * bit_hextodec(st[j]);
    }
    return s;
}

int bit_hextodec(char s)
{
    char *st = &s;
    if (atoi(st) != 0 || st[0] == '0')
        return atoi(st);
    else {
        switch (st[0]){
        case 'a':case'A':
            return 10;
        case 'b':case'B':
            return 11;
        case 'c':case'C':
            return 12;
        case 'd':case'D':
            return 13;
        case 'e':case'E':
            return 14;
        case 'f':case'F':
            return 15;
        }
    }
}
```

