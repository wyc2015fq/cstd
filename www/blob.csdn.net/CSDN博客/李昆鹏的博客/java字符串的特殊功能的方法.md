
# java字符串的特殊功能的方法 - 李昆鹏的博客 - CSDN博客


2018年03月16日 09:27:06[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：125标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[JAVASE																](https://blog.csdn.net/weixin_41547486/article/category/7484968)


**-------------------------java字符串的特殊功能的方法------------------**
其他功能
String[]    split(String regex)
根据给定正则表达式的匹配拆分此字符串
**如果是特殊字符需要转义使用反斜线****"\\"**
**\****在****java****中代表****\****后面的符号都是内容**
**使用****\****在字符串中需要用两个****\\,****分割的时候需要用四个****\\\\****如****3000\\5000****；****st.split("\\\\")**
**public****class**test9 {
**public****static****void**main(String[]args) {
Stringstr="3000-3999-uuu-999-kkk-2525";
Stringst="3000.3999.uuu.999.kkk.2525";
//分割字符串
String[]strs=str.split("-");
//如果是特殊字符需要转义使用反斜线"\\",即使不是特殊字符加反斜线都无影响
//*代表所有的意思
// \在java中代表\后面的符号都是内容
//使用\在字符串中需要用两个\\,分割的时候需要用四个\\\\
//如3000\\5000；st.split("\\\\")
//如果想输出字符串中的\t就需要多加一个\\t
//如ab\\tab
String[]strs1=st.split("\\.");
**for**(**int**i= 0;i<strs.length;i++) {
Stringstr1=strs[i];
System.**out**.println(str1);
System.**out**.println(strs1[i]);
}
}
}
结果
3000
3000
3999
3999
uuu
uuu
999
999
kkk
kkk
2525
2525

String      replace(char  oldChar, char newChar)
返回一个新的字符串，它是通过用newChar替换词字符串中出现的所有oldChar得到的。
**public****class**test10 {
**public****static****void**main(String[]args) {
Stringstr="3000-3999-uuu-999-kkk-2525";
//字符串中的单个字符替换
//第一个参数是被替换的字符，第二个参数是新的字符
Stringstr1=str.replace('-','+');
System.**out**.println(str1);
}
}
结果：
3000+3999+uuu+999+kkk+2525
String      replace(charSequence  target, CharSequence  replacement)
使用指定的字面值替换序列替换此字符串所有匹配字面值目标序列的子字符串
//字符串中的子字符串替换
//第一个参数是被替换的子字符串，第二个参数是新的子字符串
Stringstr2=str.replace("999","888");
System.**out**.println(str2);
结果：
3000-3888-uuu-888-kkk-2525
String      trim()
返回字符串的副本，忽略前导空白和尾部空白
Stringstr3="txjava    ";
//去掉字符串两边的空格
Stringstr4=str3.trim();
System.**out**.println(str3);
System.**out**.println(str4);
**Int**`lastIndexOf``(int ch)`
**返回指定字符在此字符串中最后一次出现处的索引。**
Stringstr3="txjava    ";
//从后面开始查找第一次出现的a
**int**index=str3.lastIndexOf("a");
System.**out**.println(index);
**Int**`lastIndexOf``(int ch,int fromIndex)`
**返回指定字符在此字符串中最后一次出现处的索引，从指定的索引处开始进行反向搜索。**
Stringstr3="txjava    ";
//在给定的索引前面倒叙查找第一次出现子字符串的索引
**int**index1=str3.lastIndexOf("a", 9);
System.**out**.println(index1);
--------------------------练习题-------------------------------------
/**
***需求：找出下面字符串中****li****的数量**
* "liasdflihsdhllihsdflihsdffiligsdfglikhsdfklilisdflio"
*
*/
**public****class**test1 {
**public****static****void**main(String[]args) {
Stringstr="liasdflihsdhllihsdflihsdffiligsdfglikhsdfklilisdflio";
**int**count= 0;
//获得li的第一次出现的索引
**int**index=str.indexOf("li");
System.**out**.println(index);
//循环字符串
**while**(index!= -1) {
count++;
index=str.indexOf("li",index+2);
System.**out**.println(index);
}
System.**out**.println("li的出现次数是"+count);
}
}
结果：
0
6
13
19
27
34
42
44
49
-1
li的出现次数是9


