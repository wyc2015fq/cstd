
# java的StringBuffer可变字符串的追加及修改、查找 - 李昆鹏的博客 - CSDN博客


2018年03月16日 14:20:11[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：849


**-----------java的StringBuffer可变字符串的追加及修改--------------**
|构造方法摘要
|
|StringBuffer|()
|构造一个其中不带字符的字符串缓冲区，初始容量为  16 个字符。
|
|StringBuffer|(|CharSequence|seq)
|public  java.lang.StringBuilder(CharSequence seq) 构造一个字符串缓冲区，它包含与指定的|CharSequence|相同的字符。
|
|StringBuffer|(int capacity)
|构造一个不带字符，但具有指定初始容量的字符串缓冲区。
|
|StringBuffer|(|String|str)
|构造一个字符串缓冲区，并将其内容初始化为指定的字符串内容。
|
**例子：**
**public****class**Test2 {
**public****static****void**main(String[]args) {
//创建可变字符串，开辟了一个默认是长度为0，容量为16个字符的空间
StringBuffersb=**new**StringBuffer();
System.**out**.println("输出可变字符串的长度："+sb.length()+"输出可变字符串的容量："+sb.capacity());
//追加字符串
sb.append("hellohellohellohello");
System.**out**.println("输出可变字符串的长度："+sb.length()+"输出可变字符串的容量："+sb.capacity());
//创建一个10个字符容量的可变字符串
StringBuffersb1=**new**StringBuffer(10);
sb1.append("hellohellohellohello");
System.**out**.println("输出可变字符串的长度："+sb.length()+"输出可变字符串的容量："+sb.capacity());
//创建一个带有字符串的参数的可变字符串对象
StringBuffersb2=**new**StringBuffer("hellohellohellohello");
System.**out**.println("输出可变字符串的长度："+sb.length()+"输出可变字符串的容量："+sb.capacity());
}
}
结果：
输出可变字符串的长度：0输出可变字符串的容量：16
输出可变字符串的长度：20输出可变字符串的容量：34
输出可变字符串的长度：20输出可变字符串的容量：34
输出可变字符串的长度：20输出可变字符串的容量：34
---------------StringBuffer的追加方法-----------------------------------
|StringBuffer
|append|(boolean b)
|Appends the  string representation of the|boolean|argument to the sequence.
|
|StringBuffer
|append|(char c)
|Appends the  string representation of the|char|argument to this sequence.
|
|StringBuffer
|append|(char[] str)
|Appends the  string representation of the|char|array argument to this sequence.
|
|StringBuffer
|append|(char[] str, int offset,  int len)
|Appends the  string representation of a subarray of the|char|array argument  to this sequence.
|
|StringBuffer
|append|(|CharSequence|s)
|将指定的|CharSequence|添加到该序列。
|
|StringBuffer
|append|(|CharSequence|s,  int start, int end)
|Appends a  subsequence of the specified|CharSequence|to this sequence.
|
|StringBuffer
|append|(double d)
|Appends the  string representation of the|double|argument to this sequence.
|
|StringBuffer
|append|(float f)
|Appends the  string representation of the|float|argument to this sequence.
|
|StringBuffer
|append|(int i)
|Appends the  string representation of the|int|argument to this sequence.
|
|StringBuffer
|append|(long lng)
|Appends the  string representation of the|long|argument to this sequence.
|
|StringBuffer
|append|(|Object|obj)
|Appends the  string representation of the|Object|argument.
|
|StringBuffer
|append|(|String|str)
|Appends the  specified string to this character sequence.
|
|StringBuffer
|append|(|StringBuffer|sb)
|将指定的|StringBuffer|添加到此序列中。
|
例子：
/**
* StringBuffer中的追加方法测试
***@****作者**李昆鹏
*
*/
**public****class**Test2 {
**public****static****void**main(String[]args) {
//创建可变字符串，开辟了一个默认是长度为0，容量为16个字符的空间
StringBuffersb=**new**StringBuffer();
sb.append(**true**)
.append('a')
.append(**new****char**[] {'a','g','g'})
.append("hello")
.append(100d)
.append(14.5f)
.append(13)
.append(9001);
System.**out**.println(sb);
Objectp=**new**Object();
//追加了一个Object类的对象,返回的是对象的toString方法
sb.append(p);
System.**out**.println(sb);
StringBuffersb1=**new**StringBuffer("helloworld");
//可变字符串1追加可变字符串2
sb.append(sb1);
System.**out**.println(sb);
}
}
结果：
trueaagghello100.014.5139001
trueaagghello100.014.5139001java.lang.Object@70dea4e
trueaagghello100.014.5139001java.lang.Object@70dea4ehelloworld
----------------------可变字符串的修改----------------------
插入方法：
|StringBuffer
|insert|(int offset, boolean b)
|Inserts the  string representation of the|boolean|argument into this sequence.
|
|StringBuffer
|insert|(int offset, char c)
|Inserts the  string representation of the|char|argument into this sequence.
|
|StringBuffer
|insert|(int offset, char[] str)
|Inserts the  string representation of the|char|array argument into this  sequence.
|
|StringBuffer
|insert|(int index, char[] str,  int offset, int len)
|Inserts the  string representation of a subarray of the|str|array argument  into this sequence.
|
|StringBuffer
|insert|(int dstOffset,|CharSequence|s)
|Inserts the  specified|CharSequence|into this sequence.
|
|StringBuffer
|insert|(int dstOffset,|CharSequence|s,  int start, int end)
|Inserts a  subsequence of the specified|CharSequence|into this sequence.
|
|StringBuffer
|insert|(int offset, double d)
|Inserts the  string representation of the|double|argument into this sequence.
|
|StringBuffer
|insert|(int offset, float f)
|Inserts the  string representation of the|float|argument into this sequence.
|
|StringBuffer
|insert|(int offset, int i)
|Inserts the  string representation of the second|int|argument into  this sequence.
|
|StringBuffer
|insert|(int offset, long l)
|Inserts the  string representation of the|long|argument into this sequence.
|
|StringBuffer
|insert|(int offset,|Object|obj)
|Inserts the  string representation of the|Object|argument into this character  sequence.
|
|StringBuffer
|insert|(int offset,|String|str)
|Inserts the  string into this character sequence.
|
例子：
/**
* StringBuffer中的插入方法测试
***@****作者**李昆鹏
*
*/
**public****class**Test2 {
**public****static****void**main(String[]args) {
//创建可变字符串，开辟了一个默认是长度为0，容量为16个字符的空间
StringBuffersb=**new**StringBuffer("helloworld");
//在指定的索引位置插入字符串，注意不要出现字符串的索引越界问题
sb.insert(2,**true**)
.insert(10,"xxx")
.insert(3,**new****char**[] {'a','b','c'});
System.**out**.println(sb);
**char**[]cs= {'a','b','c','d'};
//第一个参数要插入的索引位置
//第二个参数要插入的字符数组
//第三个参数是数组的起始索引
//第四个参数是要插入的长度(在插入的数组中从第三个参数忘后算几个，包含第三个参数)
sb.insert(sb.length(),cs, 1, 2);
System.**out**.println(sb);
}
}
结果：
hetabcruellowxxxorld
hetabcruellowxxxorldbc
-----------------------可变字符串的删除------------------------
删除方法：
|StringBuffer
|delete|(int start, int end)
|Removes the  characters in a substring of this sequence.
|
|StringBuffer
|deleteCharAt|(int index)
|Removes the|char|at  the specified position in this sequence.
|
例子：
/**
* StringBuffer中的删除方法测试
***@****作者**李昆鹏
*
*/
**public****class**Test2 {
**public****static****void**main(String[]args) {
//创建可变字符串，开辟了一个默认是长度为0，容量为16个字符的空间
StringBuffersb=**new**StringBuffer("helloworld");
//第一个参数删除的开始索引，第二个参数是删除结束索引
//删除的包含开始索引，不包含结束索引，结束索引不存在越界
//如果是清空的话就是delete(0，sb.length())，
sb.delete(2,4);
System.**out**.println(sb);
//删除指定索引的字符，很少用
sb.deleteCharAt(1);
System.**out**.println(sb);
}
}
结果：
heoworld
howorld
---------------可变字符串的处理-------------------------
·替换
|StringBuffer
|replace|(int start, int end,|String|str)
|Replaces the  characters in a substring of this sequence with characters in the specified|String
|
·字符串的反转
|StringBuffer
|reverse|()
|Causes this  character sequence to be replaced by the reverse of the sequence.
|
·某个字符串的倒序索引
|int
|lastIndexOf|(|String|str)
|Returns the index  within this string of the rightmost occurrence of the specified substring.
|
|int
|lastIndexOf|(|String|str,  int fromIndex)
|Returns the index  within this string of the last occurrence of the specified substring.
|
例子：
**public****class**Test6 {
**public****static****void**main(String[]args) {
//创建可变字符串，开辟了一个默认是长度为0，容量为16个字符的空间
StringBuffersb=**new**StringBuffer("helloworld");
//第一个参数是替换的开始索引，第二个参数是替换结束索引
//第三个参数是替换的字符串
sb.replace(2,5,"xx");
System.**out**.println(sb);
//把字符串倒过来,字符串的反转
sb.reverse();
System.**out**.println(sb);
//获得倒数第一次出现的字符串
StringBuffersb1=**new**StringBuffer("helloworld");
**int**lastIndex=sb1.lastIndexOf("o");
System.**out**.println(lastIndex);
//获得从指定的索引开始向前数，找到倒数第一次出现的字符串的索引
lastIndex=sb1.lastIndexOf("o", 5);
System.**out**.println(lastIndex);
}
}
结果：
hexxworld
dlrowxxeh
6
4


