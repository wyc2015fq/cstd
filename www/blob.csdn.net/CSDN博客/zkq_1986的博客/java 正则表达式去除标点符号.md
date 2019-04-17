# java 正则表达式去除标点符号 - zkq_1986的博客 - CSDN博客





2017年03月10日 11:28:22[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：211







- publicclass Test {  
- 
- 
- publicstaticvoid main(String[] args) {  
-         String str = "!!！？？!!!!%*）%￥！KTV去符号标号！！当然,，。!!..**半角";  
-         System.out.println(str);  
-         String str1 = str.replaceAll("[\\pP\\p{Punct}]", "");  
-         System.out.println("str1:" + str1);  
- 
- 
-         String str2 = str.replaceAll("[//pP]", "");  
-         System.out.println("str2:" + str2);  
- 
- 
-         String str3 = str.replaceAll("[//p{P}]", "");  
-         System.out.println("str3:" + str3);  
-     }  
- }  



