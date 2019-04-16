# Java学习笔记35：Java常用字符串操作函数 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月03日 14:11:29[initphp](https://me.csdn.net/initphp)阅读数：2125
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









```java
package com.xxx.controller.api;

import org.apache.commons.lang.StringUtils;



public class Test {
    
    public static void main(String[] args) {
        String str = "Hello World";
        
        /* 截取字符串 substring*/
        String string1 = str.substring(5, 10); 
        System.out.println(string1);
        
        /* 连接字符串 concat*/
        String concatString = "Yes ";
        String string2 = concatString.concat(str);
        System.out.println(string2);
        
        /* 替换字符串 replace*/
        String string3 = str.replace("Hello", "123");
        System.out.println(string3);
        
        /* 去除首尾空格 trim*/
        String string4 = " TRIM ".trim();
        System.out.println(string4);
        
        /* 转换为小写  toLowerCase*/
        String string5 = "ASODOIASODI".toLowerCase();
        System.out.println(string5);
        
        /* 转换为大写  toUpperCase*/
        String string6 = "asdasdasd".toUpperCase();
        System.out.println(string6);
        
        /* 字符串长度 length*/
        int len = "asdasdasd".length();
        System.out.println(len);
        
        /* 截取一个字符 charAt */
        char ch = "asdasdasd".charAt(6);
        System.out.println(ch);
        
        /* bytes */
        String string7 = "Hello!你好!";
        byte[] bytes = string7.getBytes();
        System.out.println(bytes);
        
        /* 字符串转字符数组 */
        String string8 = "Hello!你好!";
        char[] chs = string8.toCharArray();
        System.out.println(chs.toString());
        
        /* 字符串比较函数 */
        if (!str.equals("1111")) {
            System.out.println("字符串不一样"); 
        }
        
        /* 是否以特定字符开始 */
        if (str.startsWith("Hello")) {
            System.out.println("以Hello开头"); 
        }
        
        /* 是否以特定字符结尾 */
        if (str.endsWith("World")) {
            System.out.println("以World结尾"); 
        }
        
        /* 查找字符出现位置 */
        int ddd = str.indexOf("W");
        System.out.println(ddd); 
        
        /* 检查字符串是否为空 null "" " " */
        if (StringUtils.isBlank(str)) {
            System.out.println("空"); 
        }
        
        /* 检查字符串不为空 null "" " " */
        if (StringUtils.isNotBlank(str)) {
            System.out.println("空"); 
        }
        
        /* 检查字符串是否为空 null 和 “” */
        if (StringUtils.isEmpty(str)) {
            System.out.println("空"); 
        }
        
        /* 检查字符串不为空 null 和 “” */
        if (StringUtils.isNotEmpty(str)) {
            System.out.println("空"); 
        }
        
    }
}
```




