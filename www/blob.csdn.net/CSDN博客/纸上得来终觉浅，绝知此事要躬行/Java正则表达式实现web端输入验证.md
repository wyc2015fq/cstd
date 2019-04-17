# Java正则表达式实现web端输入验证 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年02月18日 22:52:48[boonya](https://me.csdn.net/boonya)阅读数：5585








**1、Java正则表达式基础**
|**                                                                                                 常用的规则符号**| |
|----|----|
|**.                                         **|**表示任意一个字符**|
|**\s **|**空格字符(空格键, tab, 换行, 换页, 回车)**|
|**\S**|**非空格字符([^\s])**|
|**\d**|**一个数字，（相当于[0-9] ）**|
|**\D **|**一个非数字的字符，（相当于[^0-9] ）**|
|**\w **|**一个单词字符(word character) （相当于 [a-zA-Z_0-9] ）**|
|**\W **|**一个非单词的字符，[^\w]**|
|**^ **|**一行的开始**|
|**$ **|**一行的结尾**|
|**\b **|**一个单词的边界**|
|**\B **|**一个非单词的边界**|
|**\G **|**前一个匹配的结束**|
|**[] **|**匹配方括号内的一个字符例如：[abc] 表示字符a，b，c中的任意一个(与a|b|c相同) [a-zA-Z] 表示从a到z或A到Z当中的任意一个字符**|
|*** **|**重复零次或更多次例如：a* 匹配零个或者多个a**|
|**+**|**重复一次或更多次例如：a+ 匹配一个或者多个a**|
|**? **|**重复零次或一次例如：a? 匹配零个或一个a**|
|**{n} **|**重复n次例如：a{4} 匹配4个a**|
|**{n,} **|**重复n次或更多次例如：a{4,} 匹配至少4个a**|
|**{n,m} **|**重复n到m次例如：a{4,10} 匹配4~10个a**|



**2、Java中正则表达式是怎么来完成字符匹配的**

主要的还是涉及到两个类：Pattern.java和Matcher.java

java.util.regex是一个用正则表达式所订制的模式来对字符串进行匹配工作的类库包。 它包括两个类：Pattern和Matcher Pattern 一个Pattern是一个正则表达式经编译后的表现模式。
 Matcher 一个Matcher对象是一个状态机器，它依据Pattern对象做为匹配模式对字符串展开匹配检查。 首先一个Pattern实例订制了一个所用语法与PERL的类似的正则表达式经编译后的模式，然后一个Matcher实例在这个给定的Pattern实例的模式控制下进行字符串的匹配工作。 以下我们就分别来看看这两个类：**（1）.Pattern类:**Pattern的方法如下：

**static Pattern compile(String regex);**将给定的正则表达式编译并赋予给Pattern类

**static Pattern compile(String regex, int flags)**同上，但增加flag参数的指定，

可选的flag参数包括：**CASE INSENSITIVE,MULTILINE,DOTALL,UNICODE CASE， CANON EQ**

**int flags() **      返回当前Pattern的匹配flag参数.

**Matcher matcher(CharSequence input)        **生成一个给定命名的Matcher对象**static boolean matches(String regex, CharSequence input)**编译给定的正则表达式并且对输入的字串以该正则表达式为模开展匹配,该方法适合于该正则表达式只会使用一次的情况，也就是只进行一次匹配工作，因为这种情况下并不需要生成一个Matcher实例。**String pattern()     ** 返回该Patter对象所编译的正则表达式。**String[] split(CharSequence input)**     将目标字符串按照Pattern里所包含的正则表达式为模进行分割。

**String[] split(CharSequence input, int limit) ** 作用同上，增加参数limit目的在于要指定分割的段数，如将limi设为2，那么目标字符串将根据正则表达式分为割为两段。一个正则表达式，也就是一串有特定意义的字符，必须首先要编译成为一个Pattern类的实例，这个Pattern对象将会使用matcher()方法来生成一个Matcher实例，接着便可以使用该
 Matcher实例以编译的正则表达式为基础对目标字符串进行匹配工作，多个Matcher是可以共用一个Pattern对象的。

