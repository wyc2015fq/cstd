# C# 正则表达式 - u014677855的博客 - CSDN博客
2018年09月05日 15:45:07[xiaobigben](https://me.csdn.net/u014677855)阅读数：36
有时候我们会对输入有要求，必须匹配某种模式才可以。正则表达式就是一种匹配输入文本的模式。.Net框架提供了允许这种匹配的正则表达式引擎。模式由一个或多个字符、运算符和结构组成。
## 一、定义正则表达式
正则表达式相关的操作都包括在Regex类中，所以使用正则表达式的时候要先引用命名空间`system.Text.RegularExpressions;`
定义正则表达式的各种类别的字符、运算符和结构有：
- 字符转义
- 字符类
- 定位点
- 分组构造
- 限定符
- 反向引用构造
- 备用构造
- 替换
- 杂项构造
### 1.1 字符转义
|转义字符|描述|模式|匹配|
|----|----|----|----|
|\a|与报警 (bell) 符 \u0007 匹配。|\a|“Warning!” + ‘\u0007’ 中的 “\u0007”|
|\b|在字符类中，与退格键 \u0008 匹配。|[\b]{3,}|“\b\b\b\b” 中的 “\b\b\b\b”|
|\t|与制表符 \u0009 匹配。|(\w+)\t|“Name\tAddr\t” 中的 “Name\t” 和 “Addr\t”|
|\r|与回车符 \u000D 匹配。（\r 与换行符 \n 不是等效的。）|\r\n(\w+)|“\r\Hello\nWorld.” 中的 “\r\nHello”|
|\v|与垂直制表符 \u000B 匹配。|[\v]{2,}|“\v\v\v” 中的 “\v\v\v”|
|\f|与换页符 \u000C 匹配。|[\f]{2,}|“\f\f\f” 中的 “\f\f\f”|
|\n|与换行符 \u000A 匹配。|\r\n(\w+)|“\r\Hello\nWorld.” 中的 “\r\nHello”|
|\e|与转义符 \u001B 匹配。|\e|“\x001B” 中的 “\x001B”|
|\ nnn|使用八进制表示形式指定一个字符（nnn 由二到三位数字组成）。|\w\040\w|“a bc d” 中的 “a b” 和 “c d”|
|\x nn|使用十六进制表示形式指定字符（nn 恰好由两位数字组成）。|\w\x20\w|“a bc d” 中的 “a b” 和 “c d”|
|\c X \c x|匹配 X 或 x 指定的 ASCII 控件字符，其中 X 或 x 是控件字符的字母。|\cC|“\x0003” 中的 “\x0003” (Ctrl-C)|
|\u nnnn|使用十六进制表示形式匹配一个 Unicode 字符（由 nnnn 表示的四位数）。|\w\u0020\w|“a bc d” 中的 “a b” 和 “c d”|
|\|在后面带有不识别的转义字符时，与该字符匹配。|\d+[+-x*]\d+\d+[+-x*\d+|“(2+2) * 3*9” 中的 “2+2” 和 “3*9”|
### 1.2 字符类
字符类与一组字符中的任何一个字符匹配。
|字符类|描述|模式|匹配|
|----|----|----|----|
|character_group|匹配 character_group 中的任何单个字符。 默认情况下，匹配区分大小写。|[mn]|“mat” 中的 “m”，”moon” 中的 “m” 和 “n”|
|[^character_group]|非：与不在 character_group 中的任何单个字符匹配。 默认情况下，character_group 中的字符区分大小写。|[^aei]|“avail” 中的 “v” 和 “l”|
|[first-last]|字符范围：与first到last的任何单个字符匹配|[b-d]|[b-d]irds 可以匹配 Birds、 Cirds、 Dirds|
|\w|与任何单词字符匹配。|\w|“Room#1” 中的 “R”、 “o”、 “m” 和 “1”|
|\W|与任何非单词字符匹配。|\W|“Room#1” 中的 “#”|
|\s|与任何空白字符匹配。|\w\s   \”ID A1.3” 中的 “D “||
|\S|与任何非空白字符匹配。|\s\S   \”int *_ctr” 中的 ” *“||
|\d|与任何十进制数字匹配。|\d|“4 = IV” 中的 “4”|
|\D|匹配不是十进制数的任意字符。|\D|“4 = IV” 中的 ” “、 “=”、 ” “、 “I” 和 “V”|
### 1.3 定位点
|断言|描述|模式|匹配|
|----|----|----|----|
|^|匹配必须从字符串或一行的开头开始。|^\d{3}|“567-777-” 中的 “567”|
|￥|匹配必须出现在字符串的末尾或出现在行或字符串末尾的 \n 之前。|-\d{4}$|“8-12-2012” 中的 “-2012”|
|\A|匹配必须出现在字符串的开头。|\A\w{4}||
|\Z|匹配必须出现在字符串的末尾或出现在字符串末尾的 \n 之前。|-\d{3}\Z|“Bond-901-007” 中的 “-007”|
|\z|匹配必须出现在字符串的末尾。|-\d{3}\z|“-901-333” 中的 “-333”|
|\G|匹配必须出现在上一个匹配结束的地方。|\G(\d)|“(1)(3)(5)[7](9)” 中的 “(1)”、 “(3)” 和 “(5)”|
|\b|匹配一个单词边界，也就是指单词和空格间的位置。|er\b|匹配”never”中的”er”，但不能匹配”verb”中的”er”。|
|\B|匹配非单词边界。|er\B|匹配”verb”中的”er”，但不能匹配”never”中的”er”。|
### 1.4 C# 常用正则表达式
|正则表达式|描述|
|----|----|
|“^\d+$”|非负整数|
|^[0-9]*[1-9][0-9]*$|正整数|
|“^[0-9]*$”|只能输入数字|
|“^\d{n}$”只能输入n位的数字||
|“^\d{n,}$”|只能输入至少n位数字|
|^\d{m,n}$|只能输入m-n位的数字|
只能输入零和非零开头的数字：“^(0|[1-9][0-9]*)$”
只能输入有两位小数的正实数：“^[0-9]+(.[0-9]{2})?$”
只能输入有1-3位小数的正实数：“^[0-9]+(.[0-9]{1,3})?$”
只能输入非零的正整数：“^+?[1-9][0-9]*$”
只能输入非零的负整数：“^-[1-9][0-9]*$”
只能输入长度为3的字符：“^.{3}$”
只能输入由26个英文字母组成的字符串：“^[A-Za-z]+$”
只能输入由26个大写英文字母组成的字符串：“^[A-Z]+$”
只能输入由26个小写英文字母组成的字符串：“^[a-z]+$”
只能输入由数字和26个英文字母组成的字符串：“^[A-Za-z0-9]+$”
只能输入由数字、26个英文字母或者下划线组成的字符串：“^\w+$”
验证用户密码:“^[a-zA-Z]\w{5,17}$”正确格式为：以字母开头，长度在6-18之间，
只能包含字符、数字和下划线。
验证是否含有^%&’,;=?$\"等字符：“[^%&',;=?$\x22]+”
只能输入汉字：“^[\u4e00-\u9fa5],{0,}$”
验证Email地址：“^\w+([-+.]\w+)*@\w+([-.]\w+)*.\w+([-.]\w+)*$”
验证InternetURL：“^[http://([](http://([)\w-]+.)+[\w-]+(/[\w-./?%&=]*)?$”
验证电话号码：“^((\d{3,4})|\d{3,4}-)?\d{7,8}$”
正确格式为：“XXXX-XXXXXXX”，“XXXX-XXXXXXXX”，“XXX-XXXXXXX”，
“XXX-XXXXXXXX”，“XXXXXXX”，“XXXXXXXX”。
验证身份证号（15位或18位数字）：“^\d{15}|\d{}18$”
验证一年的12个月：“^(0?[1-9]|1[0-2])$”正确格式为：“01”-“09”和“1”“12”
验证一个月的31天：“^((0?[1-9])|((1|2)[0-9])|30|31)$”
正确格式为：“01”“09”和“1”“31”。
## 二、Regex类
Regex类用于表示一个正则表达式，Regex类中一些常用的方法如下表：
|方法|描述|
|----|----|
|public bool IsMatch(string Input)|指示 Regex 构造函数中指定的正则表达式是否在指定的输入字符串中找到匹配项。|
|public bool IsMatch(string Input,int start)|指示 Regex 构造函数中指定的正则表达式是否在指定的输入字符串中找到匹配项，从字符串中指定的开始位置开始。|
|public static bool IsMatch( string input, string pattern )|指示指定的正则表达式是否在指定的输入字符串中找到匹配项。|
|public MatchCollection Matches( string input )|在指定的输入字符串中搜索正则表达式的所有匹配项。|
|public string Replace( string input, string replacement )|在指定的输入字符串中，把所有匹配正则表达式模式的所有匹配的字符串替换为指定的替换字符串。|
|public string[] Split( string input )|把输入字符串分割为子字符串数组，根据在 Regex 构造函数中指定的正则表达式模式定义的位置进行分割。|
## 三、实例
### 3.1 输入一个18位纯数字的身份证号
```
static void Main(string[] args)
{
     Console.WriteLine("请输入18位身份证号:");
     string a = Console.ReadLine();
     //只能输入18位数字，d匹配数字，\d等价于[0-9]*$
     while ( !Regex.IsMatch(a,@"^\d{18}$"))
     {
         Console.WriteLine("请输入18位数字。");
         a = Console.ReadLine();
     }
     Console.WriteLine("身份证号：{0}", a);
     Console.ReadKey();
 }
```
### 3.2 输入QQ号：只能是8-13位的数字
```
static void Main(string[] args)
{
    Console.WriteLine("请输入您的QQ账号:");
    string a = Console.ReadLine();
    //只能输入8-13位的数字
    while ( !Regex.IsMatch(a,@"^\d{8,13}$"))
    {
        Console.WriteLine("请输入8-13位数字。");
        a = Console.ReadLine();
    }
    Console.WriteLine("QQ号为：{0}", a);
    Console.ReadKey();
}
```
### 3.3 输入注册编号，编号只能由0或非0开头，纯数字
```
static void Main(string[] args)
{
     Console.WriteLine("请输入您的编号:");
     string a = Console.ReadLine();
     //只能输入0或非0开头的数字
     while ( !Regex.IsMatch(a,@"^(0|[1-9][0-9]*)$"))
     {
         Console.WriteLine("注册编号必须为0或非0开头的数字");
         a = Console.ReadLine();
     }
     Console.WriteLine("注册编号为：{0}", a);
     Console.ReadKey();
 }
```
### 3.4 验证用户密码，只能由26个字母组成。
```
Console.WriteLine("请输入密码，只能由26个英文字母组成：");
 string a = Console.ReadLine();
  while ( !Regex.IsMatch(a,"^[A-Za-z]+$"))
  {
      // while ( !Regex.IsMatch(a,"^[A-Z]+$"))，只能由大写字母组成
      //"^[a-z]+$"只能由小写字母组成
      Console.WriteLine("只能由26英文字母组成,请重新输入：");
      a = Console.ReadLine();
  }
  Console.WriteLine("密码1为：{0}",a);
  Console.ReadKey();
```
### 3.5 验证密码，只能由字母，下划线，数字组成，6-18位，首字母大写字母。
```
static void Main(string[] args)
 {
      Console.WriteLine("请输入密码，字母开头，6-18位，由数字、字母和下划线组成");
      string a = Console.ReadLine();
      while ( !Regex.IsMatch(a,@"^[A-Za-z]\w{5,17}$"))//e
      {
          Console.WriteLine("字母开头，6-18位，由数字、字母和下划线组成");
          a = Console.ReadLine();
      }
      Console.WriteLine("密码为：{0}",a);
      Console.ReadKey();
  }
```
### 3.6 从输入的字符串中查找以“S”开头的字符串
```
static void Main(string[] args)
{
    Console.WriteLine("请输入字符串：");
    string str = "A Thousand Splendid Suns";
    string expr = @"\bS\S*";//S是开头边界字符
    Console.WriteLine("匹配字符串为：" + expr);
    MatchCollection mc = Regex.Matches(str, expr);//找出所有匹配的字符串集合
    foreach(Match m in mc)
    {
        Console.WriteLine(m);
    }
    string a = Console.ReadLine();
    Console.ReadKey();
}
```
