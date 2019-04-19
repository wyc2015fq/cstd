# std string 的常用使用 - xqhrs232的专栏 - CSDN博客
2012年07月05日 10:46:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：649标签：[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
 原文地址::[http://www.cnblogs.com/highmayor/archive/2008/01/02/1023015.html](http://www.cnblogs.com/highmayor/archive/2008/01/02/1023015.html)
 相关网帖
### 1.[std::string用法详解](http://blog.csdn.net/linux5blue19/article/details/4146684)----[http://blog.csdn.net/linux5Blue19/article/details/4146684](http://blog.csdn.net/linux5Blue19/article/details/4146684)
用 string来代替char * 数组，使用[sort排序算法](http://www.stlchina.org/twiki/bin/view.pl/Main/STLSortAlgorithms)来排序，用[unique
 函数](http://www.stlchina.org/stl_doc/unique.html)来去重
1、Define
        string s1 = "hello";
        string s2 = "world";
        string s3 = s1 + "," + s2 +"!\n";
2、append
        s1 += ",shanshan\n";
3、Compare
        if(s1 == s2)
           .....
        else if(s1 == "hello")
           .....
4、 string 重载了许多操作符，包括 +, +=, <, `=, `, [], <<, >>等，正式这些操作符，对字符串操作非常方便
#include <string>
#include <iostream>
usingnamespace std;
int main(){
string strinfo="Please input your name:";
cout << strinfo ;
cin >> strinfo;
if( strinfo == "winter" )
cout << "you are winter!"<<endl;
elseif( strinfo != "wende" )
cout << "you are not wende!"<<endl;
elseif( strinfo < "winter")
cout << "your name should be ahead of winter"<<endl;
else 
cout << "your name should be after of winter"<<endl;
strinfo += " , Welcome to China!";
cout << strinfo<<endl;
cout <<"Your name is :"<<endl;
string strtmp = "How are you? " + strinfo;
for(int i = 0 ; i < strtmp.size(); i ++)
cout<<strtmp[i];
return 0;
} 
5、find函数
由于查找是使用最为频繁的功能之一，string 提供了非常丰富的查找函数。其列表如下：
|函数名|描述|
|----|----|
|find|查找|
|rfind|反向查找|
|find_first_of|查找包含子串中的任何字符，返回第一个位置|
|find_first_not_of|查找不包含子串中的任何字符，返回第一个位置|
|find_last_of|查找包含子串中的任何字符，返回最后一个位置|
|find_last_not_of|查找不包含子串中的任何字符，返回最后一个位置|
以上函数都是被重载了4次，以下是以find_first_of 函数为例说明他们的参数，其他函数和其参数一样，也就是说总共有24个函数：
size_type find_first_of(const basic_string& s, size_type pos = 0)
size_type find_first_of(const charT* s, size_type pos, size_type n)
size_type find_first_of(const charT* s, size_type pos = 0)
size_type find_first_of(charT c, size_type pos = 0)
所有的查找函数都返回一个size_type类型，这个返回值一般都是所找到字符串的位置，如果没有找到，则返回string::npos。
其实string::npos表示的是-1。即没找到就返回-1。例子如下：
#include <string>
#include <iostream>
usingnamespace std;
int main(){
string strinfo="   //*---Hello Word!......------";
string strset="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int first = strinfo.find_first_of(strset);
if(first == string::npos) { 
cout<<"not find any characters"<<endl;
return -1;
} 
int last = strinfo.find_last_of(strset);
if(last == string::npos) { 
cout<<"not find any characters"<<endl;
return -1;
} 
cout << strinfo.substr(first, last - first + 1)<<endl;//string.substr是子串
return 0;
}
6、insert函数, replace函数和erase函数
string只是提供了按照位置和区间的replace函数，而不能用一个string字串来替换指定string中的另一个字串。
例子：
#include <string>
#include <iostream>
usingnamespace std;
int main() {
string strinfo="This is Winter, Winter is a programmer. Do you know Winter?";
cout<<"Orign string is :\n"<<strinfo<<endl;
string_replace(strinfo, "Winter", "wende");
cout<<"After replace Winter with wende, the string is :\n"<<strinfo<<endl;
return 0;
}
string.erase(pos,srclen);//srclen是删除的长度
string.insert(pos,strdst); //pos是定位，strdst是插入的函数
void string_replace(string & strBig, const string & strsrc, const string &strdst) {
string::size_type pos=0;
string::size_type srclen=strsrc.size();
string::size_type dstlen=strdst.size();
while( (pos=strBig.find(strsrc, pos)) != string::npos){
strBig.erase(pos, srclen);
strBig.insert(pos, strdst);
pos += dstlen;
}
}
相关链接：http://www.stlchina.org/twiki/bin/view.pl/Main/STLDetailString
 
7、切割字符串
#include <sstream>
#include <string>
#include <iostream>
using namespace std;
int main()
{
string text = "big|dog|china|sonic|free";
stringstream ss(text);
string sub_str;
while(getline(ss,sub_str,'|'))  //以|为间隔分割test的内容
cout << sub_str << endl;
return 0;
}
输出如下：
big
dog
china
sonic
free
8、构造函数和析构函数
string s                  生成一个空字符串S
string s(str)             Copy构造函数，生成字符串Str的一个复制品
string s(str,stridx)      将字符串Str内始于位置Stridx的部分，当作字符串S的初值
string s(str,stridx,strlen)  将字符串Str内始于位置Stridx且长度为strlen的部分，当作字符串S的初值
string s(cstr)            以C-String cstr作为S的初值
string s(num,c)           生成一个字符串,包含Num个C字符
string s(beg,end)         以区间[beg,end]内的字符作为s初值
s.~string()               销毁所有字符,释放内存
注意:
std::string s('x');//error
std::string s(1,'x'); //ok,create a string that has one charactor 'x'
9、substr(string.substr的方法)
#### （1）参数
#### **start**:Number -- 一个整数，指示 my_str 中用于创建子字符串的第一个字符的位置。如果 start 为一个负数，则起始位置从字符串的结尾开始确定，其中 -1 表示最后一个字符。
#### **length**:Number -- 要创建的子字符串中的字符数。如果没有指定 length，则子字符串包括从字符串开头到字符串结尾的所有字符。
#### （2）返回
#### String -- 指定字符串的子字符串。
#### （3）示例
#### 下面的示例创建一个新字符串 my_str，并使用 substr() 返回该字符串中的第二个单词；首先，使用正的 start 参数，然后使用负的 start 参数：
#### var my_str:String = new String("Hello world");var mySubstring:String = new String();mySubstring = my_str.substr(6,5);trace(mySubstring); // 输出：worldmySubstring = my_str.substr(-5,5);trace(mySubstring); // 输出：world
分类: [C++](http://www.cnblogs.com/highmayor/category/105795.html)