现在我们先来看一个简单的例子，再通过分析它来了解怎样生成一个Pattern对象并且编译一个正则表达式，最后根据这个正则表达式将目标字符串进行分割：

import java.util.regex.*;

public class Replacement{

  public static void main(String[] args) throws Exception {

    // 生成一个Pattern,同时编译一个正则表达式

    Pattern p = Pattern.compile("[/]+");

    //用Pattern的split()方法把字符串按"/"分割 

    String[] result = p.split( "Kevin has seen《LEON》seveal times,because it is a good film."

          +"/ 凯文已经看过《这个杀手不太冷》几次了，因为它是一部" +"好电影。/名词:凯文。");

    for (int i=0; i<result.length; i++)

            System.out.println(result[i]);

    }

  }

}


输出结果为：
**Kevin has seen《LEON》seveal times,because it is a good film.凯文已经看过《这个杀手不太冷》几次了，因为它是一部好电影。 名词:凯文。 **

很明显，该程序将字符串按"/"进行了分段，我们以下再使用** split(CharSequence input, int limit)**方法来指定分段的段数，程序改动为：

**tring[] result = p.split("Kevin has seen《LEON》seveal times,because it is a good film./ 凯文已经看过《这个杀手不太冷》"**

**+"几次了，因为它是一部好电影。/名词:凯文。"，2);**

这里面的参数"2"表明将目标语句分为两段。

输出结果则为： 
**Kevin has seen《LEON》seveal times,because it is a good film.凯文已经看过《这个杀手不太冷》几次了，因为它是一部好电影。/名词:凯文。 **

由上面的例子，我们可以比较出java.util.regex包在构造Pattern对象以及编译指定的正则表达式的实现手法与我们在上一篇中所介绍的Jakarta-ORO 包在完成同样工作时的差别，Jakarta-ORO 包要先构造一个PatternCompiler类对象接着生成一个Pattern对象，再将正则表达式用该PatternCompiler类的compile()方法来将所需的正则表达式编译赋予

**PatternCompiler   orocom=new Perl5Compiler();Pattern                     pattern=orocom.compile("REGULAR EXPRESSIONS"); PatternMatcher     matcher=new Perl5Matcher(); **

但是在java.util.regex包里，我们仅需生成一个Pattern类，直接使用它的compile()方法就可以达到同样的效果: 
**Pattern p = Pattern.compile("[/]+"); **

因此似乎java.util.regex的构造法比Jakarta-ORO更为简洁并容易理解。

**（2）.Matcher类:**

Matcher方法如下：

**Matcher appendReplacement(StringBuffer sb, String replacement)**

将当前匹配子串替换为指定字符串，并且将替换后的子串以及其之前到上次匹配子串之后的字符串段添加到一个StringBuffer对象里。
**StringBuffer appendTail(StringBuffer sb)     **将最后一次匹配工作后剩余的字符串添加到一个StringBuffer对象里。
**int end()                             **返回当前匹配的子串的最后一个字符在原目标字符串中的索引位置。

int end(int group)             返回与匹配模式里指定的组相匹配的子串最后一个字符的位置。 

boolean find()                   尝试在目标字符串里查找下一个匹配子串。 

boolean find(int start)     重设Matcher对象，并且尝试在目标字符串里从指定的位置开始查找下一个匹配的子串。 

String group()                   返回当前查找而获得的与组匹配的所有子串内容 

String group(int group)   返回当前查找而获得的与指定的组匹配的子串内容 

int groupCount()              返回当前查找所获得的匹配组的数量。 

boolean lookingAt()         检测目标字符串是否以匹配的子串起始。 

boolean matches()         尝试对整个目标字符展开匹配检测，也就是只有整个目标字符串完全匹配时才返回真值。 

Pattern pattern()              返回该Matcher对象的现有匹配模式，也就是对应的Pattern 对象。 

String replaceAll(String replacement)        将目标字符串里与既有模式相匹配的子串全部替换为指定的字符串。 

