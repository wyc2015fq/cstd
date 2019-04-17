# c++类 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include <bits/stdc++.h>
using namespace std;
const int maxn = 1000;
const double pi = 3.1415926;

class Point
{
public:
    Point(int xx = 0,int yy = 0)
    {
        X = xx;
        Y = yy;
        cnt++;
    }
    Point(Point &p) //拷贝构造函数
    {
        X = p.X;
        Y = p.Y;
        cnt++;  //在拷贝构造函数中也是会构造函数的
    }
    int getx(){ return X;}
    int gety(){ return Y;}
    static void getc(){cout << "id = "<< cnt << endl;} //静态全局函数

private:
    int X,Y;
    static int cnt;
};

int Point::cnt = 0;//静态全局数这样定义

int main()
{
    Point a(4,5);
    cout << "point a:" << a.getx() << "," <<a.gety() << endl;
    a.getc();
    Point b(a);//一个拷贝的例子
    cout << "point b:" << b.getx() << "," <<b.gety() << endl;
    b.getc();
    a.getc();
    Point::getc();
    return 0;
}
```

——











