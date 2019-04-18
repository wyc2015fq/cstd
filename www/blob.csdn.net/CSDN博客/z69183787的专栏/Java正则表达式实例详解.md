# Java正则表达式实例详解 - z69183787的专栏 - CSDN博客
2014年02月11日 10:04:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1189
你可以从比较简单的东西入手学习正则表达式。要想全面地掌握怎样构建正则表达式，可以去看JDK 文档的java.util.regex 的Pattern 类的文档。
\e
Escape
正则表达式的强大体现在它能定义字符集(character class)。下面是一些最常见的字符集及其定义的方式，此外还有一些预定义的字符集：
\W
一个非单词的字符，[^\w]
如果你用过其它语言的正则表达式，那么你一眼就能看出反斜杠的与众不同。在其它语言里，"\\ "的意思是"我只是要在正则表达式里插入一个反斜杠。没什么特别的意思。"但是在Java里，"\\ "的意思是"我要插入一个正则表达式的反斜杠，所以跟在它后面的那个字符的意思就变了。"举例来说，如果你想表示一个或更多的"单词字符"，那么这个正则表达式就应该是"\\w+ "。如果你要插入一个反斜杠，那就得用"\\\\ "。不过像换行，跳格之类的还是只用一根反斜杠："\n\t"。
这里只给你讲一个例子；你应该JDK 文档的java.util.regex.Pattern 加到收藏夹里，这样就能很容易地找到各种正则表达式的模式了。
(X)
一个"要匹配的组(capturing group)". 以后可以用\i来表示第i个被匹配的组。\G
前一个匹配的结束
举一个具体一些的例子。下面这些正则表达式都是合法的，而且都能匹配"Rudolph"：
> Rudolph
[rR]udolph
[rR][aeiou][a-z]ol.*
R.*
### 数量表示符
"数量表示符(quantifier)"的作用是定义模式应该匹配多少个字符。
- Greedy(贪婪的)： 除非另有表示，否则数量表示符都是greedy的。Greedy的表达式会一直匹配下去，直到匹配不下去为止。(如果你发现表达式匹配的结果与预期的不符) ，很有可能是因为，你以为表达式会只匹配前面几个字符，而实际上它是greedy的，因此会一直匹配下去。
- Reluctant(勉强的)： 用问号表示，它会匹配最少的字符。也称为lazy, minimal matching, non-greedy, 或ungreedy。
- Possessive(占有的)： 目前只有Java支持(其它语言都不支持)。它更加先进，所以你可能还不太会用。用正则表达式匹配字符串的时候会产生很多中间状态，(一般的匹配引擎会保存这种中间状态，) 这样匹配失败的时候就能原路返回了。占有型的表达式不保存这种中间状态，因此也就不会回头重来了。它能防止正则表达式的失控，同时也能提高运行的效率。
|Greedy|Reluctant|Possessive|匹配|
|----|----|----|----|
|X?|X??|X?+|匹配一个或零个X|
|X*|X*?|X*+|匹配零或多个X|
|X+|X+?|X++|匹配一个或多个X|
|X{n}|X{n}?|X{n}+|匹配正好n个X|
|X{n,}|X{n,}?|X{n,}+|匹配至少n个X|
|X{n,m}|X{n,m}?|X{n,m}+|匹配至少n个，至多m个X|
再提醒一下，要想让表达式照你的意思去运行，你应该用括号把'X'括起来。比方说：
> abc+
似乎这个表达式能匹配一个或若干个'abc'，但是如果你真的用它去匹配'abcabcabc'的话，实际上只会找到三个字符。因为这个表达式的意思是'ab'后边跟着一个或多个'c'。要想匹配一个或多个完整的'abc'，你应该这样：
> (abc)+
正则表达式能轻而易举地把你给耍了；这是一种建立在Java 之上的新语言。
#### CharSequence
JDK 1.4定义了一个新的接口，叫CharSequence 。它提供了String 和StringBuffer 这两个类的字符序列的抽象：
> interface
 CharSequence {
  charAt(int
 i);
  length();
  subSequence(int
 start, int
 end);
  toString();
}
为了实现这个新的CharSequence 接口，String ，StringBuffer 以及CharBuffer 都作了修改。很多正则表达式的操作都要拿CharSequence 作参数。
### Pattern 和Matcher
先给一个例子。下面这段程序可以测试正则表达式是否匹配字符串。第一个参数是要匹配的字符串，后面是正则表达式。正则表达式可以有多个。在Unix/Linux环境下，命令行下的正则表达式还必须用引号。
当你创建正则表达式时，可以用这个程序来判断它是不是会按照你的要求工作。
//: c12:TestRegularExpression.java// Allows you to easly try out regular expressions.// {Args: abcabcabcdefabc "abc+" "(abc)+" "(abc){2,}" }import
 java.util.regex.*;
