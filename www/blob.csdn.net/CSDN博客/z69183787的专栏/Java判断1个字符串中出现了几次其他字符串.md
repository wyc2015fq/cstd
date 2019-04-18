# Java判断1个字符串中出现了几次其他字符串 - z69183787的专栏 - CSDN博客
2013年12月04日 15:52:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7476
```java
public class Test {
	    public static int count(String text,String sub){
	        int count =0, start =0;
	        while((start=text.indexOf(sub,start))>=0){
	            start += sub.length();
	            count ++;
	        }
	        return count;
	    }
	    public static void main(String[] args){
	        String text ="nihaoksdoksad";
	        String sub ="o k";
	        System.out.println(count(text,sub));
	    }
	}
```
