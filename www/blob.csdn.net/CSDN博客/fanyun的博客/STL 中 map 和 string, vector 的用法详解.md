# STL 中 map 和 string, vector 的用法详解 - fanyun的博客 - CSDN博客
2017年02月26日 21:11:21[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：826
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
1. map 用法详解
std map是STL的一个关联容器，它提供一对一（其中第一个可以称为关键字，每个关键字只能在map中出现一次，第二个可能称为该关键字的值）的数据处理能力，由于这个特性，它完成有可能在我们处理一对一数据的时候，在编程上提供快速通道。这里说下std map内部数据的组织，std map内部自建一颗红黑树(一种非严格意义上的平衡二叉树)，这颗树具有对数据自动排序的功能，所以在std map内部所有的数据都是有序的，后边我们会见识到有序的好处。
下面举例说明什么是一对一的数据映射。比如一个班级中，每个学生的学号跟他的姓名就存在着一一映射的关系，这个模型用map可能轻易描述，很明显学号用int描述，姓名用字符串描述(本篇文章中不用char *来描述字符串，而是采用STL中string来描述),下面给出map描述代码：
Map<int, string> mapStudent;
1. map的构造函数
map共提供了6个构造函数，这块涉及到内存分配器这些东西，略过不表，在下面我们将接触到一些map的构造方法，这里要说下的就是，我们通常用如下方法构造一个map：
Map<int, string> mapStudent;
2. 数据的插入
在构造map容器后，我们就可以往里面插入数据了。这里讲三种插入数据的方法：
第一种：用insert函数插入pair数据，下面举例说明(以下代码虽然是随手写的，应该可以在VC和GCC下编译通过，大家可以运行下看什么效果，在VC下请加入这条语句，屏蔽4786警告＃pragma warning (disable:4786) )
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第二种：用insert函数插入value_type数据，下面举例说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(map<int, string>::value_type (1, “student_one”));
mapStudent.insert(map<int, string>::value_type (2, “student_two”));
mapStudent.insert(map<int, string>::value_type (3, “student_three”));
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第三种：用数组方式插入数据，下面举例说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent[1] = “student_one”;
mapStudent[2] = “student_two”;
mapStudent[3] = “student_three”;
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
以上三种用法，虽然都可以实现数据的插入，但是它们是有区别的，当然了第一种和第二种在效果上是完成一样的，用insert函数插入数据，在数据的插入上涉及到集合的唯一性这个概念，即当map中有这个关键字时，insert操作是插入数据不了的，但是用数组方式就不同了，它可以覆盖以前该关键字对应的值，用程序说明
mapStudent.insert(map<int, string>::value_type (1, “student_one”));
mapStudent.insert(map<int, string>::value_type (1, “student_two”));
上面这两条语句执行后，map中1这个关键字对应的值是“student_one”，第二条语句并没有生效，那么这就涉及到我们怎么知道insert语句是否插入成功的问题了，可以用pair来获得是否插入成功，程序如下
Pair<map<int, string>::iterator, bool> Insert_Pair;
Insert_Pair = mapStudent.insert(map<int, string>::value_type (1,“student_one”));
我们通过pair的第二个变量来知道是否插入成功，它的第一个变量返回的是一个map的迭代器，如果插入成功的话Insert_Pair.second应该是true的，否则为false。
下面给出完成代码，演示插入成功与否问题
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
Pair<map<int, string>::iterator, bool> Insert_Pair;
Insert_Pair ＝ mapStudent.insert(pair<int, string>(1, “student_one”));
If(Insert_Pair.second == true)
{
Cout<<”Insert Successfully”<<endl;
}
Else
{
Cout<<”Insert Failure”<<endl;
}
Insert_Pair ＝ mapStudent.insert(pair<int, string>(1, “student_two”));
If(Insert_Pair.second == true)
{
Cout<<”Insert Successfully”<<endl;
}
Else
{
Cout<<”Insert Failure”<<endl;
}
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
大家可以用如下程序，看下用数组插入在数据覆盖上的效果
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent[1] = “student_one”;
mapStudent[1] = “student_two”;
mapStudent[2] = “student_three”;
map<int, string>::iterator iter;
for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
3. map的大小
在往map里面插入了数据，我们怎么知道当前已经插入了多少数据呢，可以用size函数，用法如下：
Int nSize = mapStudent.size();
4. 数据的遍历
这里也提供三种方法，对map进行遍历
第一种：应用前向迭代器，上面举例程序中到处都是了，略过不表
第二种：应用反相迭代器，下面举例说明，要体会效果，请自个动手运行程序
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::reverse_iterator iter;
for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)
{
Cout<<iter->first<<” ”<<iter->second<<end;
}
}
第三种：用数组方式，程序说明如下
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
int nSize = mapStudent.size()
//此处有误，应该是 for(int nIndex = 1; nIndex <= nSize; nIndex++) 
//by rainfish
for(int nIndex = 0; nIndex < nSize; nIndex++)
{
Cout<<mapStudent[nIndex]<<end;
}
}
5. 数据的查找（包括判定这个关键字是否在map中出现）
在这里我们将体会，map在数据插入时保证有序的好处。
要判定一个数据（关键字）是否在map中出现的方法比较多，这里标题虽然是数据的查找，在这里将穿插着大量的map基本用法。
这里给出三种数据查找方法
第一种：用count函数来判定关键字是否出现，其缺点是无法定位数据出现位置,由于map的特性，一对一的映射关系，就决定了count函数的返回值只有两个，要么是0，要么是1，出现的情况，当然是返回1了
第二种：用find函数来定位数据出现位置，它返回的一个迭代器，当数据出现时，它返回数据所在位置的迭代器，如果map中没有要查找的数据，它返回的迭代器等于end函数返回的迭代器，程序说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
map<int, string>::iterator iter;
iter = mapStudent.find(1);
if(iter != mapStudent.end())
{
Cout<<”Find, the value is ”<<iter->second<<endl;
}
Else
{
Cout<<”Do not Find”<<endl;
}
}
第三种：这个方法用来判定数据是否出现，是显得笨了点，但是，我打算在这里讲解
Lower_bound函数用法，这个函数用来返回要查找关键字的下界(是一个迭代器)
Upper_bound函数用法，这个函数用来返回要查找关键字的上界(是一个迭代器)
例如：map中已经插入了1，2，3，4的话，如果lower_bound(2)的话，返回的2，而upper-bound（2）的话，返回的就是3
Equal_range函数返回一个pair，pair里面第一个变量是Lower_bound返回的迭代器，pair里面第二个迭代器是Upper_bound返回的迭代器，如果这两个迭代器相等的话，则说明map中不出现这个关键字，程序说明
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent[1] = “student_one”;
mapStudent[3] = “student_three”;
mapStudent[5] = “student_five”;
map<int, string>::iterator iter;
iter = mapStudent.lower_bound(2);
{
//返回的是下界3的迭代器
Cout<<iter->second<<endl;
}
iter = mapStudent.lower_bound(3);
{
//返回的是下界3的迭代器
Cout<<iter->second<<endl;
}
iter = mapStudent.upper_bound(2);
{
//返回的是上界3的迭代器
Cout<<iter->second<<endl;
}
iter = mapStudent.upper_bound(3);
{
//返回的是上界5的迭代器
Cout<<iter->second<<endl;
}
Pair<map<int, string>::iterator, map<int, string>::iterator>mapPair;
mapPair = mapStudent.equal_range(2);
if(mapPair.first == mapPair.second)
{
cout<<”Do not Find”<<endl;
}
Else
{
Cout<<”Find”<<endl;
}
mapPair = mapStudent.equal_range(3);
if(mapPair.first == mapPair.second)
{
cout<<”Do not Find”<<endl;
}
Else
{
Cout<<”Find”<<endl;
}
}
6. 数据的清空与判空
清空map中的数据可以用clear()函数，判定map中是否有数据可以用empty()函数，它返回true则说明是空map
7. 数据的删除
这里要用到erase函数，它有三个重载了的函数，下面在例子中详细说明它们的用法
#include <map>
#include <string>
#include <iostream>
Using namespace std;
Int main()
{
Map<int, string> mapStudent;
mapStudent.insert(pair<int, string>(1, “student_one”));
mapStudent.insert(pair<int, string>(2, “student_two”));
mapStudent.insert(pair<int, string>(3, “student_three”));
//如果你要演示输出效果，请选择以下的一种，你看到的效果会比较好
//如果要删除1,用迭代器删除
map<int, string>::iterator iter;
iter = mapStudent.find(1);
mapStudent.erase(iter);
//如果要删除1，用关键字删除
Int n = mapStudent.erase(1);//如果删除了会返回1，否则返回0
//用迭代器，成片的删除
//一下代码把整个map清空
mapStudent.earse(mapStudent.begin(), mapStudent.end());
//成片删除要注意的是，也是STL的特性，删除区间是一个前闭后开的集合
//自个加上遍历代码，打印输出吧
}
8. 其他一些函数用法
这里有swap,key_comp,value_comp,get_allocator等函数，感觉到这些函数在编程用的不是很多，略过不表，有兴趣的话可以自个研究
9. 排序
这里要讲的是一点比较高深的用法了,排序问题，STL中默认是采用小于号来排序的，以上代码在排序上是不存在任何问题的，因为上面的关键字是int型，它本身支持小于号运算，在一些特殊情况，比如关键字是一个结构体，涉及到排序就会出现问题，因为它没有小于号操作，insert等函数在编译的时候过不去，下面给出两个方法解决这个问题
第一种：小于号重载，程序举例
#include <map>
#include <string>
Using namespace std;
Typedef struct tagStudentInfo
{
Int nID;
String strName;
}StudentInfo, *PStudentInfo; //学生信息
Int main()
{
int nSize;
//用学生信息映射分数
map<StudentInfo, int>mapStudent;
map<StudentInfo, int>::iterator iter;
StudentInfo studentInfo;
studentInfo.nID = 1;
studentInfo.strName = “student_one”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));
studentInfo.nID = 2;
studentInfo.strName = “student_two”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));
for (iter=mapStudent.begin(); iter!=mapStudent.end(); iter++)
cout<<iter->first.nID<<endl<<iter->first.strName<<endl<<iter->second<<endl;
}
以上程序是无法编译通过的，只要重载小于号，就OK了，如下：
Typedef struct tagStudentInfo
{
Int nID;
String strName;
Bool operator < (tagStudentInfo const& _A) const
{
//这个函数指定排序策略，按nID排序，如果nID相等的话，按strName排序
If(nID < _A.nID) return true;
If(nID == _A.nID) return strName.compare(_A.strName) < 0;
Return false;
}
}StudentInfo, *PStudentInfo; //学生信息
第二种：仿函数的应用，这个时候结构体中没有直接的小于号重载，程序说明
#include <map>
#include <string>
Using namespace std;
Typedef struct tagStudentInfo
{
Int nID;
String strName;
}StudentInfo, *PStudentInfo; //学生信息
Classs sort
{
Public:
Bool operator() (StudentInfo const &_A, StudentInfo const &_B) const
{
If(_A.nID < _B.nID) return true;
If(_A.nID == _B.nID) return _A.strName.compare(_B.strName) < 0;
Return false;
}
};
Int main()
{
//用学生信息映射分数
Map<StudentInfo, int, sort>mapStudent;
StudentInfo studentInfo;
studentInfo.nID = 1;
studentInfo.strName = “student_one”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));
studentInfo.nID = 2;
studentInfo.strName = “student_two”;
mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));
}
10. 另外
由于STL是一个统一的整体，map的很多用法都和STL中其它的东西结合在一起，比如在排序上，这里默认用的是小于号，即less<>，如果要从大到小排序呢，这里涉及到的东西很多，在此无法一一加以说明。
还要说明的是，map中由于它内部有序，由红黑树保证，因此很多函数执行的时间复杂度都是log2N的，如果用map函数可以实现的功能，而STL Algorithm也可以完成该功能，建议用map自带函数，效率高一些。
下面说下，map在空间上的特性，否则，估计你用起来会有时候表现的比较郁闷，由于map的每个数据对应红黑树上的一个节点，这个节点在不保存你的数据时，是占用16个字节的，一个父节点指针，左右孩子指针，还有一个枚举值（标示红黑的，相当于平衡二叉树中的平衡因子），我想大家应该知道，这些地方很费内存了吧，不说了……
2. string 用法详解
C++中的string 类 简单介绍
 前言: string 的角色
