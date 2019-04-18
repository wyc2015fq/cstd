# Java精确判断一个字符串是否有中文 - z69183787的专栏 - CSDN博客
2016年11月14日 18:22:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6536
Java判断一个字符串是否有中文是利用Unicode编码来判断，因为中文的编码区间为：0x4e00--0x9fbb，不过通用区间来判断中文也不非常精确，因为有些中文的标点符号利用区间判断会得到错误的结果。而且利用区间判断中文效率也并不高，例如；str.substring(i,
 i + 1).matches("[\\u4e00-\\u9fbb]+")，就需要遍历整个字符串，如果字符串太长效率非常低，而且判断标点还会错误。这里提高一个高效准确的判断方法，方法在下面的代码里：private static final boolean isChinese(char c) 。类已经编译通过，运行可以查看结果。 
Java代码  ![收藏代码](http://javapub.iteye.com/images/icon_star.png)
- package com.zakisoft.ch;  
- 
- publicclass IsChineseOrNot {  
- 
- // GENERAL_PUNCTUATION 判断中文的“号
- // CJK_SYMBOLS_AND_PUNCTUATION 判断中文的。号
- // HALFWIDTH_AND_FULLWIDTH_FORMS 判断中文的，号
- privatestaticfinalboolean isChinese(char c) {  
-         Character.UnicodeBlock ub = Character.UnicodeBlock.of(c);  
- if (ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS  
-                 || ub == Character.UnicodeBlock.CJK_COMPATIBILITY_IDEOGRAPHS  
-                 || ub == Character.UnicodeBlock.CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A  
-                 || ub == Character.UnicodeBlock.GENERAL_PUNCTUATION  
-                 || ub == Character.UnicodeBlock.CJK_SYMBOLS_AND_PUNCTUATION  
-                 || ub == Character.UnicodeBlock.HALFWIDTH_AND_FULLWIDTH_FORMS) {  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- publicstaticfinalboolean isChinese(String strName) {  
- char[] ch = strName.toCharArray();  
- for (int i = 0; i < ch.length; i++) {  
- char c = ch[i];  
- if (isChinese(c)) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(isChinese("き"));  
-         System.out.println(isChinese("test,.?!%^&*(){}[]"));  
-         System.out.println(isChinese("测试"));  
-         System.out.println(isChinese("“测试”，。？！%……&*（）——{}【】”"));  
-     }  
- 
- publicstaticfinalboolean isChineseCharacter(String chineseStr) {  
- char[] charArray = chineseStr.toCharArray();  
- for (int i = 0; i < charArray.length; i++) {  
- if ((charArray[i] >= 0x4e00) && (charArray[i] <= 0x9fbb)) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- 
- /**
-      * @deprecated; 弃用。和方法isChineseCharacter比效率太低。
-      * */
- publicstaticfinalboolean isChineseCharacter_f2() {  
-         String str = "！？";  
- for (int i = 0; i < str.length(); i++) {  
- if (str.substring(i, i + 1).matches("[\\u4e00-\\u9fbb]+")) {  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- }  
System.out.println(isChinese("き")); 
System.out.println(isChinese("test,.?!%^&*(){}[]")); 
System.out.println(isChinese("测试")); 
System.out.println(isChinese("“测试”，。？！%……&*（）——{}【】”")); 
运行结果为： 
false 
false 
true 
true
