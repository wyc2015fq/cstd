# HttpClient 发送Post,Get请求例子（包含设置请求头信息和获取返回头信息） - z69183787的专栏 - CSDN博客
2015年01月21日 12:51:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：37524
```java
package com.test.action;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import org.apache.commons.httpclient.DefaultHttpMethodRetryHandler;
import org.apache.commons.httpclient.Header;
import org.apache.commons.httpclient.HttpClient;
import org.apache.commons.httpclient.HttpException;
import org.apache.commons.httpclient.HttpStatus;
import org.apache.commons.httpclient.methods.PostMethod;
import org.apache.commons.httpclient.params.HttpMethodParams;
/**
*
 * 依赖 commons-httpclient-3.1.jar commons-codec-1.4.jar 
 * @author tianjun
*
*/
public class PostTest
{
public static void main(String[] args)
{
// （1）构造HttpClient的实例
HttpClient httpClient = new HttpClient();
// （2）创建POST方法的实例
PostMethod postMethod = new PostMethod(
"http://localhost:8080/b/test1.do");
// GetMethod getMethod = new
// GetMethod("http://localhost:8080/b/test1.do");
// （3）设置http request头
List<Header> headers = new ArrayList<Header>();
headers.add(new Header("tianjun_key","tianjun_value"));
httpClient.getHostConfiguration().getParams().setParameter(
"http.default-headers", headers);
// 使用系统提供的默认的恢复策略
postMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
new DefaultHttpMethodRetryHandler());
// getMethod.getParams().setParameter(HttpMethodParams.RETRY_HANDLER,
// new DefaultHttpMethodRetryHandler());
try
{
// （4）执行postMethod
int statusCode = httpClient.executeMethod(postMethod);
if (statusCode != HttpStatus.SC_OK)
{
System.err.println("Method failed:"
+ postMethod.getStatusLine());
}
// （5）读取response头信息
Header headerResponse = postMethod
.getResponseHeader("response_key");
String headerStr = headerResponse.getValue();
// （6）读取内容
byte[] responseBody = postMethod.getResponseBody();
// （7） 处理内容
System.out.println(headerStr);
System.out.println(new String(responseBody));
} catch (HttpException e)
{
// 发生致命的异常，可能是协议不对或者返回的内容有问题
System.out.println("Please check your provided http address!");
e.printStackTrace();
} catch (IOException e)
{
e.printStackTrace();
} catch (Exception e)
{
e.printStackTrace();
} finally
{
// 释放连接
postMethod.releaseConnection();
}
}
}
```