1 string 使用
1.1 充分使用string 操作符
1.2 眼花缭乱的string find 函数
1.3 string insert, replace, erase 2 string 和 C风格字符串
3 string 和 Charactor Traits
4 string 建议
5 附录前言: string 的角色
C++ 语言是个十分优秀的语言，但优秀并不表示完美。还是有许多人不愿意使用C或者C++，为什么？原因众多，其中之一就是C/C++的文本处理功能太麻烦，用起来很不方便。以前没有接触过其他语言时，每当别人这么说，我总是不屑一顾，认为他们根本就没有领会C++的精华，或者不太懂C++，现在我接触 perl, [PHP](http://lib.csdn.net/base/php), 和Shell脚本以后，开始理解了以前为什么有人说C++文本处理不方便了。
总之，有了string 后，C++的字符文本处理功能总算得到了一定补充，加上配合STL其他容器使用，其在文本处理上的功能已经与perl, shell, php的距离缩小很多了。因此掌握string 会让你的工作事半功倍。
1、 string 使用
其实，string并不是一个单独的容器，只是basic_string 模板类的一个typedef 而已，相对应的还有wstring, 你在string 头文件中你会发现下面的代码:
extern "C++" {
typedef basic_string <char> string;
typedef basic_string <wchar_t> wstring;
} // extern "C++"
由于只是解释string的用法，如果没有特殊的说明，本文并不区分string 和 basic_string的区别。
string 其实相当于一个保存字符的序列容器，因此除了有字符串的一些常用操作以外，还有包含了所有的序列容器的操作。字符串的常用操作包括：增加、删除、修改、查找比较、链接、输入、输出等。详细函数列表参看附录。不要害怕这么多函数，其实有许多是序列容器带有的，平时不一定用的上。
如果你要想了解所有函数的详细用法，你需要查看basic_string，或者下载STL编程手册。这里通过实例介绍一些常用函数。
1.1 充分使用string 操作符
string 重载了许多操作符，包括 +, +=, <, =, , [], <<, >>等，正式这些操作符，对字符串操作非常方便。先看看下面这个例子：tt.cpp（例程2）
＃i nclude <string>
＃i nclude <iostream>
using namespace std;
int main(){
string strinfo="Please input your name:";
cout << strinfo ;
cin >> strinfo;
if( strinfo == "winter" )
cout << "you are winter!"<<endl;
else if( strinfo != "wende" )
cout << "you are not wende!"<<endl;
else if( strinfo < "winter")
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
下面是程序的输出
-bash-2.05b$ make tt
c++ -O -pipe -march=pentiumpro tt.cpp -o tt
-bash-2.05b$ ./tt
Please input your name:Hero
you are not wende!
Hero , Welcome to China!
How are you? Hero , Welcome to China!
有了这些操作符，在STL中仿函数都可以直接使用string作为参数，例如 less, great, equal_to 等，因此在把string作为参数传递的时候，它的使用和int 或者float等已经没有什么区别了。例如，你可以使用：
map<string, int> mymap;
//以上默认使用了 less<string>
有了 operator + 以后，你可以直接连加，例如：
string strinfo="Winter";
string strlast="Hello " + strinfo + "!";
//你还可以这样：
string strtest="Hello " + strinfo + " Welcome" + " toChina" + " !";
看见其中的特点了吗？只要你的等式里面有一个 string 对象，你就可以一直连续"+"，但有一点需要保证的是，在开始的两项中，必须有一项是 string 对象。其原理很简单：
系统遇到"+"号，发现有一项是string 对象。
系统把另一项转化为一个临时 string 对象。
执行 operator + 操作，返回新的临时string 对象。
如果又发现"+"号，继续第一步操作。
由于这个等式是由左到右开始检测执行，如果开始两项都是const char* ，程序自己并没有定义两个const char* 的加法，编译的时候肯定就有问题了。
有了操作符以后，assign(), append(), compare(), at()等函数，除非有一些特殊的需求时，一般是用不上。当然at()函数还有一个功能，那就是检查下标是否合法，如果是使用：
string str="winter";
//下面一行有可能会引起程序中断错误
str[100]='!';
//下面会抛出异常:throws: out_of_range
cout<<str.at(100)<<endl;
了解了吗？如果你希望效率高，还是使用[]来访问，如果你希望稳定性好，最好使用at()来访问。
1.2 眼花缭乱的string find 函数
由于查找是使用最为频繁的功能之一，string 提供了非常丰富的查找函数。其列表如下：
函数名 描述 find 查找 rfind 反向查找find_first_of 查找包含子串中的任何字符，返回第一个位置 find_first_not_of 查找不包含子串中的任何字符，返回第一个位置 find_last_of 查找包含子串中的任何字符，返回最后一个位置 find_last_not_of 查找不包含子串中的任何字符，返回最后一个位置以上函数都是被重载了4次，以下是以find_first_of 函数为例说明他们的参数，其他函数和其参数一样，也就是说总共有24个函数：
size_type find_first_of(const basic_string& s, size_type pos = 0)
size_type find_first_of(const charT* s, size_type pos, size_type n)
size_type find_first_of(const charT* s, size_type pos = 0)
size_type find_first_of(charT c, size_type pos = 0)
所有的查找函数都返回一个size_type类型，这个返回值一般都是所找到字符串的位置，如果没有找到，则返回string::npos。有一点需要特别注意，所有和string::npos的比较一定要用string::size_type来使用，不要直接使用int 或者unsigned int等类型。其实string::npos表示的是-1, 看看头文件：
template <class _CharT, class _Traits, class _Alloc>
const basic_string<_CharT,_Traits,_Alloc>::size_type
basic_string<_CharT,_Traits,_Alloc>::npos
= basic_string<_CharT,_Traits,_Alloc>::size_type) -1;
find 和 rfind 都还比较容易理解，一个是正向匹配，一个是逆向匹配，后面的参数pos都是用来指定起始查找位置。对于find_first_of 和find_last_of 就不是那么好理解。
find_first_of 是给定一个要查找的字符集，找到这个字符集中任何一个字符所在字符串中第一个位置。或许看一个例子更容易明白。
有这样一个需求：过滤一行开头和结尾的所有非英文字符。看看用string 如何实现：
＃i nclude <string>
＃i nclude <iostream>
using namespace std;
int main(){
string strinfo=" //*---Hello Word!......------";
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
cout << strinfo.substr(first, last - first + 1)<<endl;
return 0;
}
这里把所有的英文字母大小写作为了需要查找的字符集，先查找第一个英文字母的位置，然后查找最后一个英文字母的位置，然后用substr 来的到中间的一部分，用于输出结果。下面就是其结果：
Hello Word
前面的符号和后面的符号都没有了。像这种用法可以用来查找分隔符，从而把一个连续的字符串分割成为几部分，达到 shell 命令中的 awk 的用法。特别是当分隔符有多个的时候，可以一次指定。例如有这样的需求：
张三|3456123, 湖南
李四,4564234| 湖北
王小二, 4433253|北京
...
我们需要以 "|" ","为分隔符，同时又要过滤空格，把每行分成相应的字段。可以作为你的一个家庭作业来试试，要求代码简洁。
1.3 string insert, replace, erase
了解了string 的操作符，查找函数和substr，其实就已经了解了string的80%的操作了。insert函数, replace函数和erase函数在使用起来相对简单。下面以一个例子来说明其应用。
string只是提供了按照位置和区间的replace函数，而不能用一个string字串来替换指定string中的另一个字串。这里写一个函数来实现这个功能：
void string_replace(string & strBig, const string & strsrc, conststring &strdst) {
string::size_type pos=0;
string::size_type srclen=strsrc.size();
string::size_type dstlen=strdst.size();
while( (pos=strBig.find(strsrc, pos)) != string::npos){
strBig.replace(pos, srclen, strdst);
pos += dstlen;
}
}看看如何调用：
＃i nclude <string>
＃i nclude <iostream>
using namespace std;
int main() {
string strinfo="This is Winter, Winter is a programmer. Do you knowWinter?";
cout<<"Orign string is :/n"<<strinfo<<endl;
string_replace(strinfo, "Winter", "wende");
cout<<"After replace Winter with wende, the string is:/n"<<strinfo<<endl;
return 0;
}其输出结果：
Orign string is :
This is Winter, Winter is a programmer. Do you know Winter?
After replace Winter with wende, the string is :
This is wende, wende is a programmer. Do you know wende?如果不用replace函数，则可以使用erase和insert来替换，也能实现string_replace函数的功能：
void string_replace(string & strBig, const string & strsrc, conststring &strdst) {
string::size_type pos=0;
string::size_type srclen=strsrc.size();
string::size_type dstlen=strdst.size();
while( (pos=strBig.find(strsrc, pos)) != string::npos){
strBig.erase(pos, srclen);
strBig.insert(pos, strdst);
pos += dstlen;
}
}当然，这种方法没有使用replace来得直接。
2、 string 和 C风格字符串
现在看了这么多例子，发现const char* 可以和string 直接转换，例如我们在上面的例子中，使用
string_replace(strinfo, "Winter", "wende");来代用
void string_replace(string & strBig, const string & strsrc, conststring &strdst) 在[C语言](http://lib.csdn.net/base/c)中只有char* 和 const char*，为了使用起来方便，string提供了三个函数满足其要求：
const charT* c_str() const
const charT* data() const
size_type copy(charT* buf, size_type n, size_type pos = 0) const 其中：
c_str 直接返回一个以/0结尾的字符串。
data 直接以数组方式返回string的内容，其大小为size()的返回值，结尾并没有/0字符。
copy 把string的内容拷贝到buf空间中。
你或许会问，c_str()的功能包含data()，那还需要data()函数干什么？看看源码：
const charT* c_str () const
{ if (length () == 0) return ""; terminate (); return data (); }原来c_str()的流程是：先调用terminate()，然后在返回data()。因此如果你对效率要求比较高，而且你的处理又不一定需要以/0的方式结束，你最好选择data()。但是对于一般的C函数中，需要以const char*为输入参数，你就要使用c_str()函数。
对于c_str() data()函数，返回的数组都是由string本身拥有，千万不可修改其内容。其原因是许多string实现的时候采用了引用机制，也就是说，有可能几个string使用同一个字符存储空间。而且你不能使用sizeof(string)来查看其大小。详细的解释和实现查看Effective STL的条款15：小心string实现的多样性。
另外在你的程序中，只在需要时才使用c_str()或者data()得到字符串，每调用一次，下次再使用就会失效，如：
string strinfo("this is Winter");
...
//最好的方式是:
foo(strinfo.c_str());
//也可以这么用:
const char* pstr=strinfo.c_str();
foo(pstr);
//不要再使用了pstr了, 下面的操作已经使pstr无效了。
strinfo += " Hello!";
foo(pstr);//错误！会遇到什么错误？当你幸运的时候pstr可能只是指向"this is Winter Hello!"的字符串，如果不幸运，就会导致程序出现其他问题，总会有一些不可遇见的错误。总之不会是你预期的那个结果。
3、 string 和 Charactor Traits
了解了string的用法，该详细看看string的真相了。前面提到string 只是basic_string的一个typedef。看看basic_string 的参数：
template <class charT, class traits = char_traits<charT>,
class Allocator = allocator<charT> >
class basic_string
{
//...
}char_traits不仅是在basic_string 中有用，在basic_istream 和 basic_ostream中也需要用到。
就像Steve Donovan在过度使用C++模板中提到的，这些确实有些过头了，要不是系统自己定义了相关的一些属性，而且用了个typedef，否则还真不知道如何使用。
但复杂总有复杂道理。有了char_traits，你可以定义自己的字符串类型。当然，有了char_traits < char > 和char_traits <wchar_t > 你的需求使用已经足够了，为了更好的理解string ，咱们来看看char_traits都有哪些要求。
如果你希望使用你自己定义的字符，你必须定义包含下列成员的结构：表达式描述
char_type 字符类型
int_type int 类型
pos_type 位置类型
off_type 表示位置之间距离的类型
state_type 表示状态的类型
assign(c1,c2) 把字符c2赋值给c1
eq(c1,c2) 判断c1,c2 是否相等
lt(c1,c2) 判断c1是否小于c2
length(str) 判断str的长度
compare(s1,s2,n) 比较s1和s2的前n个字符
copy(s1,s2, n) 把s2的前n个字符拷贝到s1中
move(s1,s2, n) 把s2中的前n个字符移动到s1中
assign(s,n,c) 把s中的前n个字符赋值为c
find(s,n,c) 在s的前n个字符内查找c
eof() 返回end-of-file
to_int_type(c) 将c转换成int_type
to_char_type(i) 将i转换成char_type
not_eof(i) 判断i是否为EOF
eq_int_type(i1,i2) 判断i1和i2是否相等
想看看实际的例子，你可以看看sgi STL的char_traits结构源码.
现在默认的string版本中，并不支持忽略大小写的比较函数和查找函数，如果你想练练手，你可以试试改写一个char_traits , 然后生成一个case_string类, 也可以在string 上做继承，然后派生一个新的类，例如：ext_string，提供一些常用的功能，例如：
1、定义分隔符。给定分隔符，把string分为几个字段。
2、提供替换功能。例如，用winter, 替换字符串中的wende
3、大小写处理。例如，忽略大小写比较，转换等
4、整形转换。例如把"123"字符串转换为123数字。
这些都是常用的功能，如果你有兴趣可以试试。其实有人已经实现了，看看Extended STL string。如果你想偷懒，下载一个头文件就可以用，有了它确实方便了很多。要是有人能提供一个支持正则表达式的string，我会非常乐意用。
4、 string 建议
使用string 的方便性就不用再说了，这里要重点强调的是string的安全性。
string并不是万能的，如果你在一个大工程中需要频繁处理字符串，而且有可能是多线程，那么你一定要慎重(当然，在多线程下你使用任何STL容器都要慎重)。
string的实现和效率并不一定是你想象的那样，如果你对大量的字符串操作，而且特别关心其效率，那么你有两个选择，首先，你可以看看你使用的STL版本中string实现的源码；另一选择是你自己写一个只提供你需要的功能的类。
string的c_str()函数是用来得到C语言风格的字符串，其返回的指针不能修改其空间。而且在下一次使用时重新调用获得新的指针。
string的data()函数返回的字符串指针不会以'/0'结束，千万不可忽视。
尽量去使用操作符，这样可以让程序更加易懂（特别是那些脚本程序员也可以看懂）
5 附录
string 函数列表函数名 描述
begin 得到指向字符串开头的Iterator
end 得到指向字符串结尾的Iterator
rbegin 得到指向反向字符串开头的Iterator
rend 得到指向反向字符串结尾的Iterator
size 得到字符串的大小
length 和size函数功能相同
max_size 字符串可能的最大大小
capacity 在不重新分配内存的情况下，字符串可能的大小
empty 判断是否为空
operator[] 取第几个元素，相当于数组
c_str 取得C风格的const char* 字符串
data 取得字符串内容地址
operator= 赋值操作符
reserve 预留空间
swap 交换函数
insert 插入字符
append 追加字符
push_back 追加字符
operator+= += 操作符
erase 删除字符串
clear 清空字符容器中所有内容
resize 重新分配空间
assign 和赋值操作符一样
replace 替代
copy 字符串到空间
find 查找
rfind 反向查找
find_first_of 查找包含子串中的任何字符，返回第一个位置
find_first_not_of 查找不包含子串中的任何字符，返回第一个位置
find_last_of 查找包含子串中的任何字符，返回最后一个位置
find_last_not_of 查找不包含子串中的任何字符，返回最后一个位置
substr 得到字串
compare 比较字符串
operator+ 字符串链接
operator== 判断是否相等
operator!= 判断是否不等于
operator< 判断是否小于
operator>> 从输入流中读入字符串
operator<< 字符串写入输出流
getline 从输入流中读入一行
C++中String类的用法(综合)
之所以抛弃char*的字符串而选用C++标准程序库中的string类，是因为他和前者比较起来，不必担心内存是否足够、字符串长度等等，而且作为一个类出现，他集成的操作函数足以完成我们大多数情况下(甚至是100%)的需要。我们可以用= 进行赋值操作，== 进行比较，+ 做串联（是不是很简单?）。我们尽可以把它看成是C++的基本数据类型。
   好了，进入正题………
首先，为了在我们的程序中使用string类型，我们必须包含头文件。如下：
   #include //注意这里不是string.h string.h是C字符串头文件
1．声明一个C++字符串
声明一个字符串变量很简单：
   string Str;
这样我们就声明了一个字符串变量，但既然是一个类，就有构造函数和析构函数。上面的声明没有传入参数，所以就直接使用了string的默认的构造函数，这个函数所作的就是把Str初始化为一个空字符串。String类的构造函数和析构函数如下：
a)    string s;  //生成一个空字符串s
b)    string s(str) //拷贝构造函数 生成str的复制品
c)    string s(str,stridx) //将字符串str内“始于位置stridx”的部分当作字符串的初值
d)    string s(str,stridx,strlen) //将字符串str内“始于stridx且长度顶多strlen”的部分作为字符串的初值
e)    string s(cstr) //将C字符串作为s的初值
f)    string s(chars,chars_len) //将C字符串前chars_len个字符作为字符串s的初值。
g)    string s(num,c) //生成一个字符串，包含num个c字符
h)    string s(beg,end) //以区间beg;end(不包含end)内的字符作为字符串s的初值
i)    s.~string() //销毁所有字符，释放内存
都很简单，我就不解释了。
2．字符串操作函数
   这里是C++字符串的重点，我先把各种操作函数罗列出来，不喜欢把所有函数都看完的人可以在这里找自己喜欢的函数，再到后面看他的详细解释。
