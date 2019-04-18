# c++中string的用法 - 心纯净，行致远 - CSDN博客





2018年11月15日 15:09:32[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：25标签：[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[【C/C++常用函数及头文件】](https://blog.csdn.net/zhanshen112/article/category/8356214)









```cpp
#include<iostream>
#include"string"
#include"algorithm"
using namespace std;

//string的赋值
void f1()
{
    string s1 ="shihao";        //string是一个类
    string s2 ("bbbbbb");
    string s3 = s1;
    string s4(10, 'a');     //等价于string s4 ="aaaaaaaaaa";
    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
}

//string的遍历
void f2()
{
    string s1 = "abcdefg";

    //1:数组方式
    for(int i = 0;  i < s1.length(); i++)
    {
        cout << s1[i] << " ";
    }
    cout << endl;

    //2:迭代器
    for(string::iterator it = s1.begin(); it != s1.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;

    //3  异常处理
    try
    {
        for(int i = 0; i < s1.length() + 3; i++)
        {
            cout << s1.at(i) << " ";   //当出现错误时， 会向外抛出异常
        }
    }
    catch(...)
    {
        cout << "发生异常" << endl;
    }

    try
    {
        for(int i = 0;  i < s1.length() + 3; i++)   //出现错误, 不向外抛出异常
        {
            cout << s1[i] << " ";
        }
    }
    catch(...)
    {
        cout << "发生异常" << endl;
    }
}

//字符指针和sting的转换
void f4()
{
    //1:
    string s1 = "aaabbb";
    cout << s1.c_str() << endl;

    //2:
    char buf[128] = {0};
    s1.copy(buf, 3, 1);    //拷贝3个字符，从1个字符开始(位置下标从0开始)    //注：不会自动加上字符串结束标志
    cout << "buf: " << buf << endl;
}

//字符串的连接
void f5()
{
    //1:
    string s1 = "aaa";
    string s2 = "bbb";
    s1 = s1 + s2;
    cout << "s1: " << s1 << endl;

    //2:
    string s3 = "333";
    string s4 = "444";
    s3.append(s4);
    cout << "s3: " << s3 << endl;
}

//字符串的查找和替换
void f6()
{
    string s1 = "wbm hello wbm 111 wbm 222 wbm 333";

    //查找 从查找位置开始第一个出现的下标
    int index = s1.find("wbm", 1);  //位置下标从0开始
    cout <<"index: " << index << endl << endl;

    //查找每一次wbm出现的下标
    int offindex = s1.find("wbm", 0);
    while (offindex != string::npos)   //不等于-1
    {
        cout << "offindex: " << offindex << endl;
        offindex++;
        offindex = s1.find("wbm", offindex);
    }

    //把所有的wbm换成大写
    offindex = s1.find("wbm", 0);
    while (offindex != string::npos)   //不等于-1
    {
        s1.replace(offindex, 3, "WBM");
        offindex++;
        offindex = s1.find("wbm", offindex);
    }
    cout << endl << "s1替换后的结果为： " << s1 << endl;

    //把aaa替换成大写
    string s2 = "aaa bbb ccc";
    s2.replace(0, 3, "AAA");    //从第0个位置开始替换3个
    cout << endl << "s2: " << s2 << endl;
}

//区间删除和插入
void f7()
{
    string s1 = "hello1 hello2 hello1";
    string::iterator it = find(s1.begin(), s1.end(), 'l');
    if (it != s1.end())
    {
        s1.erase(it);   //删除
    }
    cout << "s1删除l以后的结果为：" << s1 << endl;

    s1.erase(s1.begin(), s1.end());
    cout << "s1全部删除：" << s1 << endl;
    cout << "s1的长度为: " << s1.length() << endl;

    //插入
    string s2 = "BBB";
    s2.insert(0, "AAA");
    cout << "s2: " << s2 << endl;
}

//大小写转换
void f8()
{
    string s1 = "AAAbbb";
    transform(s1.begin(), s1.end(), s1.begin(), ::tolower);       //transform(first,last,result,op);
    cout << "s1全部转化为小写为: " << s1 << endl;
    transform(s1.begin(), s1.end(), s1.begin(), ::toupper);       //transform(first,last,result,op);
    cout << "s1全部转化为大写为: " << s1 << endl;
}

int main()
{
    f8();
    return 0;
}
```

转载自：[c++中string的用法](https://blog.csdn.net/Duke10/article/details/78344226/)



