# spring工具类AntPathMatcher - z69183787的专栏 - CSDN博客
2014年04月08日 11:40:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16058

```java
package cn.bidlink.wsmp.commons.security;
 
import junit.framework.TestCase;
import org.springframework.util.AntPathMatcher;
import org.springframework.util.PathMatcher;
 
public class AntPathMatcherTest extends TestCase {
    public void testMatch() {
        PathMatcher matcher = new AntPathMatcher();
        // 完全路径url方式路径匹配
         String requestPath="/user/list.htm?username=aaa&departmentid=2&pageNumber=1&pageSize=20";//请求路径
         String patternPath="/user/list.htm**";//路径匹配模式
 
        // 不完整路径uri方式路径匹配
        // String requestPath="/app/pub/login.do";//请求路径
        // String patternPath="/**/login.do";//路径匹配模式
        // 模糊路径方式匹配
        // String requestPath="/app/pub/login.do";//请求路径
        // String patternPath="/**/*.do";//路径匹配模式
        // 包含模糊单字符路径匹配
        //String requestPath = "/app/pub/login.do";// 请求路径
        //String patternPath = "/**/lo?in.do";// 路径匹配模式
        boolean result = matcher.match(patternPath, requestPath);
        assertTrue(result);
    }
}
```
总结如下：
ANT方式的通配符有三种：
    ?（匹配任何单字符），*（匹配0或者任意数量的字符），**（匹配0或者更多的目录）
url路径匹配规则：
|URL路径|说明|
|----|----|
|/app/*.x|匹配(Matches)所有在app路径下的.x文件|
|/app/p?ttern|匹配(Matches) /app/pattern 和 /app/pXttern,但是不包括/app/pttern|
|/**/example|匹配(Matches) /app/example, /app/foo/example, 和 /example|
|/app/**/dir/file.|匹配(Matches) /app/dir/file.jsp, /app/foo/dir/file.html,/app/foo/bar/dir/file.pdf, 和 /app/dir/file.java|
|/**/*.jsp|匹配(Matches)任何的.jsp 文件|
最长匹配原则(has more characters)
说明，URL请求/app/dir/file.jsp，现在存在两个路径匹配模式/**/*.jsp和/app/dir/*.jsp，那么会根据模式/app/dir/*.jsp来匹配
