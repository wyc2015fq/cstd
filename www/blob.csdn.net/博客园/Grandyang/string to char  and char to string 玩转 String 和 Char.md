# string to char*  and  char* to string 玩转 String 和 Char* - Grandyang - 博客园







# [string to char*  and  char* to string 玩转 String 和 Char*](https://www.cnblogs.com/grandyang/p/4123477.html)







char 类型是c语言中常见的一个数据类型，string是c++中的一个，它的定义为 Strings are objects that represent sequences of characters. 由此可见string是一个char序列的对象，有时候我们经常需要混用这两个数据类型，所以有些常见的错误还是需要避免的：



比如，string的append函数只能加const char类型的，而push_back函数可以加char类型的，也可以直接用 += 来添加char类型的，如果要同时添加两个char, 比如 char a, char b, 则不能写为 str += a + b, 只能老老实实写成 str = str + a + b;



下面我们来看它们之间的相互转换：



****- char* to string****

```
char *cStr = "C++";
std::string Str = std::string(cStr);
```



****- string to char*　****

```
std::string str = "string";
const char *cstr = str.c_str();
```



由于cstr是const的，不能被修改，如果想要修改的话，可以另拷贝一份再修改：

```
std::string str = "string";
char *cstr = new char[str.length() + 1];
strcpy(cstr, str.c_str());
// do something here!
delete [] cstr;
```














