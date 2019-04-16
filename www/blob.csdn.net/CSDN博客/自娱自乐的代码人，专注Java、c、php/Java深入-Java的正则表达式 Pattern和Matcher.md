# Java深入 - Java的正则表达式 Pattern和Matcher - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月28日 16:55:05[initphp](https://me.csdn.net/initphp)阅读数：2051
个人分类：[Java深入系列](https://blog.csdn.net/initphp/article/category/2332145)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)[Java深入](https://blog.csdn.net/column/details/myjava.html)









### Pattern和Matcher

java.util.regex 是一个用正则表达式所订制的模式来对字符串进行匹配工作的类库包。它包括两个类：**Pattern**和**Matcher**

Pattern： 一个Pattern是一个**正则表达式经编译后的表现模式**。 

Matcher： 一个Matcher对象是一个状态机器，**它依据Pattern对象做为匹配模式对字符串展开匹配检查**。






### Pattern类



**常用方法**：


static Pattern compile(String regex)     将给定的正则表达式编译并赋予给Pattern类


static Pattern compile(String regex, int flags)     flag参数的指定，可选的flag参数包括：CASE INSENSITIVE,MULTILINE,DOTALL,UNICODE CASE， CANON E


int flags()   返回当前Pattern的匹配flag参数.



```java
Pattern.CANON_EQ，当且仅当两个字符的"正规分解(canonical decomposition)"都完全相同的情况下，才认定匹配。

Pattern.CASE_INSENSITIVE(?i) 默认情况下，大小写不敏感的匹配只适用于US-ASCII字符集。这个标志能让表达式忽略大小写进行匹配。

Pattern.COMMENTS(?x)在这种模式下，匹配时会忽略(正则表达式里的)空格字符(不是指表达式里的"\\s"，而是指表达式里的空格，tab，回车之类)。

Pattern.DOTALL(?s)在这种模式下，表达式'.'可以匹配任意字符，包括表示一行的结束符。 默认情况下，表达式'.'不匹配行的结束符。

Pattern.MULTILINE(?m)在这种模式下，'^'和'$'分别匹配一行的开始和结束。
此外，'^'仍然匹配字符串的开始，'$'也匹配字符串的结束。默认情况下，这两个表达式仅仅匹配字符串的开始和结束。

Pattern.UNICODE_CASE(?u)在这个模式下，如果你还启用了CASE_INSENSITIVE标志，
那么它会对Unicode字符进行大小写不明感的匹配。默认情况下，大小写不敏感的匹配只适用于US-ASCII字符集。

Pattern.UNIX_LINES(?d)在这个模式下，只有'\n'才被认作一行的中止，并且与'.'，'^'，以及'$'进行匹配。
```





Matcher matcher(CharSequence input)    生成一个给定命名的Matcher对象



```java
Pattern p = Pattern.compile("《(.*)》");
        Matcher m = p.matcher("《测试》匹配书名号中间的内容");
        while (m.find()) {
            System.out.println(m.group(1));
        }
```




static boolean matches(String regex, CharSequence input) 


String[] split(CharSequence input)  将目标字符串按照Pattern里所包含的正则表达式为模进行分割。 


String[] split(CharSequence input, int limit)   作用同上，增加参数limit目的在于要指定分割的段数



```java
Pattern p = Pattern.compile("\\|");
        String[] arr = p.split("woshishen|sadsadsa");
        System.out.println(arr[0] + "----" + arr[1]);
```






### Matcher类



Matcher appendReplacement(StringBuffer sb, String replacement)       将当前匹配子串替换为指定字符串，并且将替换后的子串以及其之前到上次匹配子串之后的字符串段添加到一个StringBuffer对象里。

StringBuffer appendTail(StringBuffer sb)      将最后一次匹配工作后剩余的字符串添加到一个StringBuffer对象里。

int end()     返回当前匹配的子串的最后一个字符在原目标字符串中的索引位置 。 


int end(int group)    返回与匹配模式里指定的组相匹配的子串最后一个字符的位置。

boolean find()    尝试在目标字符串里查找下一个匹配子串。 

boolean find(int start)   重设Matcher对象，并且尝试在目标字符串里从指定的位置开始查找下一个匹配的子串。 

String group()   返回当前查找而获得的与组匹配的所有子串内容 

String group(int group)   返回当前查找而获得的与指定的组匹配的子串内容 

int groupCount()   返回当前查找所获得的匹配组的数量。 

boolean lookingAt()   检测目标字符串是否以匹配的子串起始。 

boolean matches()   尝试对整个目标字符展开匹配检测，也就是只有整个目标字符串完全匹配时才返回真值。 

Pattern pattern()    返回该Matcher对象的现有匹配模式，也就是对应的Pattern 对象。 

String replaceAll(String replacement)   将目标字符串里与既有模式相匹配的子串全部替换为指定的字符串。 

String replaceFirst(String replacement)   将目标字符串里第一个与既有模式相匹配的子串替换为指定的字符串。 

Matcher reset()   重设该Matcher对象。 

Matcher reset(CharSequence input)   重设该Matcher对象并且指定一个新的目标字符串。 

int start()    返回当前查找所获子串的开始字符在原目标字符串中的位置。 

int start(int group)   返回当前查找所获得的和指定组匹配的子串的第一个字符在原目标字符串中的位置。 



```java
Pattern p = Pattern.compile("(第(.+)部)");
        Matcher m = p.matcher("大宋提刑官 第1");
        while (m.find()) {
            System.out.println(m.start()); //返回当前查找所获子串的开始字符在原目标字符串中的位置。
            System.out.println(m.end()); //返回与匹配模式里指定的组相匹配的子串最后一个字符的位置。
            System.out.println(m.group(2)); //返回当前查找而获得的与指定的组匹配的子串内容 
        }
```