a) =,assign()   //赋以新值
b) swap()   //交换两个字符串的内容
c) +=,append(),push_back() //在尾部添加字符
d) insert() //插入字符
e) erase() //删除字符
f) clear() //删除全部字符
g) replace() //替换字符
h) + //串联字符串
i) ==,!=,<,<=,>,>=,compare()  //比较字符串
j) size(),length()  //返回字符数量
k) max_size() //返回字符的可能最大个数
l) empty()  //判断字符串是否为空
m) capacity() //返回重新分配之前的字符容量
n) reserve() //保留一定量内存以容纳一定数量的字符
o) [ ], at() //存取单一字符
p) >>,getline() //从stream读取某值
q) <<  //将谋值写入stream
r) copy() //将某值赋值为一个C_string
s) c_str() //将内容以C_string返回
t) data() //将内容以字符数组形式返回
u) substr() //返回某个子字符串
v)查找函数
w)begin() end() //提供类似STL的迭代器支持
x) rbegin() rend() //逆向迭代器
y) get_allocator() //返回配置器
下面详细介绍：
2．1 C++字符串和C字符串的转换
   C++ 提供的由C++字符串得到对应的C_string的方法是使用data()、c_str()和copy()，其中，data()以字符数组的形式返回字符串内容，但并不添加’/0’。c_str()返回一个以‘/0’结尾的字符数组，而copy()则把字符串的内容复制或写入既有的c_string或字符数组内。C++字符串并不以’/0’结尾。我的建议是在程序中能使用C++字符串就使用，除非万不得已不选用c_string。由于只是简单介绍，详细介绍掠过，谁想进一步了解使用中的注意事项可以给我留言(到我的收件箱)。我详细解释。
