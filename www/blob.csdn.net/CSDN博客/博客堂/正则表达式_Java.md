# 正则表达式_Java - 博客堂 - CSDN博客





2017年02月25日 20:58:21[最小森林](https://me.csdn.net/u012052268)阅读数：384标签：[java																[正则表达式](https://so.csdn.net/so/search/s.do?q=正则表达式&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)








# 正则表达式


- [正则表达式](#正则表达式)- [正则表达式概述](#正则表达式概述)
- [正则表达式规则常用](#正则表达式规则常用)
- [一般使用方法](#一般使用方法)- [判断功能](#1-判断功能)
- [分割功能](#2-分割功能)
- [替换功能](#3-替换功能)
- [获取功能](#4-获取功能)





## 正则表达式概述

**正则表达式**：是指一个用来描述或者匹配一系列符合某个句法规则的字符串的单个字符串。是一个规则。

所以在Java中，把这个**规则**直接定义为一个String：
`String regex ="[1-9][0-9]{4,14}"`
## 正则表达式规则（常用）

```
A:字符
    x 字符 x。举例：'a'表示字符a
    \\ 反斜线字符。
    \n 新行（换行）符 ('\u000A') 
    \r 回车符 ('\u000D')

B:字符类
    [abc] a、b 或 c（简单类） 
    [^abc] 任何字符，除了 a、b 或 c（否定） 
    [a-zA-Z] a到 z 或 A到 Z，两头的字母包括在内（范围） 
    [0-9] 0到9的字符都包括

C:预定义字符类
    . 任何字符。我的就是.字符本身，怎么表示呢? \.  （实际应用中，写为\\.）
    \d 数字：[0-9]  （实际应用中，写为\\d）
    \w 单词字符：[a-zA-Z_0-9] （实际应用中，写为\\w）
        在正则表达式里面组成单词的东西必须有这些东西组成

D:边界匹配器
    ^ 行的开头 
    $ 行的结尾 
    \b 单词边界
        就是不是单词字符的地方。
        举例：hello world?haha;xixi

E:Greedy 数量词 
    X?  表示：X，一次或一次也没有
    X*  表示：X，零次或多次
    X+  表示：X，一次或多次
    X{n}    表示：X，恰好 n 次 
    X{n,}   表示：X，至少 n 次 
    X{n,m}  表示：X，至少 n 次，但是不超过 m 次
```

## 一般使用方法

### 1. 判断功能

> 
String类的public boolean matches(String regex)


返回的是 boolean

eg1：（检验QQ号码）

```
public class RegexDemo2 {
    public static void main(String[] args) {
        // 创建键盘录入对象
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入你的QQ号码：");
        String qq = sc.nextLine();

        System.out.println("checkQQ:" + checkQQ(qq));
    }

    public static boolean checkQQ(String qq) {
        // String regex ="[1-9][0-9]{4,14}";
        // boolean flag = qq.matches(regex);
        // return flag;

        // 简便写法：
        return qq.matches("[1-9]\\d{4,14}");
    }
}
```

eg2：检验邮箱

```
public class RegexTest {
    public static void main(String[] args) {
        //键盘录入邮箱
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入邮箱：");
        String email = sc.nextLine();

        //定义邮箱的规则
        //String regex = "[a-zA-Z_0-9]+@[a-zA-Z_0-9]{2,6}(\\.[a-zA-Z_0-9]{2,3})+";
        String regex = "\\w+@\\w{2,6}(\\.\\w{2,3})+";

        //调用功能，判断即可
        boolean flag = email.matches(regex);

        //输出结果
        System.out.println("flag:"+flag);
    }
}
```

### 2. 分割功能

> 
String类的public String[] split(String regex)


返回的是以个String数组

```
/*
 * 分割功能练习
 */
public class RegexDemo2 {
    public static void main(String[] args) {
        // 定义一个字符串
        String s1 = "aa,bb,cc";
        // 直接分割
        String[] str1Array = s1.split(",");
        for (int x = 0; x < str1Array.length; x++) {
            System.out.println(str1Array[x]);
        }
        System.out.println("---------------------");

        String s2 = "aa.bb.cc";
        String[] str2Array = s2.split("\\.");
        for (int x = 0; x < str2Array.length; x++) {
            System.out.println(str2Array[x]);
        }
        System.out.println("---------------------");

        String s3 = "aa    bb                cc";
        String[] str3Array = s3.split(" +");
        for (int x = 0; x < str3Array.length; x++) {
            System.out.println(str3Array[x]);
        }
        System.out.println("---------------------");

        //硬盘上的路径，我们应该用\\替代\
        String s4 = "E:\\JavaSE\\day14\\avi";
        String[] str4Array = s4.split("\\\\");
        for (int x = 0; x < str4Array.length; x++) {
            System.out.println(str4Array[x]);
        }
        System.out.println("---------------------");
    }
}
```

### 3. 替换功能

> 
String类的public String replaceAll(String regex,String replacement)


使用给定的 **replacement**替换此字符串所有匹配给定的**regex**。 

```
public class RegexDemo {
    public static void main(String[] args) {
        // 定义一个字符串
        String s = "helloqq12345worldkh622112345678java";

        // 我要去除所有的数字,用*给替换掉
                String regex = "\\d+";
                String ss = "*";
                String result = s.replaceAll(regex, ss);
                System.out.println(result);
    }
}
```

### 4. 获取功能

使用方法：       

```
Pattern和Matcher

    Pattern p = Pattern.compile("a*b");
    Matcher m = p.matcher("aaaaab");

    find():查找存不存在
    group():获取刚才查找过的数据
```

```
public class RegexDemo2 {
    public static void main(String[] args) {
        // 定义字符串
        String s = "da jia ting wo shuo,jin tian yao xia yu,bu shang wan zi xi,gao xing bu?";
        // 规则
        String regex = "\\b\\w{3}\\b";

        // 把规则编译成模式对象
        Pattern p = Pattern.compile(regex);
        // 通过模式对象得到匹配器对象
        Matcher m = p.matcher(s);

        // 调用匹配器对象的功能
        // 通过find方法就是查找有没有满足条件的子串，然后再用group()接收。
        while (m.find()) {
            System.out.println(m.group());
        }

        // 注意：一定要先find()，然后才能group()

    }
}
```