publicclass
 TestRegularExpression {
  publicstaticvoid
 main(String[] args) {
    if
(args.length < 2) {
      System.out.println("Usage:\n"
 +
        "java TestRegularExpression "
 +
        "characterSequence regularExpression+"
);
      System.exit(0);
    }
    System.out.println("Input: \"" + args[0] + "
\""
);
    for
(int
 i = 1; i < args.length; i++) {
      System.out.println(
        "Regular expression: \"" + args[i] + "
\""
);
      Pattern p = Pattern.compile(args[i]);
      Matcher m = p.matcher(args[0]);
      while
(m.find()) {
        System.out.println("Match \"
" + m.group() +
          "\"
 at positions " +
          m.start() + "-"
 + (m.end() - 1));
      }
    }
  }
} ///:~
Java 的正则表达式是由java.util.regex 的Pattern 和Matcher 类实现的。Pattern 对象表示经编译的正则表达式。静态的compile(
 ) 方法负责将表示正则表达式的字符串编译成Pattern 对象。正如上述例程所示的，只要给Pattern 的matcher( ) 方法送一个字符串就能获取一个Matcher 对象。此外，Pattern 还有一个能快速判断能否在input 里面找到regex 的(注意，原文有误，漏了方法名)
> staticboolean
 matches( regex,  input)
以及能返回String 数组的split( ) 方法，它能用regex 把字符串分割开来。
只要给Pattern.matcher( ) 方法传一个字符串就能获得Matcher 对象了。接下来就能用Matcher 的方法来查询匹配的结果了。
> boolean
 matches()
boolean
 lookingAt()
boolean
 find()
boolean
 find(int
 start)
matches( ) 的前提是Pattern 匹配整个字符串，而lookingAt( ) 的意思是Pattern 匹配字符串的开头。
#### find( )
Matcher.find( ) 的功能是发现CharSequence 里的，与pattern相匹配的多个字符序列。例如：
//: c12:FindDemo.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
import
 java.util.*;
