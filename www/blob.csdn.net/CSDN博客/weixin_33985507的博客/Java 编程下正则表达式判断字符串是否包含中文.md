# Java 编程下正则表达式判断字符串是否包含中文 - weixin_33985507的博客 - CSDN博客
2013年07月12日 16:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
```
package cn.sunzn.demo;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class Demo {
    public static void main(String[] args) {
        System.out.println(isContainChinese("中国China"));
    }
    public static boolean isContainChinese(String str) {
        Pattern p = Pattern.compile("[\u4e00-\u9fa5]");
        Matcher m = p.matcher(str);
        if (m.find()) {
            return true;
        }
        return false;
    }
}
```
运行结果如下：
```
true
```
