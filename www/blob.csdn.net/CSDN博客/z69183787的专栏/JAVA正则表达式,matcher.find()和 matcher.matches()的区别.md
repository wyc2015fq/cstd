# JAVA正则表达式,matcher.find()和 matcher.matches()的区别 - z69183787的专栏 - CSDN博客
2017年12月06日 19:18:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：774
[https://www.cnblogs.com/hubingxu/archive/2012/02/17/2355516.html](https://www.cnblogs.com/hubingxu/archive/2012/02/17/2355516.html)
1.find()方法是部分匹配，是查找输入串中与模式匹配的子串，如果该匹配的串有组还可以使用group()函数。
matches()是全部匹配，是将整个输入串与模式匹配，如果要验证一个输入的数据是否为数字类型或其他类型，一般要用matches()。
2.Pattern pattern= Pattern.compile(".*?,(.*)");
    Matcher matcher = pattern.matcher(result);
  if (matcher.find()) {
   return matcher.group(1);
  }
3.详解：
matches
public static boolean matches(String regex,  CharSequence input)
编译给定正则表达式并尝试将给定输入与其匹配。  
调用此便捷方法的形式  
Pattern.matches(regex, input);
Pattern.compile(regex).matcher(input).matches() ; 
如果要多次使用一种模式，编译一次后重用此模式比每次都调用此方法效率更高。
参数：
regex - 要编译的表达式
input - 要匹配的字符序列  
抛出：  
PatternSyntaxException - 如果表达式的语法无效
find
public boolean find()尝试查找与该模式匹配的输入序列的下一个子序列。  
此方法从匹配器区域的开头开始，如果该方法的前一次调用成功了并且从那时开始匹配器没有被重置，则从以前匹配操作没有匹配的第一个字符开始。  
如果匹配成功，则可以通过 start、end 和 group 方法获取更多信息。  
matcher.start() 返回匹配到的子字符串在字符串中的索引位置. 
matcher.end()返回匹配到的子字符串的最后一个字符在字符串中的索引位置. 
matcher.group()返回匹配到的子字符串 
返回：
当且仅当输入序列的子序列匹配此匹配器的模式时才返回 true。
4.部分JAVA正则表达式实例
   ①字符匹配 
   Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(str); // 操作的字符串 
   boolean b = m.matches(); //返回是否匹配的结果 
   System.out.println(b); 
   Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(str); // 操作的字符串 
   boolean b = m. lookingAt (); //返回是否匹配的结果 
   System.out.println(b); 
   Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(str); // 操作的字符串 
   boolean b = m..find (); //返回是否匹配的结果 
   System.out.println(b); 
②分割字符串 
Pattern pattern = Pattern.compile(expression); //正则表达式 
                     String[] strs = pattern.split(str); //操作字符串 得到返回的字符串数组 
③替换字符串 
   Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(text); // 操作的字符串 
   String s = m.replaceAll(str); //替换后的字符串 
④查找替换指定字符串 
Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(text); // 操作的字符串 
   StringBuffer sb = new StringBuffer(); 
   int i = 0; 
   while (m.find()) { 
    m.appendReplacement(sb, str); 
    i++;    //字符串出现次数 
   } 
   m.appendTail(sb);//从截取点将后面的字符串接上 
String s = sb.toString(); 
⑤查找输出字符串 
Pattern p = Pattern.compile(expression); // 正则表达式 
   Matcher m = p.matcher(text); // 操作的字符串 
   while (m.find()) { 
matcher.start() ;
matcher.end();
matcher.group(1);
  }
原来，group是针对（）来说的，group（0）就是指的整个串，group（1） 指的是第一个括号里的东西，group（2）指的第二个括号里的东西。
最近学习正则表达式，发现[Java](http://lib.csdn.net/base/17)中的一些术语与其他地方描述的有所差异。比如Java正则表达式中的“组”概念与《正则表达式必知必会》一书中讲述的“子表达式”其实是一样的，只是表述不同而已。由此也引发了使用JavaAPI时对group(int
 group)、start(int group)、end(int group)不是太理解。 
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.enation.newtest;
import java.io.*;
import java.util.regex.*;
import java.net.*;
public class  MailTest{
     public static void main(String[] args) throws Exception{
         
         String regEx = "count(\\d+)(df)";  
         String s = "count000dfdfsdffaaaa1";  
         Pattern pat = Pattern.compile(regEx);  
         Matcher mat = pat.matcher(s);  
         if(mat.find()){
            System.out.println(mat.group(2));
         }
     }
         
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
输出结果
mat.group() 输出为 count000df
mat.group(1) 输出为 000mat.group(2) 输出为 df如果没有括号会有异常。这就是（） 的作用。
如何没有（） 可以这样写：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public static void main(String []args){
     String regEx = "count\\d+";  
     String s = "count000dfdfsdff1";  
     Pattern pat = Pattern.compile(regEx);  
     Matcher mat = pat.matcher(s);  
    if(mat.find()){
        System.out.println(mat.group());
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
但 输出 会按照  "count\\d+";   正则输出.
结果是：count000
