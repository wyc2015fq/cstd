
# java字符串判断方法 - 李昆鹏的博客 - CSDN博客


2018年03月15日 14:24:25[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：1508


-----------------------------------------**java字符串判断方法**---------------------------------------------
**字符串的判断方法**
**Boolean         endsWith****（****String  suffix****）**
**测试次字符串是否以指定的后缀结束。**
**Boolean       equals****（****Object anObject****）**
**将此字符串与指定的对象比较**
**Boolean         equalsIgnoreCase****（****String anotherString****）**
**将此****String****与另外一个****String****比较，不考虑大小写。**
**Boolean         contains(charSequence  s)**
**当且仅当此字符串包含指定的****char****值序列时，返回****true****。**
**Boolean         startsWith(String  prefix)**
**测试此字符串是否以指定的前缀开始**
**Boolean   isEmpty()**
**当且仅当****length****（）为****0****时返回****true**
**例子：**
**public****class**test4 {
**public****static****void**main(String[]args) {
//创建常量的字符串
Stringstr="helloworld";
//判断字符串是否以ld结尾
System.**out**.println(str.endsWith("ld"));
//判断两个字符串是否相等,equals比较的是值
System.**out**.println(str.equals("helloworld"));
//建议使用，不会发生空指针异常
System.**out**.println("helloworld".equals(str));
//不区分大小写的比较,验证码的校验
System.**out**.println("HELLOWORLD".equalsIgnoreCase(str));
//判断是否包含某段连续的字符串
System.**out**.println(str.contains("lo"));
//判断字符串是否以某段连续字符串开头
System.**out**.println(str.startsWith("he"));
//判断字符串是否是空串
System.**out**.println(str.isEmpty());
System.**out**.println("".isEmpty());
//获得字符串的长度,空格是字符，空串是空
System.**out**.println(str.length());
}
}
**结果：**
true
true
true
true
true
true
false
true
10

**--------------------------****空指针异常的原理****--------------------**
![](//img-blog.csdn.net/20180315142336282?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Exception in thread"main"java.lang.NullPointerException
at cn.tx.string.test4.main(test4.java:8)


