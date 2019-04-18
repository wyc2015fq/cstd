# C++文本操作.Vs.Python - wishchinYang的专栏 - CSDN博客
2014年03月18日 22:34:27[wishchin](https://me.csdn.net/wishchin)阅读数：519
个人分类：[C++编程](https://blog.csdn.net/wishchin/article/category/1508333)
C++利用文件流：
（1）：读取一个字符
```cpp
std::string TestTxt(argv[3]);  //
    freopen(TestTxt.c_str(),"r",stdin);// 打开文件！
    float ViewValue;
    cin >>ViewValue;  // 利用 重定向输入 浮点值
```
（2）：读取一个单词
```cpp
std::string word;
    while(cin>> word){
        cout<<word<<endl;//
    }
```
（3）：读取一行——（ 一行空格也算一行）
```cpp
std::string   line;
        getline(cin,line);//得到的一行为字符串！
```
Python文本操作:[http://blog.csdn.net/lixiang0522/article/details/7751851](http://blog.csdn.net/lixiang0522/article/details/7751851)
