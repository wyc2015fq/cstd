# 【异常处理】Incorrect string value: '\xF0\x90\x8D\x83...' for column... Emoji表情字符过滤的Java实现 - z69183787的专栏 - CSDN博客
2015年09月15日 12:08:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3781
Emoji表情字符现在在APP已经广泛支持了。但是Mysql的UTF8编码对Emoji字符的支持却不是那么好。所以我们经常会遇到这样的异常：
**[html]**[view
 plain](http://blog.csdn.net/shootyou/article/details/44852639#)[copy](http://blog.csdn.net/shootyou/article/details/44852639#)[print](http://blog.csdn.net/shootyou/article/details/44852639#)[?](http://blog.csdn.net/shootyou/article/details/44852639#)
- Incorrect string value: '\xF0\x90\x8D\x83...' for column  
原因是Mysql里UTF8编码最多只能支持3个字节，而Emoji表情字符使用的UTF8编码，很多都是4个字节，有些甚至是6个字节。
解决的方案有两种：
1.使用utf8mb4的mysql编码来容纳这些字符。
2.过滤掉这些特殊的表情字符。
关于第一种解决方法，请参考：http://segmentfault.com/a/1190000000616820 和 http://info.michael-simons.eu/2013/01/21/java-mysql-and-multi-byte-utf-8-support/
有大量细节需要注意，例如：mysql版本，mysql的配置，mysql connector的版本等等。。
因为我们使用的云数据库，所以我选择了过滤这些特殊字符。其实过滤的方式很简单，直接使用正则表达式匹配编码范围，然后替换就行了。
下面是我的代码。
更多可以参考：http://stackoverflow.com/questions/27820971/why-a-surrogate-java-regexp-finds-hypen-minus
**[java]**[view
 plain](http://blog.csdn.net/shootyou/article/details/44852639#)[copy](http://blog.csdn.net/shootyou/article/details/44852639#)[print](http://blog.csdn.net/shootyou/article/details/44852639#)[?](http://blog.csdn.net/shootyou/article/details/44852639#)
- import org.apache.commons.lang3.StringUtils;  
- 
- publicclass EmojiFilterUtils {  
- 
- /**
-      * 将emoji表情替换成*
-      * 
-      * @param source
-      * @return 过滤后的字符串
-      */
- publicstatic String filterEmoji(String source) {  
- if(StringUtils.isNotBlank(source)){  
- return source.replaceAll("[\\ud800\\udc00-\\udbff\\udfff\\ud800-\\udfff]", "*");
  //不可用
-         }else{  
- return source;  
-         }  
-     }  
- publicstaticvoid main(String[] arg ){  
- try{  
-             String text = "This is a smiley \uD83C\uDFA6 face\uD860\uDD5D \uD860\uDE07 \uD860\uDEE2 \uD863\uDCCA \uD863\uDCCD \uD863\uDCD2 \uD867\uDD98 ";  
-             System.out.println(text);  
-             System.out.println(text.length());  
-             System.out.println(text.replaceAll("[\\ud83c\\udc00-\\ud83c\\udfff]|[\\ud83d\\udc00-\\ud83d\\udfff]|[\\u2600-\\u27ff]", "*"));
  //可用
-             System.out.println(filterEmoji(text));  
-         }catch (Exception ex){  
-             ex.printStackTrace();  
-         }  
-     }  
- }  
- 