publicclass
 FindDemo {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) {
    Matcher m = Pattern.compile("\\w+"
)
      .matcher("Evening is full of the linnet's wings"
);
    while
(m.find())
      System.out.println(m.group());
    int
 i = 0;
    while
(m.find(i)) {
      System.out.print(m.group() + " "
);
      i++;
    }
    monitor.expect(new
 String[] {
      "Evening"
,
      "is"
,
      "full"
,
      "of"
,
      "the"
,
      "linnet"
,
      "s"
,
      "wings"
,
      "Evening vening ening ning ing ng g is is s full "
 +
      "full ull ll l of of f the the he e linnet linnet "
 +
      "innet nnet net et t s s wings wings ings ngs gs s "
    });
  }
} ///:~
"\\w+ "的意思是"一个或多个单词字符"，因此它会将字符串直接分解成单词。find( ) 像一个迭代器，从头到尾扫描一遍字符串。第二个find( ) 是带int 参数的，正如你所看到的，它会告诉方法从哪里开始找——即从参数位置开始查找。
#### Groups
Group是指里用括号括起来的，能被后面的表达式调用的正则表达式。Group 0 表示整个表达式，group 1表示第一个被括起来的group，以此类推。所以；
> A(B(C))D
里面有三个group：group 0是ABCD ， group 1是BC ，group 2是C 。
你可以用下述Matcher 方法来使用group：
public int groupCount( ) 返回matcher对象中的group的数目。不包括group0。
public String group( ) 返回上次匹配操作(比方说find( ) )的group 0(整个匹配)
public String group(int i) 返回上次匹配操作的某个group。如果匹配成功，但是没能找到group，则返回null。
public int start(int group) 返回上次匹配所找到的，group的开始位置。
public int end(int group) 返回上次匹配所找到的，group的结束位置，最后一个字符的下标加一。
下面我们举一些group的例子：
//: c12:Groups.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
publicclass
 Groups {
  privatestatic
 Test monitor = new
 Test();
  staticpublicfinal
 String poem =
    "Twas brillig, and the slithy toves\n"
 +
    "Did gyre and gimble in the wabe.\n"
 +
    "All mimsy were the borogoves,\n"
 +
    "And the mome raths outgrabe.\n\n"
 +
    "Beware the Jabberwock, my son,\n"
 +
    "The jaws that bite, the claws that catch.\n"
 +
    "Beware the Jubjub bird, and shun\n"
 +
    "The frumious Bandersnatch."
;
  publicstaticvoid
 main(String[] args) {
    Matcher m =
      Pattern.compile("(?m)(\\S+)\\s+((\\S+)\\s+(\\S+))$"
)
        .matcher(poem);
    while
(m.find()) {
      for
(int
 j = 0; j <= m.groupCount(); j++)
        System.out.print("["
 + m.group(j) + "]"
);
      System.out.println();
    }
    monitor.expect(new
 String[]{
      "[the slithy toves]"
 +
      "[the][slithy toves][slithy][toves]"
,
      "[in the wabe.][in][the wabe.][the][wabe.]"
,
      "[were the borogoves,]"
 +
      "[were][the borogoves,][the][borogoves,]"
,
      "[mome raths outgrabe.]"
 +
      "[mome][raths outgrabe.][raths][outgrabe.]"
,
      "[Jabberwock, my son,]"
 +
      "[Jabberwock,][my son,][my][son,]"
,
      "[claws that catch.]"
 +
      "[claws][that catch.][that][catch.]"
,
      "[bird, and shun][bird,][and shun][and][shun]"
,
      "[The frumious Bandersnatch.][The]"
 +
      "[frumious Bandersnatch.][frumious][Bandersnatch.]"
    });
  }
} ///:~
这首诗是**Through the Looking Glass**的，Lewis Carroll的"Jabberwocky"的第一部分。可以看到这个正则表达式里有很多用括号括起来的group，它是由任意多个连续的非空字符('\S+ ')和任意多个连续的空格字符('\s+ ')所组成的，其最终目的是要捕获每行的最后三个单词；'$ '表示一行的结尾。但是'$ '通常表示整个字符串的结尾，所以这里要明确地告诉正则表达式注意换行符。这一点是由'(?m) '标志完成的(模式标志会过一会讲解)。
#### start( )和end( )
如果匹配成功，start( ) 会返回此次匹配的开始位置，end( ) 会返回此次匹配的结束位置，即最后一个字符的下标加一。如果之前的匹配不成功(或者没匹配)，那么无论是调用start(
 ) 还是end( ) ，都会引发一个IllegalStateException 。下面这段程序还演示了matches( ) 和lookingAt(
 ) ：
