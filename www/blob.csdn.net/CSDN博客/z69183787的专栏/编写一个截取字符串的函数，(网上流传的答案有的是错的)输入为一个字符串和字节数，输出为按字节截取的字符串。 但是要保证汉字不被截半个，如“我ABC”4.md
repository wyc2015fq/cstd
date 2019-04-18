# 编写一个截取字符串的函数，(网上流传的答案有的是错的)输入为一个字符串和字节数，输出为按字节截取的字符串。 但是要保证汉字不被截半个，如“我ABC”4 - z69183787的专栏 - CSDN博客
2013年04月09日 17:05:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2374
java：
```java
public class StringSplit {
	public static void main(String[] args) throws Exception {
		String ss = "a很bc你好";
		System.out.println(splitString(ss, 1));
	}
	public static String splitString(String str, int byteLength)
			throws Exception {
		//如果字符串为空，直接返回
		if(str == null || "".equals(str)) {
			return str;
		}
		//用于统计这个字符串中有几个中文字符
		int wordCount = 0;
		//统一按照gbk编码来得到他的字节数组，因为不同的编码字节数组是不一样的。
		byte[] strBytes = str.getBytes("GBK");
		
		//如果只截取一位，而且第一位是中文字符时的处理
		if (byteLength == 1) {
			if (strBytes[0] < 0) {
				return str.substring(0, 1);
			}
		}
		//字符串中的一个中文会使得wordCount 加两次
//如果你这个字节取出来的是一个汉字也就是两个字节当中的一个的话val的值为负数
		for (int i = 0; i < byteLength; i++) {
			int val = strBytes[i];
			if (val < 0) {
				wordCount++;
			}
		}
		
		//如果传递的这个截取的位数没有截到半个中文上面，那么就按照byteLength - (wordCount / 2个长度进行截取
		if (wordCount % 2 == 0) {
			return str.substring(0, (byteLength - (wordCount / 2)));
		}
		//否则，我们就舍弃多出来的这一位 所以  -1 
		return str.substring(0, (byteLength - (wordCount / 2) - 1));
	}
}
```
java 2：
```java
import java.io.UnsupportedEncodingException;  
  
/** 
 * 题目： 编写一个截取字符串的函数，输入为一个字符串和字节数， 输出为按字节截取的字符串。 但是要保证汉字不被截半个，如“我ABC”4， 
 * 应该截为“我AB”，输入“我ABC汉DEF”，6， 应该输出为“我ABC”而不是“我ABC+汉的半个”。 
 *  
 * 解决方法： 中文是占用2个字节的，英文是占用1一个字节， 所以先确定一种编码满足此条件。 
 * GB2312、GBK、GB18030，CP936以及CNS11643都满足条件。 
 *  
 * @author Eric 
 * @version 1.0 
 *  
 */  
public class CharactersSplit {  
  
 /** 
  *  
  * @param text 目标字符串 
  * @param length 截取长度 
  * @param encode 采用的编码方式 
  * @return 
  * @throws UnsupportedEncodingException 
  */  
 public String substring(String text, int length, String encode)  
   throws UnsupportedEncodingException {  
  if (text == null) {  
   return null;  
  }  
  StringBuilder sb = new StringBuilder();  
  int currentLength = 0;  
  for (char c : text.toCharArray()) {  
   currentLength += String.valueOf(c).getBytes(encode).length;  
   if (currentLength <= length) {  
    sb.append(c);  
   } else {  
    break;  
   }  
  }  
  return sb.toString();  
  
 }  
  
 public void test() throws Exception {  
  String text = "我ABC汉DEF";  
  int length1 = 3;  
  int length2 = 6;  
  String[] encodes = new String[] { "GB2312", "GBK", "GB18030", "CP936",  
    "CNS11643" };  
  for (String encode : encodes) {  
   System.out.println(new StringBuilder().append("用").append(encode)  
     .append("编码截取字符串 -- 【").append(text).append("】").append(  
       length1).append("个字节的结果是【").append(  
       substring(text, length1, encode)).append("】")  
     .toString());  
   System.out.println(new StringBuilder().append("用").append(encode)  
     .append("编码截取字符串 -- 【").append(text).append("】").append(  
       length2).append("个字节的结果是【").append(  
       substring(text, length2, encode)).append("】")  
     .toString());  
  
  }  
 }  
  
 public static void main(String[] args) throws Exception {  
  new CharactersSplit().test();  
 }  
  
}
```
java 3：
```java
package com.itheima;  
  
/** 
 * 10、 编写一个截取字符串的函数，输入为一个字符串和字节数，输出为按字节截取的字符串。 
 * 但是要保证汉字不被截半个，如“我ABC”4，应该截为“我AB”，输入“我ABC汉DEF”，6，应该输出为“我ABC”而不是“我ABC+汉的半个”。 
 *  
 * @author 281167413@qq.com 
 */  
  
public class Test10 {  
  
    public static void main(String[] args) {  
        String srcStr1 = "我ABC";  
        String srcStr2 = "我ABC汉DEF";  
  
        splitString(srcStr1, 4);  
        splitString(srcStr2, 6);  
    }  
  
    public static void splitString(String src, int len) {  
        int byteNum = 0;  
  
        if (null == src) {  
            System.out.println("The source String is null!");  
            return;  
        }  
  
        byteNum = src.length();  
        byte bt[] = src.getBytes(); // 将String转换成byte字节数组  
  
        if (len > byteNum) {  
            len = byteNum;  
        }  
  
        // 判断是否出现了截半，截半的话字节对于的ASC码是小于0的值  
        if (bt[len] < 0) {  
            String subStrx = new String(bt, 0, --len);  
            System.out.println("subStrx==" + subStrx);  
        } else {  
            String subStrx = new String(bt, 0, len);  
            System.out.println("subStrx==" + subStrx);  
        }  
    }  
  
}
```
java 4：
```java
public   class  SubStringWithChina {  
         public   static   void  main(String[] args)  throws  Exception{  
              
            String a  =   " 我ABC汉DEF "  ;  
             int  len  =   6  ;  
            String encoding  =   " UTF-8 "  ;  
              
            System.out.println( getSubString(a, len,encoding) );  
            a  =   " 我ABC " ;  
            len  =   4  ;  
            encoding  =   " gbk "  ;  
            System.out.println( getSubString(a, len,encoding) );  
        }  
          
         /**  
         *  
         *  @param  str 传入字符串 
         *  @param  len 截取字节数 
         *  @param  encoding 编码方式 
         *  @return  返回截取后的字符串 
         *  @throws  Exception 
          */   
         public   static  String getSubString(String str, int  len,String encoding)  throws  Exception{  
             if (str  ==   null   ||  str.length() < 1 )  return   null  ;  
             if (len < 1 )  return   null  ;  
            StringBuffer sb  =   new  StringBuffer();  
             int  all  =   0  ;  
             for ( int  i = 0 ;i < str.length();i ++ ){  
                String tmp  =  str.substring(i,i + 1 );  
                sb.append(tmp);  
                 byte [] tmpB  =  tmp.getBytes(encoding);  
                all  += tmpB.length ;  
                 if (all >= len){  
                     break  ;  
                }  
            }  
             return  sb.toString() ;  
        }  
    }
```
js
```java
```
function isChinese(str){  //判断是不是中文
    var reCh=/[u00-uff]/;
    return !reCh.test(str);
}
function lenStat(target){
    var strlen=0; //初始定义长度为0
	var txtval = $.trim(target.val());
	for(var i=0;i<txtval.length;i++){
	 if(isChinese(txtval.charAt(i))==true){
	  strlen=strlen+2;//中文为2个字符
	 }else{
	  strlen=strlen+1;//英文一个字符
	 }
	}
	strlen=Math.ceil(strlen/2);//中英文相加除2取整数
	return strlen;
}