String replaceFirst(String replacement)    将目标字符串里第一个与既有模式相匹配的子串替换为指定的字符串。 

Matcher reset()                重设该Matcher对象。 

Matcher reset(CharSequence input)          重设该Matcher对象并且指定一个新的目标字符串。 

int start()                          返回当前查找所获子串的开始字符在原目标字符串中的位置。 

int start(int group)          返回当前查找所获得的和指定组匹配的子串的第一个字符在原目标字符串中的位置。 

一个Matcher实例是被用来对目标字符串进行基于既有模式（也就是一个给定的Pattern所编译的正则表达式）进行匹配查找的，所有往Matcher的输入都是通过CharSequence接口提供的，这样做的目的在于可以支持对从多元化的数据源所提供的数据进行匹配工作。

我们分别来看看各方法的使用： 

·               **matches()/lookingAt ()/find()：**

一个Matcher对象是由一个Pattern对象调用其matcher()方法而生成的，一旦该Matcher对象生成,它就可以进行三种不同的匹配查找操作：

matches()        尝试对整个目标字符展开匹配检测，也就是只有整个目标字符串完全匹配时才返回真值。 

lookingAt ()      将检测目标字符串是否以匹配的子串起始。 

find()                 尝试在目标字符串里查找下一个匹配子串。 

以上三个方法都将返回一个布尔值来表明成功与否。 

·               **replaceAll ()/appendReplacement()/appendTail()：**

Matcher类同时提供了四个将匹配子串替换成指定字符串的方法：
**replaceAll()replaceFirst() appendReplacement() appendTail() **

replaceAll()与replaceFirst()的用法都比较简单，请看上面方法的解释。

我们主要重点了解一下appendReplacement()和appendTail()方法。

**appendReplacement(StringBuffer sb, String replacement) **  将当前匹配子串替换为指定字符串，并且将替换后的子串以及其之前到上次匹配子串之后的字符串段添加到一个StringBuffer对象里，而appendTail(StringBuffer
 sb) 方法则将最后一次匹配工作后剩余的字符串添加到一个StringBuffer对象里。

例如，有字符串fatcatfatcatfat,假设既有正则表达式模式为"cat"，第一次匹配后调用appendReplacement(sb,"dog"),那么这时StringBuffer sb的内容为fatdog，也就是fatcat中的cat被替换为dog并且与匹配子串前的内容加到sb里，而第二次匹配后调用appendReplacement(sb,"dog")，那么sb的内容就变为fatdogfatdog，如果最后再调用一次appendTail（sb）,那么sb最终的内容将是fatdogfatdogfat。

还是有点模糊？那么我们来看个简单的程序： 


```java
//该例将把句子里的"Kelvin"改为"Kevin" 
import java.util.regex.*; 
public class MatcherTest{ 
    public static void main(String[] args) throws Exception {
      //生成Pattern对象并且编译一个简单的正则表达式"Kelvin"
      Pattern p = Pattern.compile("Kevin");
      //用Pattern类的matcher()方法生成一个Matcher对象
       Matcher m = p.matcher("Kelvin Li and Kelvin Chan are both working in Kelvin Chen's KelvinSoftShop company");
      StringBuffer sb = new StringBuffer();
      int i=0; 
      //使用find()方法查找第一个匹配的对象 
       boolean result = m.find(); 
      //使用循环将句子里所有的kelvin找出并替换再将内容加到sb里
      while(result) { 
          i++; 
          m.appendReplacement(sb, "Kevin");
          System.out.println("第"+i+"次匹配后sb的内容是："+sb);
          //继续查找下一个匹配对象 
           result = m.find(); 
      } 
     //最后调用appendTail()方法将最后一次匹配后的剩余字符串加到sb里；
      m.appendTail(sb); 
     System.out.println("调用m.appendTail(sb)后sb的最终内容是:"+ sb.toString());
   } 
} 



**最终输出结果为： 第1次匹配后sb的内容是：Kevin 第2次匹配后sb的内容是：Kevin Li and Kevin 第3次匹配后sb的内容是：Kevin Li and Kevin Chan are both working in Kevin 第4次匹配后sb的内容是：Kevin Li and Kevin Chan are both working in Kevin Chen's Kevin 调用m.appendTail(sb)后sb的最终内容是：Kevin Li and Kevin Chan are both working in Kevin Chen's KevinSoftShop company.**
看了上面这个例程是否对appendReplacement()，appendTail()两个方法的使用更清楚呢，如果还是不太肯定最好自己动手写几行代码测试一下。
```

