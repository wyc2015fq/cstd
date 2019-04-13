
# java字符串获取方法及作业题 - 李昆鹏的博客 - CSDN博客


2018年03月15日 19:22:35[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：267


-----------------------------------------**java字符串获取**---------------------------------------------
**字符串的获取功能**
**Int      length****（）**
**返回此字符串的长度**
**Char          charAt(int  index)**
**返回指定字符在此字符串中第一次出现的索引**
**Int      indexOf(int  ch)**
**返回指定字符在此字符串中第一次出现处的索引**
**Int      indexOf(String  str****）**
**返回指定子字符串在此字符串中第一次出现处的索引**
**Int      indexOf(String  str, int fromIndex)**
**返回指定子字符串在此字符串中第一次出现处的索引，从指定的索引开始。**
**String      substring(int  beginIndex)**
**返回一个新的字符串，它是此字符串的一个子字符串。**
**String substring(int beginIndex, int  endIndex)**
**返回一个新字符串，它是此字符串的一个字字符串。**
**Byte[] getBytes()**
**使用平台的默认字符集将此****String****编码为****byte****序列，并将结果存储到一个新的****byte****数组中。**
**Char[] toCharArray()**
**将此字符串转换为一个新的字符数组**
**Static String  valueOf(Boolean  b)**
**返回****boolean****参数的字符串表示形式**
**Static String  valueOf(char c)**
**返回****char****参数的字符串表示形式**
**Static String  valueOf(char[] data)**
**返回****char****数组参数的字符串表示形式**
**Static String  valueOf(char[] data****，****int  offset, int count)**
**返回****char****数组参数的特定子数组的字符串表示形式**
**Static String  valueOf(double d)**
**返回****double****参数的字符串表示形式**
**Static String  valueOf(float f)**
**返回****float****参数的字符串表示形式**
**Static String  valueOf(int i)**
**返回****int****参数的字符串表示形式**
**Static String  valueOf(long l)**
**返回****long****参数的字符串表示形式**
**String toUpperCase()**
**使用默认语言环境的规则将此****String****中的所有字符转换为大写**
**String toUpperCase(Locale  locale)**
**使用给定****Locale****的规则将此****String****中的所有字符转换为大写**
**String toLowerCase()**
**使用默认语言环境的规则将此****String****中的所有字符转换为小写**
**String toLowerCase(Locale  locale)**
**使用给定****Locale****的规则将此****String****中的所有字符转换为小写**
**String      concat(String  str)**
**将指定字符串链接到此字符串的结尾**
**例子：**
**public****class**test5 {
**public****static****void**main(String[]args) {
//创建常量的字符串
Stringstr="helloworld";
//获得字符串的长度,空格是字符，空串是空
System.**out**.println(str.length());
//返回指定索引处的字符
**char**c=str.charAt(5);
System.**out**.println(c);
//查找自定字符索引
**int**index=str.indexOf('l');
System.**out**.println(index);
//查找指定的字符串第一次出现的索引，如果返回-1说明不存在要查找的字符串
**int**index1=str.indexOf("llo");
System.**out**.println(index1);
**int**index2=str.indexOf("aa");
System.**out**.println(index2);
//从指定索引处查找第一次出现字符串的索引,包含开始索引的本身,
**int**index3=str.indexOf("w", 5) ;
System.**out**.println(index3);
//获得子字符串,包括索引本身的字符
Stringstr1=str.substring(4);
System.**out**.println(str1);
//根据开始和结束的索引获得子字符串，包含开始索引的本身,不包括结尾的索引
Stringstr2=str.substring(2,5);
System.**out**.println(str2);
//获得字符串对应的字节数组
**byte**[]bs=str.getBytes();
**for**(**int**i= 0;i<bs.length;i++) {
System.**out**.print(bs[i] +"\t");
}
System.**out**.println();
//把字符串转换成字符数组
**char**[]cs=str.toCharArray();
**for**(**int**i= 0;i<cs.length;i++) {
System.**out**.print(cs[i] +"\t");
}
System.**out**.println();
//把boolean类型转换成字符串类型
StringstrBooblen= String.*valueOf*(**true**);
System.**out**.println(strBooblen);
StringcArray= String.*valueOf*(**new****char**[] {'1','g','g','h'});
System.**out**.println(cArray);
//把字符串转换成大写
Stringstr6="txjava";
System.**out**.println(str6.toUpperCase());
//把字符串转换成小写
Stringstr7="TXJAVA";
System.**out**.println(str7.toLowerCase());
//拼接字符串
System.**out**.println(str6.concat(str7));
}
}
结果：
10
w
2
2
-1
5
oworld
llo
104   101   108   108   111   119   111   114   108   100
h  e  l  l  o  w  o  r  l  d
true
1ggh
TXJAVA
txjava
txjavaTXJAVA
------------计算一个字符串中大写字母和小写字母还有数字的数量---------------
**import**java.util.Scanner;
/**
*计算一个字符串中大写字母和小写字母还有数字的数量
***@author**Administrator
*
*/
**public****class**test6 {
**public****static****void**main(String[]args) {
test6t=**new**test6();
t.outputResult();
}
/**
*输入字符串并输出此字符串中大小写，数字的数量
*/
**public****void**outputResult() {
printResult(inputString());
}
/**
*输入一个要计算的字符串
***@return**
*/
**public**String inputString(){
System.**out**.print("请输入一个字符串：");
**return****new**Scanner(System.**in**).nextLine();
}

/**
*计算并打印大小写字母和数字的数量
***@param**str
*/
**public****void**printResult(Stringstr) {
//计算大写字母的变量
**int**capital= 0;
//计算小写字母的变量
**int**lowercase= 0;
//计算数字的变量
**int**num= 0;
**for**(**int**i= 0;i<str.length();i++) {
//判断是否是大写字母
**if**(str.charAt(i) >'A'&&str.charAt(i) <'Z') {
capital++;
}
//判断是否是小写字母
**if**(str.charAt(i) >'a'&&str.charAt(i) <'z') {
lowercase++;
}
//判断是否是字符串
**if**(str.charAt(i) >'0'&&str.charAt(i) <'9') {
num++;
}
}
System.**out**.println("大写字母一共有："+capital+"个");
System.**out**.println("小写字母一共有："+lowercase+"个");
System.**out**.println("数字一共有："+num+"个");
}
}
**结果：**
请输入一个字符串：123sdasfsdfASFSD
大写字母一共有：4个
小写字母一共有：7个
数字一共有：3个
---------------------**把一个字符串的首字母边成大写，其余变成小写**-----------
/**
*把一个字符串的首字母边成大写，其余变成小写
*第一步要获得首字母，还要获得除了首字符以外的字符串
*第二步把首字母大写
*第三步把大写的字符和后面变成小写的字符拼接上
*/
**import**java.util.Scanner;
**public****class**test8 {
**public****static****void**main(String[]args) {
*callMethod*();
}
**public****static****void**callMethod() {
**try**{
*getResult*();
}**catch**(Exceptione) {
System.**out**.println("请输入字符串：");
*callMethod*();
}
}
**public****static****void**getResult() {
Scannersc=**new**Scanner(System.**in**);
//获得输入的字符串
System.**out**.println("请输入字符串：");
Stringline=sc.nextLine();
//获得第一个字符，但是获得第一个字符的过程中可能会出现异常，如果出现异常就向上抛
Stringresult=line.substring(0,1)
.toUpperCase()
.concat(line.substring(1)
.toLowerCase());
;
System.**out**.println(result);
}
}
结果
请输入字符串：
afasfa
Afasfa