//: c12:StartEnd.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
publicclass
 StartEnd {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) {
    String[] input = new
 String[] {
      "Java has regular expressions in 1.4"
,
      "regular expressions now expressing in Java"
,
      "Java represses oracular expressions"
    };
    Pattern
      p1 = Pattern.compile("re\\w*"
),
      p2 = Pattern.compile("Java.*"
);
    for
(int
 i = 0; i < input.length; i++) {
      System.out.println("input "
 + i + ": "
 + input[i]);
      Matcher
        m1 = p1.matcher(input[i]),
        m2 = p2.matcher(input[i]);
      while
(m1.find())
        System.out.println("m1.find() '"
 + m1.group() +
          "' start = "
+ m1.start() + " end = "
 + m1.end());
      while
(m2.find())
        System.out.println("m2.find() '"
 + m2.group() +
          "' start = "
+ m2.start() + " end = "
 + m2.end());
      if
(m1.lookingAt()) // No reset() necessary
        System.out.println("m1.lookingAt() start = "
          + m1.start() + " end = "
 + m1.end());
      if
(m2.lookingAt())
        System.out.println("m2.lookingAt() start = "
          + m2.start() + " end = "
 + m2.end());
      if
(m1.matches()) // No reset() necessary
        System.out.println("m1.matches() start = "
          + m1.start() + " end = "
 + m1.end());
      if
(m2.matches())
        System.out.println("m2.matches() start = "
          + m2.start() + " end = "
 + m2.end());
    }
    monitor.expect(new
 String[] {
      "input 0: Java has regular expressions in 1.4"
,
      "m1.find() 'regular' start = 9 end = 16"
,
      "m1.find() 'ressions' start = 20 end = 28"
,
      "m2.find() 'Java has regular expressions in 1.4'"
 +
      " start = 0 end = 35"
,
      "m2.lookingAt() start = 0 end = 35"
,
      "m2.matches() start = 0 end = 35"
,
      "input 1: regular expressions now "
 +
      "expressing in Java"
,
      "m1.find() 'regular' start = 0 end = 7"
,
      "m1.find() 'ressions' start = 11 end = 19"
,
      "m1.find() 'ressing' start = 27 end = 34"
,
      "m2.find() 'Java' start = 38 end = 42"
,
      "m1.lookingAt() start = 0 end = 7"
,
      "input 2: Java represses oracular expressions"
,
      "m1.find() 'represses' start = 5 end = 14"
,
      "m1.find() 'ressions' start = 27 end = 35"
,
      "m2.find() 'Java represses oracular expressions' "
 +
      "start = 0 end = 35"
,
      "m2.lookingAt() start = 0 end = 35"
,
      "m2.matches() start = 0 end = 35"
    });
  }
} ///:~
注意，只要字符串里有这个模式，find( ) 就能把它给找出来，但是lookingAt( ) 和matches( ) ，只有在字符串与正则表达式一开始就相匹配的情况下才能返回true 。matches(
 ) 成功的前提是正则表达式与字符串完全匹配，而lookingAt( )[67] 成功的前提是，字符串的开始部分与正则表达式相匹配。
#### 匹配的模式(Pattern flags)
compile( ) 方法还有一个版本，它需要一个控制正则表达式的匹配行为的参数：
> Pattern Pattern.compile(String regex, int
 flag)
flag 的取值范围如下：
|编译标志|效果|
|----|----|
|Pattern.CANON_EQ|当且仅当两个字符的"正规分解(canonical decomposition)"都完全相同的情况下，才认定匹配。比如用了这个标志之后，表达式"a\u030A"会匹配"?"。默认情况下，不考虑"规范相等性(canonical equivalence)"。|
|Pattern.CASE_INSENSITIVE(?i)|默认情况下，大小写不明感的匹配只适用于US-ASCII字符集。这个标志能让表达式忽略大小写进行匹配。要想对Unicode字符进行大小不明感的匹配，只要将UNICODE_CASE 与这个标志合起来就行了。|
|Pattern.COMMENTS(?x)|在这种模式下，匹配时会忽略(正则表达式里的)空格字符(译者注：不是指表达式里的"\\s"，而是指表达式里的空格，tab，回车之类)。注释从#开始，一直到这行结束。可以通过嵌入式的标志来启用Unix行模式。|
|Pattern.DOTALL(?s)|在这种模式下，表达式'.'可以匹配任意字符，包括表示一行的结束符。默认情况下，表达式'.'不匹配行的结束符。|
|Pattern.MULTILINE(?m)|在这种模式下，'^'和'$'分别匹配一行的开始和结束。此外，'^'仍然匹配字符串的开始，'$'也匹配字符串的结束。默认情况下，这两个表达式仅仅匹配字符串的开始和结束。|
|Pattern.UNICODE_CASE(?u)|在这个模式下，如果你还启用了CASE_INSENSITIVE 标志，那么它会对Unicode字符进行大小写不明感的匹配。默认情况下，大小写不明感的匹配只适用于US-ASCII字符集。|
|Pattern.UNIX_LINES(?d)|在这个模式下，只有'\n'才被认作一行的中止，并且与'.'，'^'，以及'$'进行匹配。|
在这些标志里面，Pattern.CASE_INSENSITIVE ，Pattern.MULTILINE ，以及Pattern.COMMENTS 是最有用的(其中Pattern.COMMENTS 还能帮我们把思路理清楚，并且/或者做文档)。注意，你可以用在表达式里插记号的方式来启用绝大多数的模式。这些记号就在上面那张表的各个标志的下面。你希望模式从哪里开始启动，就在哪里插记号。
可以用"OR" ('|')运算符把这些标志合使用：
//: c12:ReFlags.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
publicclass
 ReFlags {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) {
    Pattern p =  Pattern.compile("^java"
,
      Pattern.CASE_INSENSITIVE | Pattern.MULTILINE);
    Matcher m = p.matcher(
      "java has regex\nJava has regex\n"
 +
      "JAVA has pretty good regular expressions\n"
 +
      "Regular expressions are in Java"
);
    while
(m.find())
      System.out.println(m.group());
    monitor.expect(new
 String[] {
      "java"
,
      "Java"
,
      "JAVA"
    });
  }
} ///:~
这样创建出来的正则表达式就能匹配以"java"，"Java"，"JAVA"...开头的字符串了。此外，如果字符串分好几行，那它还会对每一行做匹配(匹配始于字符序列的开始，终于字符序列当中的行结束符)。注意，group( ) 方法仅返回匹配的部分。
### split( )
所谓分割是指将以正则表达式为界，将字符串分割成String 数组。
> String[] split(CharSequence charseq)
String[] split(CharSequence charseq, int
 limit)
