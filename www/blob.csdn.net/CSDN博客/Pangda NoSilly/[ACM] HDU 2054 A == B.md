# [ACM] HDU 2054 A == B? - Pangda NoSilly - CSDN博客

2017年05月25日 18:56:05[hepangda](https://me.csdn.net/hepangda)阅读数：271


```cpp
#include<iostream>
#include<string>

using namespace std;

string numlize(string n);

int main(void)
{
    string n1, n2;
    while (cin >> n1 >> n2) {
        if (numlize(n1) == numlize(n2))
            cout << "YES" << endl;
        else
            cout << "NO" << endl;
    }
    return 0;
}

string numlize(string n)
{
    int pos, i;

    pos = n.find('.');
    if (pos >= 0) {
        for (i = n.size() - 1; i >= pos - 1; i--) {
            if (n[i] == '0')
                n = n.erase(n.size() - 1, 1);
            else
                break;
        }
    }

    if (pos == -1)
        pos = n.size() - 1;
    for (i = 0; i <= pos; i++) {
        if (n[0] == '0')
            n = n.erase(0, 1);
        else
            break;
    }

    pos = n.find('.');
    if (pos == n.size() - 1)
        n = n.erase(pos, 1);

    return n;
}
```