2．2 大小和容量函数
   一个C++字符串存在三种大小：a)现有的字符数，函数是size()和length()，他们等效。Empty()用来检查字符串是否为空。b)max_size() 这个大小是指当前C++字符串最多能包含的字符数，很可能和机器本身的限制或者字符串所在位置连续内存的大小有关系。我们一般情况下不用关心他，应该大小足够我们用的。但是不够用的话，会抛出length_error异常c)capacity()重新分配内存之前 string所能包含的最大字符数。这里另一个需要指出的是reserve()函数，这个函数为string重新分配内存。重新分配的大小由其参数决定，默认参数为0，这时候会对string进行非强制性缩减。
还有必要再重复一下C++字符串和C字符串转换的问题，许多人会遇到这样的问题，自己做的程序要调用别人的函数、类什么的（比如[数据库](http://lib.csdn.net/base/mysql)连接函数 Connect(char*,char*)），但别人的函数参数用的是char*形式的，而我们知道，c_str()、data()返回的字符数组由该字符串拥有，所以是一种const char*,要想作为上面提及的函数的参数，还必须拷贝到一个char*,而我们的原则是能不使用C字符串就不使用。那么，这时候我们的处理方式是：如果此函数对参数(也就是char*)的内容不修改的话，我们可以这样Connect((char*)UserID.c_str(),
 (char*)PassWD.c_str()),但是这时候是存在危险的，因为这样转换后的字符串其实是可以修改的（有兴趣地可以自己试一试），所以我强调除非函数调用的时候不对参数进行修改，否则必须拷贝到一个char*上去。当然，更稳妥的办法是无论什么情况都拷贝到一个char*上去。同时我们也祈祷现在仍然使用C字符串进行编程的高手们（说他们是高手一点儿也不为过，也许在我们还穿开裆裤的时候他们就开始编程了，哈哈…）写的函数都比较规范，那样我们就不必进行强制转换了。
2．3元素存取
   我们可以使用下标操作符[]和函数at()对元素包含的字符进行访问。但是应该注意的是操作符[]并不检查索引是否有效（有效索引0~str.length()），如果索引失效，会引起未定义的行为。而at()会检查，如果使用at() 的时候索引无效，会抛出out_of_range异常。
   有一个例外不得不说，const string a;的操作符[]对索引值是a.length()仍然有效，其返回值是’/0’。其他的各种情况，a.length()索引都是无效的。举例如下：
const string Cstr(“const string”);
string Str(“string”);
Str[3];    //ok
Str.at(3);  //ok
Str[100]; //未定义的行为
Str.at(100);  //throw out_of_range
Str[Str.length()]  //未定义行为
Cstr[Cstr.length()] //返回 ‘/0’
Str.at(Str.length());//throw out_of_range
Cstr.at(Cstr.length()) ////throw out_of_range
我不赞成类似于下面的引用或指针赋值：
char& r=s[2];
char* p= &s[3];
因为一旦发生重新分配，r,p立即失效。避免的方法就是不使用。
2．4比较函数
   C++字符串支持常见的比较操作符（>,>=,<,& lt;=,==,!=），甚至支持string与C-string的比较(如str<”hello”)。在使用>,>=,<,<=这些操作符的时候是根据“当前字符特性”将字符按字典顺序进行逐一得比较。字典排序靠前的字符小，比较的顺序是从前向后比较，遇到不相等的字符就按这个位置上的两个字符的比较结果确定两个字符串的大小。同时，string(“aaaa”)    另一个功能强大的比较函数是成员函数compare()。他支持多参数处理，支持用索引值和长度定位子串来进行比较。他返回一个整数来表示比较结果，返回值意义如下：0-相等〉0-大于
 <0-小于。举例如下：
   string s(“abcd”);
   s.compare(“abcd”); //返回0
   s.compare(“dcba”); //返回一个小于0的值
   s.compare(“ab”); //返回大于0的值
s.compare(s); //相等
   s.compare(0,2,s,2,2); //用”ab”和”cd”进行比较 小于零
   s.compare(1,2,”bcx”,2); //用”bc”和”bc”比较。