这是一种既快又方便地将文本根据一些常见的边界标志分割开来的方法。
//: c12:SplitDemo.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
import
 java.util.*;
publicclass
 SplitDemo {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) {
    String input =
      "This!!unusual use!!of exclamation!!points"
;
    System.out.println(Arrays.asList(
      Pattern.compile("!!"
).split(input)));
    // Only do the first three:
    System.out.println(Arrays.asList(
      Pattern.compile("!!"
).split(input, 3)));
    System.out.println(Arrays.asList(
      "Aha! String has a split() built in!"
.split(" "
)));
    monitor.expect(new
 String[] {
      "[This, unusual use, of exclamation, points]"
,
      "[This, unusual use, of exclamation!!points]"
,
      "[Aha!, String, has, a, split(), built, in!]"
    });
  }
} ///:~
第二个split( ) 会限定分割的次数。
正则表达式是如此重要，以至于有些功能被加进了String 类，其中包括split( ) (已经看到了)，matches( ) ，replaceFirst(
 ) 以及replaceAll( ) 。这些方法的功能同Pattern 和Matcher 的相同。
### 替换操作
正则表达式在替换文本方面特别在行。下面就是一些方法：
replaceFirst(String replacement) 将字符串里，第一个与模式相匹配的子串替换成replacement 。
replaceAll(String replacement) ，将输入字符串里所有与模式相匹配的子串全部替换成replacement 。
appendReplacement(StringBuffer sbuf, String replacement) 对sbuf 进行逐次替换，而不是像replaceFirst(
 ) 或replaceAll( ) 那样，只替换第一个或全部子串。这是个非常重要的方法，因为它可以调用方法来生成replacement (replaceFirst(
 ) 和replaceAll( ) 只允许用固定的字符串来充当replacement )。有了这个方法，你就可以编程区分group，从而实现更强大的替换功能。
调用完appendReplacement( ) 之后，为了把剩余的字符串拷贝回去，必须调用appendTail(StringBuffer sbuf, String replacement) 。
下面我们来演示一下怎样使用这些替换方法。说明一下，这段程序所处理的字符串是它自己开头部分的注释，是用正则表达式提取出来并加以处理之后再传给替换方法的。
//: c12:TheReplacements.javaimport
 java.util.regex.*;
import
 java.io.*;
import
 com.bruceeckel.util.*;
import
 com.bruceeckel.simpletest.*;
