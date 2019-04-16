# Java学习笔记25：Java中MD5使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月19日 19:30:21[initphp](https://me.csdn.net/initphp)阅读数：1445
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









```java
import java.security.MessageDigest;


/**
 * MD5工具类
 * @author zhuli 2012-12-19 下午6:22:42
 */
public class Md5Util { 
    
    /**
     * 字符串MD5加密后返回字符串格式
     * @param md5Str
     * @return
     */
    public static String MD5(String md5Str) { 
        byte[] btInput = md5Str.getBytes();
        char hexDigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
                             'a', 'b', 'c', 'd', 'e', 'f' 
                           }; 
        try { 
            MessageDigest mdInst = MessageDigest.getInstance("MD5"); 
            mdInst.update(btInput); 
            byte[] md = mdInst.digest(); 
            int j = md.length; 
            char str[] = new char[j * 2]; 
            int k = 0; 
            for (int i = 0; i < j; i++) 
            { 
                byte byte0 = md[i]; 
                str[k++] = hexDigits[byte0 >>> 4 & 0xf]; 
                str[k++] = hexDigits[byte0 & 0xf]; 
            } 
            return new String(str); 
        } catch (Exception e) { 
            return null; 
        } 
    } 
}
```




一个MD5的工具类，使用包：java.security.MessageDigest; 是二进制的，所以需要进行处理之后，才能传递进去一个字符串，获得到一个字符串




备注：http://propedit.sourceforge.jp/eclipse/updates/ 国际化语言 插件