怎么样？功能够全的吧！什么？还不能满足你的胃口？好吧，那等着，后面有更个性化的比较[算法](http://lib.csdn.net/base/datastructure)。先给个提示，使用的是STL的比较算法。什么？对STL一窍不通？靠，你重修吧！
2．5 更改内容
这在字符串的操作中占了很大一部分。
首先讲赋值，第一个赋值方法当然是使用操作符=，新值可以是string(如：s=ns) 、c_string(如：s=”gaint”)甚至单一字符（如：s=’j’）。还可以使用成员函数assign()，这个成员函数可以使你更灵活的对字符串赋值。还是举例说明吧：
s.assign(str); //不说
s.assign(str,1,3);//如果str是”iamangel” 就是把”ama”赋给字符串
s.assign(str,2,string::npos);//把字符串str从索引值2开始到结尾赋给s
s.assign(“gaint”); //不说
s.assign(“nico”,5);//把’n’ ‘I’ ‘c’ ‘o’ ‘/0’赋给字符串
s.assign(5,’x’);//把五个x赋给字符串
把字符串清空的方法有三个：s=””;s.clear();s.erase();(我越来越觉得举例比说话让别人容易懂！)。
string提供了很多函数用于插入（insert）、删除（erase）、替换（replace）、增加字符。
先说增加字符（这里说的增加是在尾巴上），函数有 +=、append()、push_back()。举例如下：
s+=str;//加个字符串
s+=”my name is jiayp”;//加个C字符串
s+=’a’;//加个字符
s.append(str);
s.append(str,1,3);//不解释了同前面的函数参数assign的解释
s.append(str,2,string::npos)//不解释了
s.append(“my name is jiayp”);
s.append(“nico”,5);
s.append(5,’x’);
s.push_back(‘a’);//这个函数只能增加单个字符对STL熟悉的理解起来很简单
也许你需要在string中间的某个位置插入字符串，这时候你可以用insert()函数，这个函数需要你指定一个安插位置的索引，被插入的字符串将放在这个索引的后面。
s.insert(0,"my name");
s.insert(1,str);
这种形式的insert()函数不支持传入单个字符，这时的单个字符必须写成字符串形式(让人恶心)。既然你觉得恶心，那就不得不继续读下面一段话：为了插入单个字符，insert()函数提供了两个对插入单个字符操作的重载函数：insert(size_type index,size_type num,chart c)和insert(iteratorpos,size_type num,chart c)。其中size_type是无符号整数，iterator是char*,所以，你这么调用insert函数是不行的：insert(0,1,
 ’j’);这时候第一个参数将转换成哪一个呢？所以你必须这么写：insert((string::size_type)0,1,’j’)！第二种形式指出了使用迭代器安插字符的形式，在后面会提及。顺便提一下，string有很多操作是使用STL的迭代器的，他也尽量做得和STL靠近。
删除函数erase()的形式也有好几种（真烦！），替换函数replace()也有好几个。举例吧：
string s="il8n";
s.replace(1,2,"nternationalizatio");//从索引1开始的2个替换成后面的C_string
s.erase(13);//从索引13开始往后全删除
s.erase(7,5);//从索引7开始往后删5个
2．6提取子串和字符串连接
题取子串的函数是：substr(),形式如下：
s.substr();//返回s的全部内容
s.substr(11);//从索引11往后的子串
s.substr(5,6);//从索引5开始6个字符
把两个字符串结合起来的函数是+。（谁不明白请致电120）
2．7输入输出操作
1．>> 从输入流读取一个string。
2．<< 把一个string写入输出流。
另一个函数就是getline(),他从输入流读取一行内容，直到遇到分行符或到了文件尾。
2．8搜索与查找
查找函数很多，功能也很强大，包括了：
find()
rfind()
find_first_of()
find_last_of()
find_first_not_of()
find_last_not_of()
这些函数返回符合搜索条件的字符区间内的第一个字符的索引，没找到目标就返回npos。所有的函数的参数说明如下：
第一个参数是被搜寻的对象。第二个参数（可有可无）指出string内的搜寻起点索引，第三个参数（可有可无）指出搜寻的字符个数。比较简单，不多说不理解的可以向我提出，我再仔细的解答。当然，更加强大的STL搜寻在后面会有提及。
最后再说说npos的含义，string::npos的类型是string::size_type,所以，一旦需要把一个索引与npos相比，这个索引值必须是string::size)type类型的，更多的情况下，我们可以直接把函数和npos进行比较（如：if(s.find("jia")== string::npos)）。
C++中 String的用法 (一)
basic_string::append
向string 的后面加字符或字符串。(比+=, push_back 更灵活)
(1) 向string 的后面加C-string
basic _ string& append( const value _ type* _Ptr );
string s ( "Hello " ); // s= ” Hello ”
const char *c = "Out There ";
s.append ( c ); // s= ” Hello Out There ”
(2) 向string 的后面加C-string 的一部分
basic _ string& append( const value _ type* _Ptr , size _ type _Count );
string s ( "Hello " ); // s= ” Hello ”
const char *c = "Out There ";
s.append ( c , 3 ); // s= ” Hello Out ”
(3) 向string 的后面加string(有两种方法)
basic _ string& append( const basic _ string& _Str );
string s1 ( "Hello " ), s2 ( "Wide " ), s3( "World" );
s1.append ( s2 ); // s1= ” Hello Wide ”
s1 += s3; // s1= ” Hello Wide World ”
(4) 向string 的后面加string 的一部分 ---A
basic _ string& append( const basic _ string& _Str , size _ type _Off ,
size _ type _Count );
string s1 ( "Hello " ), s2 ( "Wide World " );
s1.append ( s2 , 5 , 5 ); // s1= ” Hello World ”
(5) 向string 的后面加string 的一部分 ---B
template<class InputIterator> basic _ string& append(
InputIterator _First , InputIterator _Last );
string str1f ( "Hello " ), str2f ( "Wide World" );
str1f.append ( str2f.begin ( ) + 5 , str2f.end ( ) );
// s1= ” Hello World ”
(6) 向string 的后面加多个字符
basic _ string& append( size _ type _Count , value _ type _Ch );
string str1e ( "Hello " );
str1e.append ( 4 , '!' ); // s1= ” Hello !!!! ”
basic_string::assign
给string 赋值。（比“=”更灵活）
(1) 向string 赋C-string
basic _ string& assign( const value _ type* _Ptr );
string s;
const char *c = "Out There";
s.assign ( c ); // s= ” Out There ”
(2) 向string 赋C-string 的一部分
basic _ string& assign( const value _ type* _Ptr , size _ type _Count );
string s;
const char *c = "Out There";
s.assign ( c , 3 ); // s= ” Out ”
(3) 向string 赋string(有两种方法)
basic _ string& assign( const basic _ string& _Str );
string s1 ( "Hello" ), s2 ( "Wide" ), s3( "World");
s1.assign ( s2 ); // s1= ” Wide ”
s1 = s3; // s1= ” World ”
(4) 向string 赋string 的一部分 ---A
basic _ string& assign( const basic _ string& _Str , size _ type off ,
size _ type _Count );
string s1 ( "Hello " ), s2 ( "Wide World " );
s1.assign ( s2 , 5 , 5 ); // s1= ” Hello World ”
(5) 向string 赋string 的一部分 ---B
template<class InIt> basic _ string& assign(
InputIterator _First ,
InputIterator _Last );
string str1f ( "Hello " ), str2f ( "Wide World" );
str1f.assign ( str2f.begin ( ) + 5 , str2f.end ( ) ); // s1= ” Wide World ”
(6) 向string 赋多个字符
basic _ string& assign( size _ type _Count , value _ type _Ch );
string str1e ( "Hello " );
str1e.assign ( 4 , '!' ); // s1= ” !!!! ”
C++中String类的用法(二)
basic_string::compare
如果所比较的两个string 相等，则返回0；操作string 大于参数string,返回
正数；操作string 小于参数string,返回负数。
(1) 比较操作string 与 _Str 或C-string _Ptr
int compare( const basic _ string& _Str ) const;
int compare( const value _ type* _Ptr ) const;
int com = s.compare ( sp );
(2) 比较操作string 中 _Pos1 ( 下标)开始的 _Num1 个字符与 string _Str
比较操作string 中 _Pos1 ( 下标)开始的 _Num1 个字符与 C-string _Ptr
比较操作string 中 Pos1 ( 下标)开始的 Num1 个字符与 Str 中 Off ( 下标)开始 Count 个字
符
int compare( size _ type _Pos1 , size _ type _Num1 , const basic _ string&_Str );
int compare( size _ type _Pos1 , size _ type _Num1 , const value _ type* _Ptr )const;
int compare( size _ type _Pos1 , size _ type _Num1 , const basic _ string&_Str ,
size _ type _Off , size _ type _Count );
int com1 = s.compare ( 2 , 3 , sp );
int com2 = s.compare ( 2 , 3 , c );
int com3 = s.compare ( 1 , 3 , cs , 3 ,1 );
basic_string::erase
删除string 中的一个或几个元素。前两个成员函数，返回要被删除的子串的下
一个元素的iterator; 第三个函数，返回删除后的string 的引用。
(1) 删除string 中从 _ First 到 _ Last 的字符
iterator erase( iterator _First , iterator _Last );
basic_string <char>::iterator s_Iter;
s_Iter = s.erase ( s.begin ( ) + 3 , s.end ( ) - 1 ); // s_Iter=s.end( )
(2) 删除string 中 _It 所指的字符
iterator erase( iterator _It );
s_Iter = s.erase ( s.begin ( ) + 5 );
(3) 删除string 中从 _Pos ( 下标)开始的 _Count 个字符
basic _ string& erase( size _ type _Pos = 0, size _ type _Count = npos );
str = s.erase ( 6 , 8 ); // str 也是 string
basic_string::find
寻找给定的string。返回找到的第一个string 下标值；如果没找到则返回npos。
(1) 找一个character _Ch 。（默认从头找）
size _ type find( value _ type _Ch , size _ type _Off = 0 ) const;
string s ( "Hello Everyone" );
basic_string <char>::size_type index1, index2;
static const basic_string <char>::size_type npos = -1;
index1 = s.find ( "e" , 3 ); // index1=8, 不是 6
index2 = s.find ( "x" ); // index2=-1
if ( indexCh1a != npos ) cout <<indexCh1a << endl;
else cout << "The character 'e' was not found in str1 ."<< endl;
(2) 找一个C-string。（默认从头找）
size _ type find( const value _ type* _Ptr , size _ type _Off = 0 ) const;
string s ( "Let me make this perfectly clear." );
basic_string <char>::size_type index;
const char *c = "perfect";
index = s.find ( c , 5 ); // index=17
(3) 找一个string。（默认从头找）
size _ type find( const basic _ string& _Str , size _ type _Off = 0 )const;
string s ( "clearly this perfectly unclear." );
basic_string <char>::size_type index;
string sta ( "clear" );
index = s.find ( sta , 5 ); // index=24
c++中String类的用法(三)
basic_string::max_size
返回string 能放的最大元素个数。（不同于capacity）
size _ type max _ size( ) const;
basic_string <char>::size_type cap, max;
cap = s.capacity ( );
max = s.max_size ( ); // max=4294967294.
basic_string::rfind
寻找给定的string。返回找到的第一个string 下标值；如果没找到则返回npos。
与find 不同的是：rfind 默认从npos 开始找。其他相同。
basic_string::replace
将原string 中的元素或子串替换。返回替换后的string。
（1）用string 或C-string 代替操作string 中从 _Pos1 开始的 _Num1 个字符
basic _ string& replace( size _ type _Pos1 ,size _ type _Num1 , const value_ type* _Ptr );
basic _ string& replace(size _ type _Pos1 ,size _ type _Num1 ,const basic _string _Str );
string a,b;
string s ( "AAAAAAAA" );
string s1p ( "BBB" );
const char* cs1p = "CCC" ；
a = s.replace ( 1 , 3 , s1p ); // s= ” ABBBAAAA ”
b = s.replace ( 5 , 3 , cs1p ); // s= ” ABBBACCC ”
（2）用string 中从 _Pos2 开始的 _Num2 个字符,代替操作string 中从 _Pos1 开始的 _Num1 个字符
用C-string 中的 _Num2 个字符，代替操作string 中从 _Pos1 开始的 _Num1 个字符
basic _ string& replace( size _ type _Pos1 , size _ type _Num1 , constbasic _ string& _Str ,
size _ type _Pos2 , size _ type );
basic _ string& replace( size _ type _Pos1 , size _ type _Num1 ,
const value _ type* _Ptr , size _ type _Num2 );
string a, b;
string s ( "AAAAAAAA" );
string s2p ( "BBB" );
const char* cs2p = "CCC";
a = s.replace ( 1 , 3 , s2p , 1 , 2 ); // s= ” ABBAAAA ”
b = s.replace ( 4 , 3 , cs2p , 1 ); // s= ” ABBAC ”
（3）用 _Count 个character _Ch , 代替操作string 中从 _Pos1 开始的 _Num1 个字符
basic _ string& replace( size _ type _Pos1 , size _ type _Num1 ,
size _ type _Count , value _ type _Ch );
string result;
string s ( "AAAAAAAA" );
char ch = 'C';
result = s.replace ( 1 , 3 , 4 , ch ); // s= ” ACCCCAAAA ”
（4）用string 或C-string ,代替操作string 中从 First0 到 Last0 的字符
basic _ string&replace(iterator First0 ,iterator Last0 , const basic _string& _Str );
basic _ string&replace(iterator First0 ,iterator _Last0 , const value _type* _Ptr );
string s ( "AAAAAAAA" ); string s4p ( "BBB" );
const char* cs4p = "CCC";
basic_string<char>::iterator IterF0, IterL0;
IterF0 = s.begin ( ); IterL0 = s.begin ( ) + 3;
string a, b;
a = s.replace ( IterF0 , IterL0 , s4p ); // s= ” BBBAAAAA ”
b = s.replace ( IterF0 , IterL0 , cs4p ); // s= ” CCCAAAAA ”
（5）用string 中从 _Pos2 开始的 _Num2 个字符,代替操作string 中从 First0 到 Last0 的字符
用C-string 中的 _Num2 个字符，代替操作string 中从 First0 到 Last0 的字符
basic _ string& replace( iterator _First0 , iterator _Last0 ,
const value _ type* _Ptr , size _ type _Num2 );
template<class InputIterator> basic _ string& replace(
iterator _First0 , iterator _Last0 ,
InputIterator _First , InputIterator _Last );
IterF3 = s.begin ( ) + 1; IterL3 = s.begin ( ) + 3;
IterF4 = s.begin ( ); IterL4 = s.begin ( ) + 2;
a = s.replace ( IterF3 , IterL3 , IterF4 , IterL4 );
b = s.replace ( IterF1 , IterL1 , cs5p , 4 );
（6）用 _Count 个character _Ch , 代替操作string 中从 First0 到 Last0 的字符
basic _ string& replace( iterator _First0 , iterator _Last0 ,
size _ type _Count , value _ type _Ch );
a = s.replace ( IterF2 , IterL2 , 4 , ch );
basic_string::swap(交换两个string)
void swap( basic _ string& _Str );
s1.swap ( s2 );
basic_string::substr
返回从 _Off ( 下标)开始的 _Count 个字符组成的string
basic _ string substr( size _ type _Off = 0, size _ type _Count = npos ) const;
string s("I love you!") ， sub;
sub=s.substr( ); // sub= ” I love you! ”
sub=s.substr(1); // sub= ” love you! ”
sub=s.substr(3,4); // sub= ” ove ”
 string函数用法
