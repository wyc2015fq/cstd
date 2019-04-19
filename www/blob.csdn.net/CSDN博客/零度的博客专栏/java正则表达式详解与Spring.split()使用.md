# java正则表达式详解与Spring.split()使用 - 零度的博客专栏 - CSDN博客
2016年04月18日 16:49:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：8575
今天用split这个函数时碰到了问题，我的String是  a.b 我用 split(".")，居然返回的数组长度是0，后来网上查了才知道 split参数是 正则表达式，以前一直以为是字符串，而“.”在正则表达式中有特殊含义，代表任何字符串。
转载一篇文章：[http://shukuiyan.iteye.com/blog/1058672](http://shukuiyan.iteye.com/blog/1058672)
  之前在http://shukuiyan.iteye.com/blog/507915文中已经叙述过这个问题，但是最近一次笔试中居然有碰到了这个知识点，而且还做错了，囧！学艺不精啊。题目大概是这样的：
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String s2="this is a test";  
- String sarray[]=s2.split("/s");  
- System.out.println("sarray.length="+sarray.length);  
这个输出是什么还是编译出错？我想那个split方法中的参数要是"s"的话，输出一定是4了，就是将原来字符串分成了数组{"thi","i","a te","t"}，但是原题split方法中的参数要是"/s"，那么这个参数到底是嘛意思呢，实验后输出结果居然是1。
原因还得深究啊。
    java.lang.string.split，即split 方法，它实现的功能是将一个字符串分割为子字符串，然后将结果作为字符串数组返回。 格式为：
                 stringObj.split([separator，[limit]]) 
其中stringObj是必选项，表示要被分解的 String 对象或文字。该对象不会被 split 方法修改。 separator 为可选项，表示字符串或正则表达式对象，它标识了分隔字符串时使用的是一个还是多个字符。如果忽略该选项，返回包含整个字符串的单一元素数组。 limit 为可选项，该值用来限制返回数组中的元素个数。 值得注意的是： split 方法的结果是一个字符串数组，在 stingObj 中每个出现 separator 的位置都要进行分解，separator 不作为任何数组元素的部分返回。
一个例子
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String srcstring="this is a about split test";  
- String stringarray[]=srcstring.split(" ");  
- //// 在每个空格字符处进行分解
- for(String stemp:stringarray){  
-     System.out.println(stemp);  
- }  
- String srcstring1=" this  is a about split  test";//有n个空格的话，分成的数组长度为n+1
- //如果字符串中有多个空格时，则两个空格间认为是没有字符，结果字符串数组中该位置为空。
- String stringarray1[]=srcstring1.split(" ");  
- for(String stemp:stringarray1){  
-     System.out.println(stemp);  
- }  
这样输出结果为
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- this
- is  
- a  
- about  
- split  
- test  
- 
- 另一个：  
- 
- this
- 
- is  
- a  
- about  
- split  
- 
- test  
另外一个例子
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String srcstring="this is a about split test";  
- String stringarray[]=srcstring.split(" ",2);  
- //// 在每个空格字符处进行分解
- for(String stemp:stringarray){  
-        System.out.println(stemp);  
- }  
- 输出结果为  
- this
- is a about split test  
 看看下面这个
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String ipstring="59.64.159.224";  
- String iparray[]=ipstring.split(".");  
- for(String stemp:iparray){  
-     System.out.println(stemp);  
- }  
- 这个输出为空，为什么呢？  
因为 public string[] split(string regex) 这里的参数的名称是regex ，也就是 regular expression （正则表达式）。这个参数并不是一个简单的分割用的字符，而是一个正则表达式，以下是split 方法的实现代码：
   public string[] split(string regex, int limit) { 
        return pattern.compile(regex).split(this, limit); 
   }
split 的实现直接调用的 matcher 类的 split 的方法。我们知道，“ . ”在正则表达式中有特殊的含义，因此我们使用的时候必须进行转义。 只要将
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String iparray[]=ipstring.split(".");  
改为
Java代码  ![收藏代码](http://shukuiyan.iteye.com/images/icon_star.png)
- String iparray[]=ipstring.split("\\.");  
就可以了。
那么这里在列上一些转义字符
 \\ 反斜杠 
\t 间隔 ('\u0009')
\n 换行 ('\u000A')
\r 回车 ('\u000D')
\d 数字 等价于 [0-9]
\D 非数字 等价于 [^0-9]
\s 空白符号 [\t\n\x0B\f\r]
\S 非空白符号 [^\t\n\x0B\f\r]
\w 单独字符 [a-zA-Z_0-9]
\W 非单独字符 [^a-zA-Z_0-9]
\f 换页符 
\e Escape
\b 一个单词的边界 
\B 一个非单词的边界 
\G 前一个匹配的结束
注意：public String[] split(String regex,int limit)根据匹配给定的正则表达式来拆分此字符串。 
此方法返回的数组包含此字符串的每个子字符串，这些子字符串由另一个匹配给定的表达式的子字符串终止或由字符串结束来终止。数组中的子字符串按它们在此字符串中的顺序排列。如果表达式不匹配输入的任何部分，则结果数组只具有一个元素，即此字符串。 
limit 参数控制模式应用的次数，因此影响结果数组的长度。如果该限制 n 大于 0，则模式将被最多应用 n - 1 次，数组的长度将不会大于 n，而且数组的最后项将包含超出最后匹配的定界符的所有输入。如果 n 为非正，则模式将被应用尽可能多的次数，而且数组可以是任意长度。如果 n 为零，则模式将被应用尽可能多的次数，数组可有任何长度，并且结尾空字符串将被丢弃。
回到最初的题目，题目中给出的匹配正则表达式为"/s"，表示空白字符，此时给定的字符串中没有匹配的字符，则输出为原字符串，所以输出的字符长度为1.
再附上一些关于java中正则表达式的知识吧
^ 为限制开头 
^java     条件限制为以 Java 为开头字符 
$ 为限制结尾 
java$     条件限制为以 java 为结尾字符 
.  条件限制除 \n 以外任意一个单独字符 
java..     条件限制为 java 后除换行外任意两个字符
加入特定限制条件「 [] 」 
[a-z]     条件限制在小写 a to z 范围中一个字符 
[A-Z]     条件限制在大写 A to Z 范围中一个字符 
[a-zA-Z] 条件限制在小写 a to z 或大写 A to Z 范围中一个字符 
[0-9]     条件限制在小写 0 to 9 范围中一个字符 
[0-9a-z] 条件限制在小写 0 to 9 或 a to z 范围中一个字符 
[0-9[a-z]] 条件限制在小写 0 to 9 或 a to z 范围中一个字符 ( 交集 )
[] 中加入 ^ 后加再次限制条件「 [^] 」 
[^a-z]     条件限制在非小写 a to z 范围中一个字符 
[^A-Z]     条件限制在非大写 A to Z 范围中一个字符 
[^a-zA-Z] 条件限制在非小写 a to z 或大写 A to Z 范围中一个字符 
[^0-9]     条件限制在非小写 0 to 9 范围中一个字符 
[^0-9a-z] 条件限制在非小写 0 to 9 或 a to z 范围中一个字符 
[^0-9[a-z]] 条件限制在非小写 0 to 9 或 a to z 范围中一个字符 ( 交集 )
在限制条件为特定字符出现 0 次以上时，可以使用「 * 」 
J*     0 个以上 J
.*     0 个以上任意字符 
J.*D     J 与 D 之间 0 个以上任意字符 
在限制条件为特定字符出现 1 次以上时，可以使用「 + 」 
J+     1 个以上 J
.+     1 个以上任意字符 
J.+D     J 与 D 之间 1 个以上任意字符 
在限制条件为特定字符出现有 0 或 1 次以上时，可以使用「 ? 」 
JA?     J 或者 JA 出现
限制为连续出现指定次数字符「 {a} 」 
J{2}     JJ
J{3}     JJJ
文字 a 个以上，并且「 {a,} 」 
J{3,}     JJJ,JJJJ,JJJJJ,???(3 次以上 J 并存 )
文字个以上， b 个以下「 {a,b} 」 
J{3,5}     JJJ 或 JJJJ 或 JJJJJ
两者取一「 | 」 
J|A     J 或 A
Java|Hello     Java 或 Hello
「 () 」中规定一个组合类型 
比如，我查询 <a href=\"index.html\">index</a> 中 <a href></a> 间的数据，可写作 <a.*href=\".*\">(.+?)</a>
在使用 Pattern.compile 函数时，可以加入控制正则表达式的匹配行为的参数： 
Pattern Pattern.compile(String regex, int flag)
flag 的取值范围如下： 
Pattern.CANON_EQ     当且仅当两个字符的 " 正规分解 (canonical decomposition)" 都完全相同的情况下，才认定匹配。比如用了这个标志之后，表达式 "a\u030A" 会匹配 "?" 。默认情况下，不考虑 " 规范相等性 (canonical equivalence)" 。 
Pattern.CASE_INSENSITIVE(?i)     默认情况下，大小写不明感的匹配只适用于 US-ASCII 字符集。这个标志能让表达式忽略大小写进行匹配。要想对 Unicode 字符进行大小不明感的匹配，只要将 UNICODE_CASE 与这个标志合起来就行了。 
Pattern.COMMENTS(?x)     在这种模式下，匹配时会忽略 ( 正则表达式里的 ) 空格字符 ( 译者注：不是指表达式里的 "\\s" ，而是指表达式里的空格， tab ，回车之类 ) 。注释从 # 开始，一直到这行结束。可以通过嵌入式的标志来启用 Unix 行模式。
Pattern.DOTALL(?s)     在这种模式下，表达式 '.' 可以匹配任意字符，包括表示一行的结束符。默认情况下，表达式 '.' 不匹配行的结束符。 
Pattern.MULTILINE
(?m)     在这种模式下， '^' 和 '$' 分别匹配一行的开始和结束。此外， '^' 仍然匹配字符串的开始， '$' 也匹配字符串的结束。默认情况下，这两个表达式仅仅匹配字符串的开始和结束。 
Pattern.UNICODE_CASE
(?u)     在这个模式下，如果你还启用了 CASE_INSENSITIVE 标志，那么它会对 Unicode 字符进行大小写不明感的匹配。默认情况下，大小写不敏感的匹配只适用于 US-ASCII 字符集。 
Pattern.UNIX_LINES(?d)     在这个模式下，只有 '\n' 才被认作一行的中止，并且与 '.' ， '^' ，以及 '$' 进行匹配。
抛开空泛的概念，下面写出几个简单的 Java 正则用例： 
◆ 比如，在字符串包含验证时 
// 查找以 Java 开头 , 任意结尾的字符串 
  Pattern pattern = Pattern.compile("^Java.*");
  Matcher matcher = pattern.matcher("Java 不是人 ");
  boolean b= matcher.matches();
  // 当条件满足时，将返回 true ，否则返回 false
  System.out.println(b);
◆ 以多条件分割字符串时 
Pattern pattern = Pattern.compile("[, |]+");
String[] strs = pattern.split("Java Hello World  Java,Hello,,World|Sun");
for (int i=0;i<strs.length;i++) {
    System.out.println(strs[i]);
} 
◆ 文字替换（首次出现字符） 
Pattern pattern = Pattern.compile(" 正则表达式 ");
Matcher matcher = pattern.matcher(" 正则表达式 Hello World, 正则表达式 Hello World");
// 替换第一个符合正则的数据 
System.out.println(matcher.replaceFirst("Java"));
◆ 文字替换（全部） 
Pattern pattern = Pattern.compile(" 正则表达式 ");
Matcher matcher = pattern.matcher(" 正则表达式 Hello World, 正则表达式 Hello World");
// 替换第一个符合正则的数据 
System.out.println(matcher.replaceAll("Java"));
◆ 文字替换（置换字符） 
Pattern pattern = Pattern.compile(" 正则表达式 ");
Matcher matcher = pattern.matcher(" 正则表达式 Hello World, 正则表达式 Hello World ");
StringBuffer sbr = new StringBuffer();
while (matcher.find()) {
    matcher.appendReplacement(sbr, "Java");
}
matcher.appendTail(sbr);
System.out.println(sbr.toString());
◆ 验证是否为邮箱地址 
String str="ceponline@yahoo.com.cn";
Pattern pattern = Pattern.compile("[\\w\\.\\-]+@([\\w\\-]+\\.)+[\\w\\-]+",Pattern.CASE_INSENSITIVE);
Matcher matcher = pattern.matcher(str);
System.out.println(matcher.matches());
◆ 去除 html 标记 
Pattern pattern = Pattern.compile("<.+?>", Pattern.DOTALL);
Matcher matcher = pattern.matcher("<a href=\"index.html\"> 主页 </a>");
String string = matcher.replaceAll("");
System.out.println(string);
◆ 查找 html 中对应条件字符串 
Pattern pattern = Pattern.compile("href=\"(.+?)\"");
Matcher matcher = pattern.matcher("<a href=\"index.html\"> 主页 </a>");
if(matcher.find())
  System.out.println(matcher.group(1));
}
◆ 截取 http:// 地址 
// 截取 url
Pattern pattern = Pattern.compile("(http://|https://){1}[\\w\\.\\-/:]+");
Matcher matcher = pattern.matcher("dsdsds<http://dsds//gfgffdfd>fdf");
StringBuffer buffer = new StringBuffer();
while(matcher.find()){              
    buffer.append(matcher.group());        
    buffer.append("\r\n");              
System.out.println(buffer.toString());
}
◆ 替换指定 {} 中文字 
String str = "Java 目前的发展史是由 {0} 年 -{1} 年 ";
String[][] object={new String[]{"\\{0\\}","1995"},new String[]{"\\{1\\}","2007"}};
System.out.println(replace(str,object));
public static String replace(final String sourceString,Object[] object) {
            String temp=sourceString;    
            for(int i=0;i<object.length;i++){
                      String[] result=(String[])object[i];
               Pattern    pattern = Pattern.compile(result[0]);
               Matcher matcher = pattern.matcher(temp);
               temp=matcher.replaceAll(result[1]);
            }
            return temp;
}
