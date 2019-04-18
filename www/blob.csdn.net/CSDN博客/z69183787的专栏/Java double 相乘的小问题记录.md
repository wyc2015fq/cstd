# Java double 相乘的小问题记录 - z69183787的专栏 - CSDN博客
2016年05月19日 15:58:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1581
看下面的一段代码的运行结果:
**[java]**[view
 plain](http://blog.csdn.net/aikongmeng/article/details/26618721#)[copy](http://blog.csdn.net/aikongmeng/article/details/26618721#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/358919/fork)
- publicclass TestDouble {  
- publicstaticvoid main(String[] args) {  
- double d =538.8;     
-      System.out.println(d*100);  
- }  
输出结果很意外的不是 53880 而是53879.99999999999
解决方法一:
538.8*100  用*10*10 替换,才能得到我们想要的结果
解决方法二:
**[java]**[view
 plain](http://blog.csdn.net/aikongmeng/article/details/26618721#)[copy](http://blog.csdn.net/aikongmeng/article/details/26618721#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/358919/fork)
- publicclass TestDouble {  
- publicstaticvoid main(String[] args) {  
- double d =538.8;      
-      BigDecimal a1 = new BigDecimal(Double.toString(d));  
-      BigDecimal b1 = new BigDecimal(Double.toString(100));   
-      BigDecimal result = a1.multiply(b1);// 相乘结果
-      System.out.println(result);  
-      BigDecimal one = new BigDecimal("1");  
- double a = result.divide(one,2,BigDecimal.ROUND_HALF_UP).doubleValue();//保留1位数
-      System.out.println(a);  
-     }  
- }  
- 
[Java double 相乘的小问题记录](http://blog.csdn.net/aikongmeng/article/details/26618721)