@函数名称:      strdup
函数原型:      char *strdup(const char *s)
函数功能:      字符串拷贝，目的空间由该函数分配
函数返回:      指向拷贝后的字符串指针
参数说明:      src-待拷贝的源字符串
所属文件:     
int main()
{
     char *dup_str, *string="abcde";
     dup_str=strdup(string);
     printf("%s", dup_str);
     free(dup_str);
     return 0;
}
@函数名称:      strcpy
函数原型:      char* strcpy(char* str1,char* str2);
函数功能:      把str2指向的字符串拷贝到str1中去
函数返回:      返回str1,即指向str1的指针
参数说明:
所属文件:     
int main()
{
     char string[10];
     char *str1="abcdefghi";
     strcpy(string,str1);
     printf("the string is:%s/n",string);
     return 0;
}
@函数名称:      strncpy
函数原型:      char *strncpy(char *dest, const char*src，int count)
函数功能:      将字符串src中的count个字符拷贝到字符串dest中去
函数返回:      指向dest的指针
参数说明:      dest-目的字符串，src-源字符串，count-拷贝的字符个数
所属文件:     
int main()
{
     char string[10];
     char *str1="abcdefghi";
     strncpy(string,str1,3);
     string[3]='/0';
     printf("%s",string);
     return 0;
}
@函数名称:      strcat
函数原型:      char* strcat(char * str1,char *str2);
函数功能:      把字符串str2接到str1后面,str1最后的'/0'被取消
函数返回:      str1
参数说明:
所属文件:     
int main()
{
     char buffer[80];
     strcpy(buffer,"Hello ");
     strcat(buffer,"world");
     printf("%s/n",buffer);
     return 0;
}
@函数名称:      strncat
函数原型:      char *strncat(char *dest, const char*src, size_t maxlen)
函数功能:      将字符串src中前maxlen个字符连接到dest中
函数返回:
参数说明:
所属文件:     
char buffer[80];
int main()
{
     strcpy(buffer,"Hello ");
     strncat(buffer,"world",8);
     printf("%s/n",buffer);
     strncat(buffer,"*************",4);
     printf("%s/n",buffer);
     return 0;
}
@函数名称:      strcmp
函数原型:      int strcmp(char * str1,char * str2);
函数功能:      比较两个字符串str1,str2.
函数返回:      str1str2,返回正数.
参数说明:
所属文件:     
int main()
{
     char *buf1="aaa", *buf2="bbb",*buf3="ccc";
     int ptr;
     ptr=strcmp(buf2, buf1);
     if(ptr>0)
         printf("buffer 2 isgreater than buffer 1/n");
     else
         printf("buffer 2 is lessthan buffer 1/n");
     ptr=strcmp(buf2, buf3);
     if(ptr>0)
         printf("buffer 2 isgreater than buffer 3/n");
     else
         printf("buffer 2 is lessthan buffer 3/n");
     return 0;
}
@函数名称:      strncmp
函数原型:      int strncmp(char *str1,char*str2,int count)
函数功能:      对str1和str2中的前count个字符按字典顺序比较
函数返回:      小于0：str1str2
参数说明:      str1,str2-待比较的字符串，count-比较的长度
所属文件:     
#include
#include
int   main()
{
     int ptr;
     char*buf1="aaabbb",*buf2="bbbccc",*buf3="ccc";
     ptr=strncmp(buf2,buf1,3);
     if (ptr>0)
         printf("buffer 2 isgreater than buffer 1");
     else
         printf("buffer 2 is lessthan buffer 1");
         ptr=strncmp(buf2,buf3,3);
     if (ptr>0)
         printf("buffer 2 isgreater than buffer 3");
     else
         printf("buffer 2 is lessthan buffer 3");
     return(0);
}
@函数名称:      strpbrk
函数原型:      char *strpbrk(const char *s1, constchar *s2)
函数功能:      得到s1中第一个“同时也出现在s2中”字符的位置指针
函数返回:      位置指针
参数说明:
所属文件:     
int main()
{
   char *p="Find all vowels";
   while(p)
   {
     printf("%s/n",p);
     p=strpbrk(p+1,"aeiouAEIOU");
   }
   return 0;
}
@函数名称:      strcspn
函数原型:      int strcspn(const char *s1, constchar *s2)
函数功能:      统计s1中从头开始直到第一个“来自s2中的字符”出现的长度
函数返回:      长度
参数说明:
所属文件:     
int main()
{
    printf("%d/n",strcspn("abcbcadef","cba"));
    printf("%d/n",strcspn("xxxbcadef","cba"));
    printf("%d/n",strcspn("123456789","cba"));
     return 0;
}
@函数名称:      strspn
函数原型:      int strspn(const char *s1, constchar *s2)
函数功能:      统计s1中从头开始直到第一个“不来自s2中的字符”出现的长度
函数返回:      位置指针
参数说明:
所属文件:     
int main()
{
     printf("%d/n",strspn("out tolunch","aeiou"));
     printf("%d/n",strspn("out tolunch","xyz"));
     return 0;
}
@函数名称:      strchr
函数原型:      char* strchr(char* str,char ch);
函数功能:      找出str指向的字符串中第一次出现字符ch的位置
函数返回:      返回指向该位置的指针,如找不到,则返回空指针
参数说明:      str-待搜索的字符串，ch-查找的字符
所属文件:     
int main()
{
     char string[15];
     char *ptr, c='r';
     strcpy(string, "This is a string");
     ptr=strchr(string, c);
     if (ptr)
         printf("The character %cis at position: %d/n",c,ptr-string);
     else
         printf("The character wasnot found/n");
     return 0;
}
@函数名称:      strrchr
函数原型:      char *strrchr(const char *s, int c)
函数功能:      得到字符串s中最后一个含有c字符的位置指针
函数返回:      位置指针
参数说明:
所属文件:     
int main()
{
     char string[15];
     char *ptr,c='r';
     strcpy(string,"This is a string");
     ptr=strrchr(string,c);
     if (ptr)
         printf("The character %cis at position:%d",c,ptr-string);
     else
         printf("The character wasnot found");
     return 0; 
C++ String 类常用函数收藏
string类的构造函数：
string(const char *s);    //用c字符串s初始化
string(int n,char c);     //用n个字符c初始化
此外，string类还支持默认构造函数和复制构造函数，如string s1；string s2="hello"；都是正确的写法。当构造的string太长而无法表达时会抛出length_error异常
string类的字符操作：
const char &operator[](int n)const;
const char &at(int n)const;
char &operator[](int n);
char &at(int n);
operator[]和at()均返回当前字符串中第n个字符的位置，但at函数提供范围检查，当越界时会抛出out_of_range异常，下标运算符[]不提供检查访问。
const char *data()const;//返回一个非null终止的c字符数组
const char *c_str()const;//返回一个以null终止的c字符串
int copy(char *s, int n, int pos = 0) const;//把当前串中以pos开始的n个字符拷贝到以s为起始位置的字符数组中，返回实际拷贝的数目
string的特性描述:
int capacity()const;    //返回当前容量（即string中不必增加内存即可存放的元素个数）
int max_size()const;    //返回string对象中可存放的最大字符串的长度
int size()const;        //返回当前字符串的大小
int length()const;       //返回当前字符串的长度
bool empty()const;        //当前字符串是否为空
void resize(int len,char c);//把字符串当前大小置为len，并用字符c填充不足的部分string类的输入输出操作:
string类重载运算符operator>>  //用于输入，同样重载运算符operator<<用于输出操作。
函数getline(istream &in,string &s);//用于从输入流in中读取字符串到s中，以换行符'/n'分开。
string的赋值：
string &operator=(const string &s);//把字符串s赋给当前字符串
string &assign(const char *s);//用c类型字符串s赋值
string &assign(const char *s,int n);//用c字符串s开始的n个字符赋值
string &assign(const string &s);//把字符串s赋给当前字符串
string &assign(int n,char c);//用n个字符c赋值给当前字符串
string &assign(const string &s,int start,int n);//把字符串s中从start开始的n个字符赋给当前字符串
string &assign(const_iterator first,const_itertor last);//把first和last迭代器之间的部分赋给字符串
string的连接：
string &operator+=(const string &s);//把字符串s连接到当前字符串的结尾
string &append(const char *s);   //把c类型字符串s连接到当前字符串结尾
string &append(const char *s,int n);//把c类型字符串s的前n个字符连接到当前字符串结尾
string &append(const string &s);    //同operator+=()
string &append(const string &s,int pos,int n); //把字符串s中从pos开始的n个字符连接到当前字符串的结尾
string &append(int n,char c);        //在当前字符串结尾添加n个字符c
string &append(const_iterator first,const_iterator last);//把迭代器first和last之间的部分连接到当前字符串的结尾
string的比较：
bool perator==(const string &s1,const string &s2)const;//比较两个字符串是否相等
运算符">","<",">=","<=","!="均被重载用于字符串的比较；
int compare(const string &s) const;//比较当前字符串和s的大小
int compare(int pos, int n,const string &s)const;//比较当前字符串从pos开始的n个字符组成的字符串与s的大小
int compare(int pos, int n,const string &s,int pos2,int n2)const;//比较当前字符串从pos开始的n个字符组成的字符串与s中pos2开始的n2个字符组成的字符串的大小
int compare(const char *s) const;
int compare(int pos, int n,const char *s) const;
int compare(int pos, int n,const char *s, int pos2) const;
compare函数在>时返回1，<时返回-1，==时返回0 
string的子串：
string substr(int pos = 0,int n = npos) const;//返回pos开始的n个字符组成的字符串string的交换：
void swap(string &s2);    //交换当前字符串与s2的值
string类的查找函数：
int find(char c, int pos = 0) const;//从pos开始查找字符c在当前字符串的位置
int find(const char *s, int pos = 0) const;//从pos开始查找字符串s在当前串中的位置
int find(const char *s, int pos, int n) const;//从pos开始查找字符串s中前n个字符在当前串中的位置
int find(const string &s, int pos = 0) const;//从pos开始查找字符串s在当前串中的位置
//查找成功时返回所在位置，失败返回string::npos的值
int rfind(char c, int pos = npos) const;//从pos开始从后向前查找字符c在当前串中的位置
int rfind(const char *s, int pos = npos) const;
int rfind(const char *s, int pos, int n = npos) const;
int rfind(const string &s,int pos = npos) const;
//从pos开始从后向前查找字符串s中前n个字符组成的字符串在当前串中的位置，成功返回所在位置，失败时返回string::npos的值
int find_first_of(char c, int pos = 0) const;//从pos开始查找字符c第一次出现的位置
int find_first_of(const char *s, int pos = 0) const;
int find_first_of(const char *s, int pos, int n) const;
int find_first_of(const string &s,int pos = 0) const;
//从pos开始查找当前串中第一个在s的前n个字符组成的数组里的字符的位置。查找失败返回
string::npos
int find_first_not_of(char c, int pos = 0) const;
int find_first_not_of(const char *s, int pos = 0) const;
int find_first_not_of(const char *s, int pos,int n) const;
int find_first_not_of(const string &s,int pos = 0) const;
//从当前串中查找第一个不在串s中的字符出现的位置，失败返回string::npos
int find_last_of(char c, int pos = npos) const;
int find_last_of(const char *s, int pos = npos) const;
int find_last_of(const char *s, int pos, int n = npos) const;
int find_last_of(const string &s,intpos = npos) const;
int find_last_not_of(char c, int pos = npos) const;
int find_last_not_of(const char *s, int pos = npos) const;
int find_last_not_of(const char *s, int pos,  int n) const;
int find_last_not_of(const string &s,int pos = npos) const;
//find_last_of和find_last_not_of与find_first_of和find_first_not_of相似，只不过是从后向前查找
string类的替换函数：
string &replace(int p0, int n0,const char *s);//删除从p0开始的n0个字符，然后在p0处插入串s
string &replace(int p0, int n0,const char *s, int n);//删除p0开始的n0个字符，然后在p0处插入字符串s的前n个字符
string &replace(int p0, int n0,const string &s);//删除从p0开始的n0个字符，然后在p0处插入串s
string &replace(int p0, int n0,const string &s, int pos, int n);//删除p0开始的n0个字符，然后在p0处插入串s中从pos开始的n个字符
string &replace(int p0, int n0,int n, char c);//删除p0开始的n0个字符，然后在p0处插入n个字符c
string &replace(iterator first0, iterator last0,const char *s);//把[first0，last0）之间的部分替换为字符串s
string &replace(iterator first0, iterator last0,const char *s, int n);//把[first0，last0）之间的部分替换为s的前n个字符
string &replace(iterator first0, iterator last0,const string &s);//把[first0，last0）之间的部分替换为串s
string &replace(iterator first0, iterator last0,int n, char c);//把[first0，last0）之间的部分替换为n个字符c
string &replace(iterator first0, iterator last0,const_iterator first,const_iteratorlast);//把[first0，last0）之间的部分替换成[first，last）之间的字符串
string类的插入函数：
string &insert(int p0, const char *s);
string &insert(int p0, const char *s, int n);
string &insert(int p0,const string &s);
string &insert(int p0,const string &s, int pos, int n);
//前4个函数在p0位置插入字符串s中pos开始的前n个字符
string &insert(int p0, int n, char c);//此函数在p0处插入n个字符c
iterator insert(iterator it, char c);//在it处插入字符c，返回插入后迭代器的位置
void insert(iterator it, const_iterator first, const_iterator last);//在it处插入[first，last）之间的字符
void insert(iterator it, int n, char c);//在it处插入n个字符c
string类的删除函数
iterator erase(iterator first, iterator last);//删除[first，last）之间的所有字符，返回删除后迭代器的位置
iterator erase(iterator it);//删除it指向的字符，返回删除后迭代器的位置
string &erase(int pos = 0, int n = npos);//删除pos开始的n个字符，返回修改后的字符串
string类的迭代器处理：
string类提供了向前和向后遍历的迭代器iterator，迭代器提供了访问各个字符的语法，类似于指针操作，迭代器不检查范围。
用string::iterator或string::const_iterator声明迭代器变量，const_iterator不允许改变迭代的内容。常用迭代器函数有：
const_iterator begin()const;
iteratorbegin();               //返回string的起始位置
const_iterator end()const;
iteratorend();                   //返回string的最后一个字符后面的位置
const_iterator rbegin()const;
iteratorrbegin();               //返回string的最后一个字符的位置
const_iterator rend()const;
iteratorrend();                   //返回string第一个字符位置的前面rbegin和rend用于从后向前的迭代访问，通过设置迭代器
string::reverse_iterator,string::const_reverse_iterator实现
字符串流处理：
通过定义ostringstream和istringstream变量实现，<sstream>头文件中
例如：
    string input("hello,this is a test");
    istringstream is(input);
    string s1,s2,s3,s4;
   is>>s1>>s2>>s3>>s4;//s1="hello,this",s2="is",s3="a",s4="test"
    ostringstream os;
    os<<s1<<s2<<s3<<s4;
    cout<<os.str();
3. vector 用法详解
这篇文章的目的是为了介绍std::vector，如何恰当地使用它们的成员函数等操作。本文中还讨论了条件函数和函数指针在迭代算法中使用，如在remove_if()和for_each()中的使用。通过阅读这篇文章读者应该能够有效地使用vector容器，而且应该不会再去使用C类型的动态数组了。
Vector总览
vector是C++标准模板库中的部分内容，它是一个多功能的，能够操作多种[数据结构](http://lib.csdn.net/base/datastructure)和算法的模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单地说，vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。
为了可以使用vector，必须在你的头文件中包含下面的代码：
#include <vector>
vector属于std命名域的，因此需要通过命名限定，如下完成你的代码：
using std::vector;
vector<int> vInts;
或者连在一起，使用全名：
std::vector<int> vInts;
建议使用全局的命名域方式：
using namespace std;
在后面的操作中全局的命名域方式会造成一些问题。vector容器提供了很多接口，在下面的表中列出vector的成员函数和操作。
Vector成员函数
函数
 表述
c.assign(beg,end)
c.assign(n,elem)
 将[beg; end)区间中的数据赋值给c。
将n个elem的拷贝赋值给c。
c.at(idx)
 传回索引idx所指的数据，如果idx越界，抛出out_of_range。
c.back()
 传回最后一个数据，不检查这个数据是否存在。
c.begin()
 传回迭代器重的可一个数据。
c.capacity()
 返回容器中数据个数。
c.clear()
 移除容器中所有数据。
c.empty()
 判断容器是否为空。
c.end()
 指向迭代器中的最后一个数据地址。
c.erase(pos)
c.erase(beg,end)
 删除pos位置的数据，传回下一个数据的位置。
删除[beg,end)区间的数据，传回下一个数据的位置。
c.front()
 传回地一个数据。
get_allocator
 使用构造函数返回一个拷贝。
c.insert(pos,elem)
c.insert(pos,n,elem)
c.insert(pos,beg,end)
 在pos位置插入一个elem拷贝，传回新数据位置。
在pos位置插入n个elem数据。无返回值。
在pos位置插入在[beg,end)区间的数据。无返回值。
c.max_size()
 返回容器中最[大数据](http://lib.csdn.net/base/hadoop)的数量。
c.pop_back()
 删除最后一个数据。
c.push_back(elem)
 在尾部加入一个数据。
c.rbegin()
 传回一个逆向队列的第一个数据。
c.rend()
 传回一个逆向队列的最后一个数据的下一个位置。
c.resize(num)
 重新指定队列的长度。
c.reserve()
 保留适当的容量。
c.size()
 返回容器中实际数据的个数。
c1.swap(c2)
swap(c1,c2)
 将c1和c2元素互换。
同上操作。
vector<Elem> c
vector <Elem> c1(c2)
vector <Elem> c(n)
vector <Elem> c(n, elem)
vector <Elem> c(beg,end)
c.~ vector <Elem>()
 创建一个空的vector。
复制一个vector。
创建一个vector，含有n个数据，数据均已缺省构造产生。
创建一个含有n个elem拷贝的vector。
创建一个以[beg;end)区间的vector。
销毁所有数据，释放内存。
Vector操作
函数
 描述
operator[] 
 返回容器中指定位置的一个引用。
创建一个vector
vector容器提供了多种创建方法，下面介绍几种常用的。
创建一个Widget类型的空的vector对象：
vector<Widget> vWidgets;
//     ------
//      |
//      |- Sincevector is a [Container](http://lib.csdn.net/base/docker), its member functions
//        operate on iterators and the container itself so
//        it can hold objects of any type.
创建一个包含500个Widget类型数据的vector：
vector<Widget> vWidgets(500);
创建一个包含500个Widget类型数据的vector，并且都初始化为0：
vector<Widget> vWidgets(500,Widget(0));
创建一个Widget的拷贝：
vector<Widget>vWidgetsFromAnother(vWidgets);
向vector添加一个数据
vector添加数据的缺省方法是push_back()。push_back()函数表示将数据添加到vector的尾部，并按需要来分配内存。例如：向vector<Widget>中添加10个数据，需要如下编写代码：
for(int i= 0;i<10; i++)
   vWidgets.push_back(Widget(i));
获取vector中制定位置的数据
很多时候我们不必要知道vector里面有多少数据，vector里面的数据是动态分配的，使用push_back()的一系列分配空间常常决定于文件或一些数据源。如果你想知道vector存放了多少数据，你可以使用empty()。获取vector的大小，可以使用size()。例如，如果你想获取一个vector v的大小，但不知道它是否为空，或者已经包含了数据，如果为空想设置为-1，你可以使用下面的代码实现：
int nSize = v.empty() ? -1 :static_cast<int>(v.size());
访问vector中的数据
使用两种方法来访问vector。
1、  vector::at()
2、  vector::operator[]
operator[]主要是为了与C语言进行兼容。它可以像C语言数组一样操作。但at()是我们的首选，因为at()进行了边界检查，如果访问超过了vector的范围，将抛出一个例外。由于operator[]容易造成一些错误，所有我们很少用它，下面进行验证一下：
分析下面的代码：
vector<int> v;
v.reserve(10);
for(int i=0; i<7; i++)
    v.push_back(i);
try
{
 int iVal1 = v[7];  // not boundschecked - will not throw
 int iVal2 = v.at(7); // boundschecked - will throw if out of range
}
catch(const exception& e)
{
 cout << e.what();
}
我们使用reserve()分配了10个int型的空间，但并不没有初始化。如下图所示：
你可以在这个代码中尝试不同条件，观察它的结果，但是无论何时使用at()，都是正确的。
删除vector中的数据
vector能够非常容易地添加数据，也能很方便地取出数据，同样vector提供了erase()，pop_back()，clear()来删除数据，当你删除数据的时候，你应该知道要删除尾部的数据，或者是删除所有数据，还是个别的数据。在考虑删除等操作之前让我们静下来考虑一下在STL中的一些应用。
Remove_if()算法
现在我们考虑操作里面的数据。如果要使用remove_if()，我们需要在头文件中包含如下代码：：
#include <algorithm>
        Remove_if()有三个参数：
1、  iterator _First：指向第一个数据的迭代指针。
2、  iterator _Last：指向最后一个数据的迭代指针。
3、  predicate _Pred：一个可以对迭代操作的条件函数。
条件函数
条件函数是一个按照用户定义的条件返回是或否的结果，是最基本的函数指针，或者是一个函数对象。这个函数对象需要支持所有的函数调用操作，重载operator()()操作。remove_if()是通过unary_function继承下来的，允许传递数据作为条件。
例如，假如你想从一个vector<CString>中删除匹配的数据，如果字串中包含了一个值，从这个值开始，从这个值结束。首先你应该建立一个数据结构来包含这些数据，类似代码如下：
#include <functional>
enum findmodes
{
 FM_INVALID = 0,
 FM_IS,
 FM_STARTSWITH,
 FM_ENDSWITH,
 FM_CONTAINS
};
typedef struct tagFindStr
{
 UINT iMode;
 CString szMatchStr;
} FindStr;
typedef FindStr* LPFINDSTR;
然后处理条件判断：
class FindMatchingString
    : publicstd::unary_function<CString, bool>
{
public:
  FindMatchingString(const LPFINDSTRlpFS) : m_lpFS(lpFS) {}
  bool operator()(CString&szStringToCompare) const
  {
     bool retVal =false;
    switch(m_lpFS->iMode)
     {
     case FM_IS:
       {
        retVal = (szStringToCompare == m_lpFDD->szMatchStr);
        break;
       }
     caseFM_STARTSWITH:
       {
        retVal = (szStringToCompare.Left(m_lpFDD->szMatchStr.GetLength())
              == m_lpFDD->szWindowTitle);
        break;
       }
     case FM_ENDSWITH:
       {
        retVal = (szStringToCompare.Right(m_lpFDD->szMatchStr.GetLength())
              == m_lpFDD->szMatchStr);
        break;
       }
     case FM_CONTAINS:
       {
        retVal = (szStringToCompare.Find(m_lpFDD->szMatchStr) != -1);
        break;
       }
     }
     return retVal;
 }
private:
    LPFINDSTR m_lpFS;
};
通过这个操作你可以从vector中有效地删除数据：
// remove all strings containing the valueof
// szRemove from vector<CString> vs.
FindStr fs;
fs.iMode = FM_CONTAINS;
fs.szMatchStr = szRemove;
vs.erase(std::remove_if(vs.begin(),vs.end(), FindMatchingString(&fs)), vs.end());
Remove_if()能做什么？
你可能会疑惑，对于上面那个例子在调用remove_if()的时候还要使用erase()呢？这是因为大家并不熟悉STL中的算法。Remove(),remove_if()等所有的移出操作都是建立在一个迭代范围上的，那么不能操作容器中的数据。所以在使用remove_if()，实际上操作的时容器里数据的上面的。思考上面的例子：
1、  szRemove = “o”.
2、   vs见下面图表中的显示。
观察这个结果，我们可以看到remove_if()实际上是根据条件对迭代地址进行了修改，在数据的后面存在一些残余的数据，那些需要删除的数据。剩下的数据的位置可能不是原来的数据，但他们是不知道的。
调用erase()来删除那些残余的数据。注意上面例子中通过erase()删除remove_if()的结果和vs.enc()范围的数据。
压缩一个臃肿的vector
很多时候大量的删除数据，或者通过使用reserve()，结果vector的空间远远大于实际需要的。所有需要压缩vector到它实际的大小。resize()能够增加vector的大小。Clear()仅仅能够改变缓存的大小，所有的这些对于vector释放内存等九非常重要了。如何来解决这些问题呢，让我们来操作一下。
我们可以通过一个vector创建另一个vector。让我们看看这将发生什么。假定我们已经有一个vector v，它的内存大小为1000，当我们调用size()的时候，它的大小仅为7。我们浪费了大量的内存。让我们在它的基础上创建一个vector。
std::vector<CString> vNew(v);
cout << vNew.capacity();
vNew.capacity()返回的是7。这说明新创建的只是根据实际大小来分配的空间。现在我们不想释放v，因为我们要在其它地方用到它，我们可以使用swap()将v和vNew互相交换一下？
    vNew.swap(v);
    cout <<vNew.capacity();
    cout <<v.capacity();
有趣的是：vNew.capacity()是1000，而v.capacity()是7。
现在是达到我的目的了，但是并不是很好的解决方法，我们可以像下面这么写：
   std::vector<CString>(v).swap(v); 
你可以看到我们做了什么？我们创建了一个临时变量代替那个命名的，然后使用swap(),这样我们就去掉了不必要的空间，得到实际大小的v。