/*! Here's a block of text to use as input to
    the regular expression matcher. Note that we'll
    first extract the block of text by looking for
    the special delimiters, then process the
    extracted block. !*/publicclass
 TheReplacements {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) throws
 Exception {
    String s = TextFile.read("TheReplacements.java"
);
    // Match the specially-commented block of text above:
    Matcher mInput =
      Pattern.compile("/\\*!(.*)!\\*/"
, Pattern.DOTALL)
        .matcher(s);
    if
(mInput.find())
      s = mInput.group(1); // Captured by parentheses// Replace two or more spaces with a single space:
    s = s.replaceAll(" {2,}"
, " "
);
    // Replace one or more spaces at the beginning of each// line with no spaces. Must enable MULTILINE mode:
    s = s.replaceAll("(?m)^ +"
, ""
);
    System.out.println(s);
    s = s.replaceFirst("[aeiou]"
, "(VOWEL1)"
);
    StringBuffer sbuf = new
 StringBuffer();
    Pattern p = Pattern.compile("[aeiou]"
);
    Matcher m = p.matcher(s);
    // Process the find information as you// perform the replacements:while
(m.find())
      m.appendReplacement(sbuf, m.group().toUpperCase());
    // Put in the remainder of the text:
    m.appendTail(sbuf);
    System.out.println(sbuf);
    monitor.expect(new
 String[]{
      "Here's a block of text to use as input to"
,
      "the regular expression matcher. Note that we'll"
,
      "first extract the block of text by looking for"
,
      "the special delimiters, then process the"
,
      "extracted block. "
,
      "H(VOWEL1)rE's A blOck Of tExt tO UsE As InpUt tO"
,
      "thE rEgUlAr ExprEssIOn mAtchEr. NOtE thAt wE'll"
,
      "fIrst ExtrAct thE blOck Of tExt by lOOkIng fOr"
,
      "thE spEcIAl dElImItErs, thEn prOcEss thE"
,
      "ExtrActEd blOck. "
    });
  }
} ///:~
我们用前面介绍的TextFile.read( ) 方法来打开和读取文件。mInput 的功能是匹配'/*! ' 和 '!*/ '
 之间的文本(注意一下分组用的括号)。接下来，我们将所有两个以上的连续空格全都替换成一个，并且将各行开头的空格全都去掉(为了让这个正则表达式能对所有的行，而不仅仅是第一行起作用，必须启用多行模式)。这两个操作都用了String 的replaceAll( ) (这里用它更方便)。注意，由于每个替换只做一次，因此除了预编译Pattern 之外，程序没有额外的开销。
replaceFirst( ) 只替换第一个子串。此外，replaceFirst( ) 和replaceAll( ) 只能用常量(literal)来替换，所以如果你每次替换的时候还要进行一些操作的话，它们是无能为力的。碰到这种情况，你得用appendReplacement(
 ) ，它能让你在进行替换的时候想写多少代码就写多少。在上面那段程序里，创建sbuf 的过程就是选group做处理，也就是用正则表达式把元音字母找出来，然后换成大写的过程。通常你得在完成全部的替换之后才调用appendTail( ) ，但是如果要模仿replaceFirst(
 ) (或"replace n")的效果，你也可以只替换一次就调用appendTail( ) 。它会把剩下的东西全都放进sbuf 。
你还可以在appendReplacement( ) 的replacement 参数里用"$g"引用已捕获的group，其中'g' 表示group的号码。不过这是为一些比较简单的操作准备的，因而其效果无法与上述程序相比。
### reset( )
此外，还可以用reset( ) 方法给现有的Matcher 对象配上个新的CharSequence 。
//: c12:Resetting.javaimport
 java.util.regex.*;
import
 java.io.*;
import
 com.bruceeckel.simpletest.*;
publicclass
 Resetting {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) throws
 Exception {
    Matcher m = Pattern.compile("[frb][aiu][gx]"
)
      .matcher("fix the rug with bags"
);
    while
(m.find())
      System.out.println(m.group());
    m.reset("fix the rig with rags"
);
    while
(m.find())
      System.out.println(m.group());
    monitor.expect(new
 String[]{
      "fix"
,
      "rug"
,
      "bag"
,
      "fix"
,
      "rig"
,
      "rag"
    });
  }
} ///:~
如果不给参数，reset( ) 会把Matcher 设到当前字符串的开始处。
### 正则表达式与Java I/O
到目前为止，你看到的都是用正则表达式处理静态字符串的例子。下面我们来演示一下怎样用正则表达式扫描文件并且找出匹配的字符串。受Unix的grep启发，我写了个JGrep.java ，它需要两个参数：文件名，以及匹配字符串用的正则表达式。它会把匹配这个正则表达式那部分内容及其所属行的行号打印出来。
//: c12:JGrep.java// A very simple version of the "grep" program.// {Args: JGrep.java "\\b[Ssct]\\w+"}import
 java.io.*;
import
 java.util.regex.*;
