# C++中的C_str()函数用法 - 数据之美的博客 - CSDN博客
2017年09月25日 12:24:28[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：154
个人分类：[算法																[数据结构](https://blog.csdn.net/oppo62258801/article/category/6551324)](https://blog.csdn.net/oppo62258801/article/category/6551325)
语法: 
const char *c_str();
c_str()函数返回一个指向正规C字符串的指针常量, 内容与本string串相同. 
这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
注意：一定要使用strcpy()函数 等来操作方法c_str()返回的指针 
比如：最好不要这样: 
char* c; 
string s="1234"; 
c = s.c_str(); //c最后指向的内容是垃圾，因为s对象被析构，其内容被处理，同时，编译器也将报错——将一个const char *赋与一个char *。
应该这样用： 
char c[20]; 
string s="1234"; 
strcpy(c,s.c_str()); 
这样才不会出错，c_str()返回的是一个临时指针，不能对其进行操作
再举个例子
c_str() 以 char* 形式传回 string 内含字符串
如果一个函数要求char*参数，可以使用c_str()方法： 
string s = "Hello World!";
printf("%s", s.c_str()); //输出 "Hello World!"
