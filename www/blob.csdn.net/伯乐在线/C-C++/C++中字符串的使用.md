# C++中字符串的使用 - 文章 - 伯乐在线
原文出处： [乞力马扎罗的雪](http://blog.csdn.net/chenyufeng1991/article/details/52266035)
当我们一开始使用C语言来处理字符串的时候，会感觉非常的麻烦。C语言中缺少相应的字符串处理函数，如果想要实现某个字符串功能，只能靠我们自己来实现。但是当来到C++中，字符串的处理就会变得异常简单。今天我们就来学习一下C++中最高频的字符串处理函数。示例代码上传至：[https://github.com/chenyufeng1991/CppString](https://github.com/chenyufeng1991/CppString)。
首先要引入C++中的字符串头文件：

C++
```
#include <string>
```
请注意，这里的头文件是没有.h的，否则就成了C语言中的头文件了。
（1）创建字符串
创建字符串有好几种构造方式，最典型的方式就是使用复制构造函数，


```
string str("chenyufeng",3);  
cout << str << endl;
```
cout 拷贝原先的字符串开头处的3个字符最为最新的字符串。打印结果为che.

C++
```
string str2("chenyufeng",2,3);  
cout << str2 << endl;
```
cout 拷贝原先字符串index=2开始处的3个字符最为新的字符串。打印结果为eny。

C++
```
// = :字符串赋值  
str2 = "Robert";  
cout << str2 << endl;
```
同样也可以用直接赋值的方式为某个变量赋值字符串，使用”=“。打印结果为Robert.
（2）swap:交换两个字符串的值

C++
```
// swap:交换两个字符串的值  
   string string1 = "chen";  
   string string2 = "yufeng";  
   swap(string1, string2);  
   cout << "string1 = " << string1 << ";string2 = " << string2 << endl;
```
打印结果就已经和原先的字符串的值交换了。
（3）+，append ：添加字符串

C++
```
// += ,append：在尾部添加字符串  
    string stringOrigin = "chen";  
    string stringAppend = "yufeng";  
    stringOrigin = stringOrigin + stringAppend;  
    cout << "stringOrigin = " << stringOrigin << endl;  
  
    stringOrigin.append("_OK");  
    cout << "stringOriginAppend = " << stringOrigin << endl;
```
注意，添加字符串操作是会修改原先的字符串的。可以直接使用+号进行字符串的添加，非常方便。
（4）insert：在指定位置插入字符串

C++
```
// insert：在指定position插入字符串  
    string stringInsertOrigin = "chenyufeng";  
    stringInsertOrigin.insert(3, "__");  
    cout << "stringInsertOrigin = " << stringInsertOrigin << endl;
```
上述代码可以在indx=3位置插入__下划线，打印结果为 che__nyufeng.
（5）erase，clear删除字符串

C++
```
// erase: 删除字符  
   string stringEraseOrigin = "chenyufeng";  
   stringEraseOrigin.erase();  
   cout << "stringEraseOrigin = " << stringEraseOrigin << endl;  
  
  
   // clear ：删除全部字符  
   string stringClearOrigin = "chenyufeng";  
   stringClearOrigin.clear();  
   cout << "stringClearOrigin = " << stringClearOrigin << endl;
```
上述操作其实都是把字符串清空了。
（6）replace：替换字符串

C++
```
// replace: 替换字符串,某个pos位置开始的size个字符替换成后面的“”字符串  
    string stringReplaceOrigin = "chenyufeng";  
    stringReplaceOrigin.replace(3, 2, "66");  
    cout << "stringReplaceOrigin = " << stringReplaceOrigin << endl;
```
上述代码把字符串从index=3开始的2个字符替换成”66“，打印结果为che66ufeng.
（7）==，,  =: 比较字符串大小
C++中使用这种运算符对字符串进行操作，其实都是用了运算符重载。字符串比较大小是根据字母的字典序或者说是ASCII码值按顺序比较大小。直到比较出两个字符串的不同字母或者比较到某个字符串的最后一位停止。

C++
```
// ==，<,>,<=,>=:比较字符串  
   string stringLeft = "zhen";  
   string stringRight = "yufeng";  
   if (stringLeft == stringRight)  
   {  
       cout << "equal" << endl;  
   }  
  
   if (stringLeft != stringRight)  
   {  
       cout << "not equal" << endl;  
   }  
  
   if (stringLeft < stringRight)  
   {  
       cout << "stringLeft < stringRight" << endl;  
   }  
  
   if (stringLeft > stringRight)  
   {  
       cout << "stringLeft > stringRight" << endl;  
   }
```
（8）size,length:计算字符串长度
这里的计算字符串长度和C语言中不同，是不包括末尾的的，计算的是真实的长度。

C++
```
// size(), length():计算字符串长度  
    string stringCount = "chenyufeng";  
    cout << "stringSize = " << stringCount.size() << endl;  
    cout << "stringLength = " << stringCount.length() << endl;
```
上述的打印结果都是10.
（9）empty：判断字符串是否为空

C++
```
// empty():判断字符串是否为空  
    string stringIsEmpty = "";  
    string stringNotEmpty = "chen";  
    if (stringIsEmpty.empty())  
    {  
        cout << "stringIsEmpty == empty" << endl;  
    }  
    else  
    {  
        cout << "stringIsEmpty != empty" << endl;  
    }  
  
    if (stringNotEmpty.empty())  
    {  
        cout << "stringNotEmpty == empty" << endl;  
    }  
    else  
    {  
        cout << "stringNotEmpty != empty" << endl;  
    }
```
（10）字符串的输入输出流

C++
```
// 输入输出stream  
    cout << "请输入一个字符串"<<endl;  
    string stringInput;  
    cin >> stringInput;  
    cout << "stringInput = " << stringInput << endl;
```
字符串也可以类似于C++其他数据类型一样使用输入输出流。可以使用回车键结束输入流。
（11）max_size:字符串的最大可容纳量。

C++
```
// max_size:  
    string stringMaxSize;  
    cout << "stringMaxSize = " << stringMaxSize.max_size() << endl;
```
打印结果为：18446744073709551599  。表示该字符串可以容纳这么多的字符数。
（12）[], at :元素存取与修改

C++
```
// [],at() :元素存取  
    string stringAt = "chenyufeng";  
    cout << "stringAt[3] = " <<stringAt[3] << endl;  
    cout << "stringAt.at(3) = " << stringAt.at(3) << endl;  
  
    stringAt[3] = '6';  
    stringAt.at(5) = '9';  
    cout << "stringAt = " << stringAt << endl;
```
字符串可以和数组一样进行操作，使用下标进行存取，并可以进行修改原字符串。
（13）compare:字符串的比较，返回0，1 ，-1。

C++
```
// compare()  
    string stringCompare = "chenyufeng";  
    int aaa = stringCompare.compare("chen"); // > 0  
    int bbb = stringCompare.compare("chenyufeng"); // == 0  
    int ccc = stringCompare.compare("done"); // < 0  
    cout << "aaa = " << aaa << ";bbb = " << bbb << ";ccc = " << ccc << endl;
```
（14）substr:取子字符串

C++
```
// substr  
   string stringSubstr = "chenyufeng";  
   // 从索引为4开始的3个字符  
   cout << "stringSubstr.substr(4,3) = " << stringSubstr.substr(4,3) << endl;  
  
   // 从索引为4开始的所有字符  
   cout << "stringSubstr.substr(4) = " <<stringSubstr.substr(4) << endl;  
  
   // 整个字符  
   cout << "stringSubstr.substr() = " <<stringSubstr.substr() << endl;
```
（15）find：查找某个字符

C++
```
// find  
  
    string stringFind = "chenyufeng";  
    stringFind.find('n');  
    cout << "stringFind.find('n') = " << stringFind.find('n') << endl;  
  
    cout << "stringFind.find_first_of('e') = " << stringFind.find_first_of('e') << endl;  
      
    cout << "stringFind.find_last_of('e') = " << stringFind.find_last_of('e') << endl;
```
默认find函数是返回某个字符第一次出现的下标index。find_first_of和find_last_of则分别是第一次和最后一次出现某个字符的index。
上述15个C++中的字符串处理函数是最为常见的，当然其他还有不少，我会在在后续的使用中继续补充。string其实也是STL中的一部分。关于字符串函数的使用更为具体的为官方文档：[http://en.cppreference.com/w/cpp/string/basic_string](http://en.cppreference.com/w/cpp/string/basic_string) 。