import
 java.util.*;
import
 com.bruceeckel.util.*;
publicclass
 JGrep {
  publicstaticvoid
 main(String[] args) throws
 Exception {
    if
(args.length < 2) {
      System.out.println("Usage: java JGrep file regex"
);
      System.exit(0);
    }
    Pattern p = Pattern.compile(args[1]);
    // Iterate through the lines of the input file:
    ListIterator it = new
 TextFile(args[0]).listIterator();
    while
(it.hasNext()) {
      Matcher m = p.matcher((String)it.next());
      while
(m.find())
        System.out.println(it.nextIndex() + ": "
 +
          m.group() + ": "
 + m.start());
    }
  }
} ///:~
文件是用TextFile 打开的(本章的前半部分讲的)。由于TextFile 会把文件的各行放在ArrayList 里面，而我们又提取了一个ListIterator ，因此我们可以在文件的各行当中自由移动(既能向前也可以向后)。
每行都会有一个Matcher ，然后用find( ) 扫描。注意，我们用ListIterator.nextIndex( ) 跟踪行号。
测试参数是JGrep.java 和以[Ssct] 开头的单词。
### 还需要StringTokenizer吗?
看到正则表达式能提供这么强大的功能，你可能会怀疑，是不是还需要原先的StringTokenizer 。JDK 1.4以前，要想分割字符串，只有用StringTokenizer 。但现在，有了正则表达式之后，它就能做得更干净利索了。
//: c12:ReplacingStringTokenizer.javaimport
 java.util.regex.*;
import
 com.bruceeckel.simpletest.*;
import
 java.util.*;
publicclass
 ReplacingStringTokenizer {
  privatestatic
 Test monitor = new
 Test();
  publicstaticvoid
 main(String[] args) {
    String input = "But I'm not dead yet! I feel happy!"
;
    StringTokenizer stoke = new
 StringTokenizer(input);
    while
(stoke.hasMoreElements())
      System.out.println(stoke.nextToken());
    System.out.println(Arrays.asList(input.split(" "
)));
    monitor.expect(new
 String[] {
      "But"
,
      "I'm"
,
      "not"
,
      "dead"
,
      "yet!"
,
      "I"
,
      "feel"
,
      "happy!"
,
      "[But, I'm, not, dead, yet!, I, feel, happy!]"
    });
  }
} ///:~
有了正则表达式，你就能用更复杂的模式将字符串分割开来——要是交给StringTokenizer 的话，事情会麻烦得多。我可以很有把握地说，正则表达式可以取代StringTokenizer 。
要想进一步学习正则表达式，建议你看**Mastering Regular Expression, 2nd Edition**，作者Jeffrey E. F. Friedl (O'Reilly, 2002)。
## 总结
Java的I/O流类库应该能满足你的基本需求：你可以用它来读写控制台，文件，内存，甚至是Internet。你还可以利用继承来创建新的输入和输出类型。你甚至可以利用Java会自动调用对象的toString( )方法的特点(Java仅有的"自动类型转换")，通过重新定义这个方法，来对要传给流的对象做一个简单的扩展。
但是Java的I/O流类库及其文档还是留下了一些缺憾。比方说你打开一个文件往里面写东西，但是这个文件已经有了，这么做会把原先的内容给覆盖了 。这时要是能有一个异常就好了——有些编程语言能让你规定只能往新建的文件里输出。看来Java是要你用File 对象来判断文件是否存在，因为如果你用FileOutputStream 或FileWriter 的话，文件就会被覆盖了。
我对I/O流类库的评价是比较矛盾的；它确实能干很多事情，而且做到了跨平台。但是如果你不懂decorator模式，就会觉得这种设计太难理解了，所以无论是对老师还是学生，都得多花精力。此外这个类库也不完整，否则我也用不着去写TextFile 了。此外它没有提供格式化输出的功能，而其他语言都已经提供了这种功能。
但是，一旦你真正理解了decorator模式，并且能开始灵活运用这个类库的时候，你就能感受到这种设计的好处了。这时多写几行代码就算不了什么了。
如果你觉得不解渴(本章只是做个介绍，没想要面面俱到)，可以去看Elliotte Rusty Harold 写的**Java I/O** (O'Reilly, 1999)。这本书讲得更深。