·               **group()/group(int group)/groupCount()：**

该系列方法与我们在上篇介绍的Jakarta-ORO中的MatchResult .group()方法类似(有关Jakarta-ORO请参考上篇的内容)，都是要返回与组匹配的子串内容，下面代码将很好解释其用法：


```java
import java.util.regex.*; 
public class GroupTest{ 
   public static void main(String[] args) throws Exception {
      Pattern p = Pattern.compile("(ca)(t)");
      Matcher m = p.matcher("one cat,two cats in the yard");
      StringBuffer sb = new StringBuffer();
      boolean result = m.find(); 
      System.out.println("该次查找获得匹配组的数量为："+m.groupCount());
      for(int i=1;i<=m.groupCount();i++）{
         System.out.println("第"+i+"组的子串内容为："+m.group(i));
      }
  } 
} 


输出为： 
该次查找获得匹配组的数量为：2 
第1组的子串内容为：ca 
第2组的子串内容为：t
```

Matcher对象的其他方法因比较好理解且由于篇幅有限，请读者自己编程验证。

**3、Java正则表达式web输入验证示例**

```java
package com.boonya.expression;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
/**
 * 文件：ExpresssoinValidateUtil.java
 * 描述：使用正则表达式验证web端输入工具类
 * 注释：如果用过HTML5的就知道 常用的  输入验证已经不需  要自己编写方法验证了，HTML5 的类库已经帮我们做了
 * JDK： 1.6.x或1.7.x(这里使用的是JDK1.6.39)
 * @author PengJunlin
 * @date   2013-02-18 
 */
public class ExpresssoinValidateUtil {        
        private static Pattern pattern=null;
        private static Matcher macher=null;

        /*----------常用输入验证------*/
        //匹配双字节字符(包括汉字在内)：[^x00-xff]             ---已验证
         public static boolean isDoubleByteString(String inputString){
               pattern=Pattern.compile("[^x00-xff]");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配HTML标记的正则表达式：/< (.*)>.*|< (.*) />/      ---未验证：可以实现HTML过滤
         public static boolean isHtmlString(String inputString){
               pattern=Pattern.compile("/< (.*)>.*|< (.*) />/");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配首尾空格的正则表达式：[\\s*)]+\\w+[\\s*$]         ---已验证
         public static boolean isTrimStartAndEndInthisString(String inputString){
               pattern=Pattern.compile("[\\s*)]+\\w+[\\s*$]");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //邮箱规则：用户名@服务器名.后缀                                   ---已验证
         //匹配Email地址的正则表达式：^([a-z0-9A-Z]?)+[a-z0-9A-Z]@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-zA-Z]{2,}
        public static boolean isEmail(String inputString){
               pattern=Pattern.compile("^([a-z0-9A-Z]?)+[a-z0-9A-Z]@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-zA-Z]{2,}");
               macher=pattern.matcher(inputString);
               return macher.find();

        }

        //匹配网址URL的正则表达式：^http://[a-zA-Z0-9./\\s]      ---已验证
         public static boolean isUrl(String inputString){
               pattern=Pattern.compile("^http://[a-zA-Z0-9./\\s]");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //验证用户密码:“^[a-zA-Z]\\w{5,17}$”
         //正确格式为：以字母开头，长度在6-18                      --已验证
         public static boolean isPassword(String inputString){
               pattern=Pattern.compile("^[a-zA-Z]\\w{5,17}$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //验证身份证是否有效15位或18位  ^\\d{15}(\\d{2}[0-9xX])?$  ---已验证<包括对年月日的合法性进行验证>
        public static boolean isIdCard(String inputString){
               pattern=Pattern.compile("^\\d{15}(\\d{2}[0-9xX])?$");
               macher=pattern.matcher(inputString);
               if(macher.find()){                                 //对年月日字符串的验证
                           String power=inputString.substring(inputString.length()-12,inputString.length()-4);
                       pattern=Pattern.compile("^[1-2]+([0-9]{3})+(0[1-9][0-2][0-9]|0[1-9]3[0-1]|1[0-2][0-3][0-1]|1[0-2][0-2][0-9])");
                       macher=pattern.matcher(power);
               }
               return macher.find();
        }

        //验证固定电话号码   ^(([0-9]{3,4})|([0-9]{3,4})-)?[0-9]{7,8}$ ---已验证
         public static boolean isTelePhone(String inputString){
               pattern=Pattern.compile("^(([0-9]{3,4})|([0-9]{3,4})-)?[0-9]{7,8}$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //验证移动电话号码   ^[1][3-8]+\\d{9}                     ---已验证
         public static boolean isMobilePhone(String inputString){
               pattern=Pattern.compile("^[1][3-8]+\\d{9}");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入汉字，匹配中文字符的正则表达式：^[\u4e00-\u9fa5]*$  ---已验证
         public static boolean isChineseString(String inputString){
               pattern=Pattern.compile("^[\u4e00-\u9fa5]*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        
        /*--------- 数字操作验证---对于使用过正则表达式的人而言，下面的就太简单了故不再测试--*/
        

        //匹配正整数 ^[1-9]d*$　 　 
         public static boolean isPositiveInteger(String inputString){
               pattern=Pattern.compile("^[1-9]d*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配负整数 ^-[1-9]d*$ 　 
         public static boolean isNegativeInteger(String inputString){
               pattern=Pattern.compile("^-[1-9]d*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配整数  ^-?[1-9]d*$　　 
         public static boolean isInteger(String inputString){
               pattern=Pattern.compile("^-?[1-9]d*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配非负整数（正整数 + 0） ^[1-9]d*|0$　
         public static boolean isNotNegativeInteger(String inputString){
               pattern=Pattern.compile("^[1-9]d*|0$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配非正整数（负整数 + 0） ^-[1-9]d*|0$　
         public static boolean isNotPositiveInteger(String inputString){
               pattern=Pattern.compile("^-[1-9]d*|0$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配正浮点数    ^[1-9]d*.d*|0.d*[1-9]d*$　　
         public static boolean isPositiveFloat(String inputString){
               pattern=Pattern.compile("^[1-9]d*.d*|0.d*[1-9]d*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配负浮点数    ^-([1-9]d*.d*|0.d*[1-9]d*)$　
         public static boolean isNegativeFloat(String inputString){
               pattern=Pattern.compile("^-([1-9]d*.d*|0.d*[1-9]d*)$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配浮点数   ^-?([1-9]d*.d*|0.d*[1-9]d*|0?.0+|0)$　
         public static boolean isFloat(String inputString){
               pattern=Pattern.compile("^-?([1-9]d*.d*|0.d*[1-9]d*|0?.0+|0)$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配非负浮点数（正浮点数 + 0）^[1-9]d*.d*|0.d*[1-9]d*|0?.0+|0$　　
         public static boolean isNotNegativeFloat(String inputString){
               pattern=Pattern.compile("^[1-9]d*.d*|0.d*[1-9]d*|0?.0+|0$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //匹配非正浮点数（负浮点数 + 0）^(-([1-9]d*.d*|0.d*[1-9]d*))|0?.0+|0$
        public static boolean isNotPositiveFloat(String inputString){
               pattern=Pattern.compile("^(-([1-9]d*.d*|0.d*[1-9]d*))|0?.0+|0$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入数字：“^[0-9]*$”
         public static boolean isNumber(String inputString){
               pattern=Pattern.compile("^[0-9]*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入n位的数字：“^d{n}$”
         public static boolean isNumberFormatLength(int length,String inputString){
               pattern=Pattern.compile("^d{"+length+"}$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入至少n位数字：“^d{n,}$”
         public static boolean isNumberLengthLess(int length,String inputString){
               pattern=Pattern.compile("^d{"+length+",}$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入m-n位的数字：“^d{m,n}$”
         public static boolean isNumberLengthBetweenLowerAndUpper(int lower,int upper,String inputString){
               pattern=Pattern.compile("^d{"+lower+","+upper+"}$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入零和非零开头的数字：“^(0|[1-9][0-9]*)$”
         public static boolean isNumberStartWithZeroOrNot(String inputString){
              pattern=Pattern.compile("^(0|[1-9][0-9]*)$");
              macher=pattern.matcher(inputString);
              return macher.find();
        }

        //只能输入有两位小数的正实数：“^[0-9]+(.[0-9]{2})?$”
         public static boolean isNumberInPositiveWhichHasTwolengthAfterPoint(String inputString){
               pattern=Pattern.compile("^[0-9]+(.[0-9]{2})?$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入有1-3位小数的正实数：“^[0-9]+(.[0-9]{1,3})?$”
         public static boolean isNumberInPositiveWhichHasOneToThreelengthAfterPoint(String inputString){
               pattern=Pattern.compile("^[0-9]+(.[0-9]{1,3})?$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入非零的正整数：“^+?[1-9][0-9]*$”
         public static boolean isIntegerUpZero(String inputString){
               pattern=Pattern.compile("^+?[1-9][0-9]*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入非零的负整数：“^-[1-9][0-9]*$”
         public static boolean isIntegerBlowZero(String inputString){
               pattern=Pattern.compile("^-[1-9][0-9]*$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入由26个英文字母组成的字符串：“^[A-Za-z]+$”
         public static boolean isEnglishAlphabetString(String inputString){
               pattern=Pattern.compile("^[A-Za-z]+$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入由26个大写英文字母组成的字符串：“^[A-Z]+$”
         public static boolean isUppercaseEnglishAlphabetString(String inputString){
               pattern=Pattern.compile("^[A-Z]+$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入由26个小写英文字母组成的字符串：“^[a-z]+$”
         public static boolean isLowerEnglishAlphabetString(String inputString){
               pattern=Pattern.compile("^[a-z]+$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入由数字和26个英文字母组成的字符串：“^[A-Za-z0-9]+$”
         public static boolean isNumberEnglishAlphabetString(String inputString){
               pattern=Pattern.compile("^[A-Za-z0-9]+$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

        //只能输入由数字、26个英文字母或者下划线组成的字符串：“^w+$”
         public static boolean isNumberEnglishAlphabetWithUnderlineString(String inputString){
               pattern=Pattern.compile("^w+$");
               macher=pattern.matcher(inputString);
               return macher.find();
        }

}

class ExpressionTest{
        public static void main(String[] args) {
               Boolean print=false;
//             print=ExpresssoinValidateUtil.isNumberEnglishAlphabetString("*gusdfu423Zsdfs");
//             print=ExpresssoinValidateUtil.isDoubleByteString("a");//"中"(一个汉字占两个字节)
//             print=ExpresssoinValidateUtil.isIdCard("51012119901031996X");
//             print=ExpresssoinValidateUtil.isEmail("my1000@gmail.com");
//             print=ExpresssoinValidateUtil.isUrl("http://12306ng.org/mybbs/83434.html");
//             print=ExpresssoinValidateUtil.isTelePhone("028-85279999");
//             print=ExpresssoinValidateUtil.isMobilePhone("15928057099");
               print=ExpresssoinValidateUtil.isChineseString("china");
//             print=ExpresssoinValidateUtil.isPassword("jidf68");
//             print=ExpresssoinValidateUtil.isTrimStartAndEndInthisString(" jiksd ");
//             print=ExpresssoinValidateUtil.isHtmlString("<html><head><title></title></head><body>helloworld</body></html>");
               System.out.println(print);
        }

}
```





