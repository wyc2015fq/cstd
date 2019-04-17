# 探究replaceAll replace方法的源码 - westbrook1998的博客 - CSDN博客





2018年02月28日 16:59:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：531








replaceAll

```
public String replaceAll(String regex, String replacement) {
        return Pattern.compile(regex).matcher(this).replaceAll(replacement);
    }
```

传入的参数是一个正则表达式regex和一个替换的字符串replacement 

返回的是Pattern类的compile方法的matcher方法再调用replaceAll方法 

再看Pattern类 

compile
```
public static Pattern compile(String regex) {
        return new Pattern(regex, 0);
    }
```

静态工厂模式，传入一个正则表达式字符串，返回一个Pattern的实例，所以再看Pattern的这个构造方法 

Pattern

```
private Pattern(String p, int f) {
        pattern = p;
        flags = f;

        // to use UNICODE_CASE if UNICODE_CHARACTER_CLASS present
        if ((flags & UNICODE_CHARACTER_CLASS) != 0)
            flags |= UNICODE_CASE;

        // Reset group index count
        capturingGroupCount = 1;
        localCount = 0;

        if (pattern.length() > 0) {
            compile();
        } else {
            root = new Start(lastAccept);
            matchRoot = lastAccept;
        }
    }
```

传入正则表达式p，和一个参数flag？暂时还不知道是什么意思 

看到下面的if判断，出现

```java
public static final int UNICODE_CHARACTER_CLASS = 0x100;
public static final int UNICODE_CASE = 0x40;
transient int capturingGroupCount;
transient int localCount;
private transient volatile boolean compiled = false;
transient Node root;
static Node lastAccept = new LastNode();
transient Node matchRoot;
private transient volatile boolean compiled = false;
```

没错，出现了这么多乱七八糟的变量（/(ㄒoㄒ)/~~） 

一个一个看

```java
public static final int UNICODE_CHARACTER_CLASS = 0x100;
//十进制相当于256  二进制为0000....100000000
if ((flags & UNICODE_CHARACTER_CLASS) != 0)
//由此可知，只有当flags二进制倒数第九位不为1，判断成立？/(ㄒoㄒ)/~~
            flags |= UNICODE_CASE;
public static final int UNICODE_CASE = 0x40;
//十进制相当于64，二进制相当,0000....01000000
//如果判断成立，flags和这个数进行按位或？
```

再看一下这个判断上面的一句话
`// to use UNICODE_CASE if UNICODE_CHARACTER_CLASS present`
大致就是说如果UNICODE_CHARACTER_CLASS存在则使用UNICODE_CASE,所以。。反正传入0的话就不会执行了，暂时跳过，再看下面的

```java
// Reset group index count
        capturingGroupCount = 1;
        //很明显（其实是谷歌翻译）字面意思是捕获组的数量 (⊙o⊙)我就猜难道是指该正则匹配的组数
        localCount = 0;
        //???
     /**
     * The local variable count used by parsing tree. Used by matchers to
     * allocate storage needed to perform a match.
     */
        //解析树使用的局部变量计数。 由匹配者使用
        //分配执行匹配所需的存储空间。
```

重置了索引 

看不下去 未完待续




