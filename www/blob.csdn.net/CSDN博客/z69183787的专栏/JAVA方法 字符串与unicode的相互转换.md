# JAVA方法 字符串与unicode的相互转换 - z69183787的专栏 - CSDN博客
2014年05月13日 21:43:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：61175
unicode编码简而言之就是将每一个字符用16位2进制数标识。但是通常都用4位的16进制数标识。 
例如： 
1)中文字符串"你好"的unicode码为：\u60\u597d; 
2)英文字符串"ab"的unicode码为：\u0061\u0062； 
其中\u是标识unicode码用的，后面的4位16进制数则是对应字符的unicode码。 
unicode码在J2EE项目中应用广泛，java对unicode码提供了很好的支持。例如国际化，则是unicode的经典运用。 
那么unicode的编码规则具体是什么，如何用程序实现？ 
1、unicode编码规则
   unicode码对每一个字符用4位16进制数表示。具体规则是：将一个字符(char)的高8位与低8位分别取出，转化为16进制数， 
   如果转化的16进制数的长度不足2位，则在其后补0，然后将高、低8位转成的16进制字符串拼接起来并在前面补上"\u" 即可。 
2、转码程序
1)字符串转unicode
/** 
* 将字符串转成unicode 
* @param str 待转字符串 
* @return unicode字符串 
*/ 
public String convert(String str) 
{ 
str = (str == null ? "" : str); 
String tmp; 
StringBuffer sb = new StringBuffer(1000); 
char c; 
int i, j; 
sb.setLength(0); 
for (i = 0; i < str.length(); i++) 
{ 
c = str.charAt(i); 
sb.append("\\u"); 
j = (c >>>8); //取出高8位 
tmp = Integer.toHexString(j); 
if (tmp.length() == 1) 
sb.append("0"); 
sb.append(tmp); 
j = (c & 0xFF); //取出低8位 
tmp = Integer.toHexString(j); 
if (tmp.length() == 1) 
sb.append("0"); 
sb.append(tmp); 
} 
return (new String(sb)); 
} 
2)unicode转成字符串，与上述过程反向操作即可
/** 
* 将unicode 字符串 
* @param str 待转字符串 
* @return 普通字符串 
*/ 
public String revert(String str) 
{ 
str = (str == null ? "" : str); 
if (str.indexOf("\\u") == -1)//如果不是unicode码则原样返回 
return str; 
StringBuffer sb = new StringBuffer(1000); 
for (int i = 0; i < str.length() - 6;) 
{ 
String strTemp = str.substring(i, i + 6); 
String value = strTemp.substring(2); 
int c = 0; 
for (int j = 0; j < value.length(); j++) 
{ 
char tempChar = value.charAt(j); 
int t = 0; 
switch (tempChar) 
{ 
case 'a': 
t = 10; 
break; 
case 'b': 
t = 11; 
break; 
case 'c': 
t = 12; 
break; 
case 'd': 
t = 13; 
break; 
case 'e': 
t = 14; 
break; 
case 'f': 
t = 15; 
break; 
default: 
t = tempChar - 48; 
break; 
} 
c += t * ((int) Math.pow(16, (value.length() - j - 1))); 
} 
sb.append((char) c); 
i = i + 6; 
} 
return sb.toString(); 
}
// Method 2 ：
```java
java环境安装后jdk的bin目录有个native2ascii.exe可以实现类似的功能，但是通过java代码也可以实现同样的功能。
字符串转换unicode java方法代码片段：
复制代码 代码如下:
/**
 * 字符串转换unicode
 */
public static String string2Unicode(String string) {
 
    StringBuffer unicode = new StringBuffer();
 
    for (int i = 0; i < string.length(); i++) {
 
        // 取出每一个字符
        char c = string.charAt(i);
 
        // 转换为unicode
        unicode.append("\\u" + Integer.toHexString(c));
    }
 
    return unicode.toString();
}
unicode转换字符串java方法代码片段：
复制代码 代码如下:
/**
 * unicode 转字符串
 */
public static String unicode2String(String unicode) {
 
    StringBuffer string = new StringBuffer();
 
    String[] hex = unicode.split("\\\\u");
 
    for (int i = 1; i < hex.length; i++) {
 
        // 转换出每一个代码点
        int data = Integer.parseInt(hex[i], 16);
 
        // 追加成string
        string.append((char) data);
    }
 
    return string.toString();
}
测试java代码片段：
复制代码 代码如下:
public static void main(String[] args) {
    String test = "最代码网站地址:www.zuidaima.com";
 
    String unicode = string2Unicode(test);
     
    String string = unicode2String(unicode) ;
     
    System.out.println(unicode);
     
    System.out.println(string);
 
}
输出结果：
\u6700\u4ee3\u7801\u7f51\u7ad9\u5730\u5740\u3a\u77\u77\u77\u2e\u7a\u75\u69\u64\u61\u69\u6d\u61\u2e\u63\u6f\u6d
```
