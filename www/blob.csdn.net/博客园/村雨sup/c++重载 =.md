# c++重载 = - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include<iostream>
#include<cstring>
using namespace std;

class MyStr
{
private:
    char *name;
    int id;
public:
    MyStr() {}
    MyStr(int _id, char *_name)   //constructor
    {
        cout << "constructor" << endl;
        id = _id;
        name = new char[strlen(_name) + 1];//因为结尾有/0,所以要加1
        strcpy(name, _name);
    }
    MyStr(const MyStr& str)     //拷贝构造函数
    {
        cout << "copy constructor" << endl;
        id = str.id;
        if (name != NULL)
            delete name;
        name = new char[strlen(str.name) + 1];
        strcpy(name, str.name);
    }
    MyStr& operator =(const MyStr& str)//赋值运算符
    {
        cout << "operator =" << endl;
        if (this != &str) //自赋值检测
        {
            if (name != NULL) //覆盖数据
                delete name;
            this->id = str.id;
            int len = strlen(str.name);
            name = new char[len + 1];
            strcpy(name, str.name);
        }
        return *this;
    }
    ~MyStr()
    {
        delete name;
    }
};

int main()
{
    MyStr str1(1, "hhxx");//构造函数
    cout << "====================" << endl;
    MyStr str2;
    str2 = str1;//重载
    cout << "====================" << endl;
    MyStr str3 = str2;//拷贝构造
    return 0;
}
```

————

运行结果

```
constructor
====================
operator =
====================
copy constructor
```

___











