# 如何获取e.printStackTrace()的内容 - z69183787的专栏 - CSDN博客
2015年06月26日 07:08:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5378
e.printStackTrace()通常是打印在控制台的，但是，有时候程序上线了需要看这个堆栈的内容就不容易了，一来生产环境打印的东西很多或者很少，二来有时候无法直接查看到，这个时候就需要把这些内容记录下来，比如记录到数据库中，下面的方法可以完整记录。
Java代码  ![收藏代码](http://mywork.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {  
- try {  
-             String aa = "";  
-             System.out.println(aa.substring(3));  
- 
-         } catch (Exception e) {  
-             e.printStackTrace();  
-             StringWriter sw = new StringWriter();  
-             e.printStackTrace(new PrintWriter(sw, true));  
-             String str = sw.toString();  
-             System.out.println("==========");  
- 
-             System.out.println(str);  
-         }  
-     }  
打印的效果如下：
Java代码  ![收藏代码](http://mywork.iteye.com/images/icon_star.png)
- java.lang.StringIndexOutOfBoundsException: String index out of range: -3
-     at java.lang.String.substring(Unknown Source)  
-     at java.lang.String.substring(Unknown Source)  
-     at Getc.main(Getc.java:16)  
- ==========  
- java.lang.StringIndexOutOfBoundsException: String index out of range: -3
-     at java.lang.String.substring(Unknown Source)  
-     at java.lang.String.substring(Unknown Source)  
-     at Getc.main(Getc.java:16)  
