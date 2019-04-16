# POJ 3348 Cows | 凸包——童年的回忆（误） - 胡小兔 - 博客园







# [POJ 3348 Cows | 凸包——童年的回忆（误）](https://www.cnblogs.com/RabbitHu/p/POJ3348.html)





想当年……还是邱神给我讲的凸包来着……

```
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#define eps 0.000000001
#define enter putchar('\n')
#define space putchar(' ')
using namespace std;
typedef long long ll;
template <class T>
void read(T &x){
    char c;
    bool op = 0;
    while(c = getchar(), c < '0' || c > '9')
    if(c == '-') op = 1;
    x = c - '0';
    while(c = getchar(), c >= '0' && c <= '9')
    x = x * 10 + c - '0';
    if(op) x = -x;
}
template <class T>
void write(T x){
    if(x < 0) putchar('-'), x = -x;
    if(x >= 10) write(x / 10);
    putchar('0' + x % 10);
}
const int N = 10005, INF = 0x3f3f3f3f;
int n, top, ans;
struct point {
    int x, y;
    point(){}
    point(int _x, int _y): x(_x), y(_y){}
    point operator - (point b) const{
    return point(x - b.x, y - b.y);
    }
    int operator * (point b){
    return x * b.y - y * b.x;
    }
    int norm(){
    return x * x + y * y;
    }
} p[N], stk[N], root;
bool operator < (const point &a, const point &b){
    int det = (a - root) * (b - root);
    if(det) return det > 0;
    return (a - root).norm() < (b - root).norm();
}
void graham(){
    int pos = 0;
    root.x = root.y = INF;
    for(int i = 1; i <= n; i++)
    if(p[i].x < root.x || (p[i].x == root.x && p[i].y < root.y))
        root = p[i], pos = i;
    if(pos != 1) swap(p[pos], p[1]);
    sort(p + 2, p + n + 1);
    stk[top = 1] = root;
    for(int i = 2; i <= n; i++){
    while(top >= 2 && (stk[top] - stk[top - 1]) * (p[i] - stk[top - 1]) < 0) top--;
    stk[++top] = p[i];
    }
}
void getarea(){
    stk[top + 1] = stk[1];
    for(int i = 1; i <= top; i++)
    ans += stk[i] * stk[i + 1];
}
int main(){
    read(n);
    for(int i = 1; i <= n; i++)
    read(p[i].x), read(p[i].y);
    graham();
    getarea();
    write(ans / 2 / 50), enter;
    return 0;
}
```












