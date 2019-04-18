# [ACM] POJ 3087 Shuffle'm Up - Pangda NoSilly - CSDN博客

2017年05月25日 19:17:23[hepangda](https://me.csdn.net/hepangda)阅读数：198
个人分类：[ACM](https://blog.csdn.net/hepangda/article/category/6685366)



```cpp
#include<iostream>
#include<string>
#include<map>

using namespace std;

const int MAXDATA = 100000;

map<string, bool> vis;

void shuffle(string &r, string &s1, string &s2) {
    r.clear();
    string::iterator iter, jter;
    for (iter = s1.begin(), jter = s2.begin(); iter != s1.end(); ++iter, ++jter) {
        r += *jter;
        r += *iter;
    }

}

void update(string t, string &s1, string &s2) {
    s1.clear(); s2.clear();
    size_t jg = t.size();
    for (int i = 0; i < jg / 2; i++) {
        s1 += t[i];
    }
    for (int i = jg / 2; i < jg; i++) {
        s2 += t[i];
    }
}

int main() {
    int __;
    cin >> __;
    for (int i = 1; i <= __; i++) {
        string s1, s2, des;
        string t;
        int trash;
        cin >> trash >> s1 >> s2 >> des;
        int R = 0;
        while (++R) {
            shuffle(t, s1, s2);
            if (t == des)
                break;
            if (vis.find(t) != vis.end()) {
                R = -1;
                break;
            }
            vis[t] = true;
            update(t, s1, s2);
        }
        cout << i << " " << R << endl;
    }
    return 0;
}
```

