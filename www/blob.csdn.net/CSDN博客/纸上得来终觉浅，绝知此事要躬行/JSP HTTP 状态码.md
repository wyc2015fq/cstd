# JSP HTTP 状态码 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 16:57:39[boonya](https://me.csdn.net/boonya)阅读数：493








文章来源：[http://www.runoob.com/jsp/jsp-http-status-codes.html](http://www.runoob.com/jsp/jsp-http-status-codes.html)



HTTP请求与HTTP响应的格式相近，都有着如下结构：
- 以状态行+CRLF（回车换行）开始
- 零行或多行头模块+CRLF
- 一个空行，比如CRLF
- 可选的消息体比如文件，查询数据，查询输出

举例来说，一个服务器响应头看起来就像下面这样： 
HTTP/1.1200 OK
Content-Type: text/html
Header2:......HeaderN:...(BlankLine)<!doctype ...><html><head>...</head><body>...</body></html>状态行包含HTTP版本，一个状态码，和状态码相对应的短消息。
下表列出了可能会从服务器返回的HTTP状态码和与之关联的消息：
|**状态码**|**消息**|**描述**|
|----|----|----|
|100|Continue|只有一部分请求被服务器接收，但只要没被服务器拒绝，客户端就会延续这个请求|
|101|Switching Protocols|服务器交换机协议|
|200|OK|请求被确认|
|201|Created|请求时完整的，新的资源被创建|
|202|Accepted|请求被接受，但未处理完|
|203|Non-authoritative Information||
|204|No Content||
|205|Reset Content||
|206|Partial Content||
|300|Multiple Choices|一个超链接表，用户可以选择一个超链接并访问，最大支持5个超链接|
|301|Moved Permanently|被请求的页面已经移动到了新的URL下|
|302|Found|被请求的页面暂时性地移动到了新的URL下|
|303|See Other|被请求的页面可以在一个不同的URL下找到|
|304|Not Modified||
|305|Use Proxy||
|306|*Unused*|已经不再使用此状态码，但状态码被保留|
|307|Temporary Redirect|被请求的页面暂时性地移动到了新的URL下|
|400|Bad Request|服务器无法识别请求|
|401|Unauthorized|被请求的页面需要用户名和密码|
|402|Payment Required|*目前还不能使用此状态码*|
|403|Forbidden|禁止访问所请求的页面|
|404|Not Found|服务器无法找到所请求的页面|
|405|Method Not Allowed|请求中所指定的方法不被允许|
|406|Not Acceptable|服务器只能创建一个客户端无法接受的响应|
|407|Proxy Authentication Required|在请求被服务前必须认证一个代理服务器|
|408|Request Timeout|请求时间超过了服务器所能等待的时间，连接被断开|
|409|Conflict|请求有矛盾的地方|
|410|Gone|被请求的页面不再可用|
|411|Length Required|"Content-Length"没有被定义，服务器拒绝接受请求|
|412|Precondition Failed|请求的前提条件被服务器评估为false|
|413|Request Entity Too Large|因为请求的实体太大，服务器拒绝接受请求|
|414|Request-url Too Long|服务器拒绝接受请求，因为URL太长。多出现在把"POST"请求转换为"GET"请求时所附带的大量查询信息|
|415|Unsupported Media Type|服务器拒绝接受请求，因为媒体类型不被支持|
|417|Expectation Failed||
|500|Internal Server Error|请求不完整，服务器遇见了出乎意料的状况|
|501|Not Implemented|请求不完整，服务器不提供所需要的功能|
|502|Bad Gateway|请求不完整，服务器从上游服务器接受了一个无效的响应|
|503|Service Unavailable|请求不完整，服务器暂时重启或关闭|
|504|Gateway Timeout|网关超时|
|505|HTTP Version Not Supported|服务器不支持所指定的HTTP版本|

## 设置HTTP状态码的方法

下表列出了HttpServletResponse 类中用来设置状态码的方法：
|**S.N.**|**方法 & 描述**|
|----|----|
|1|**public void setStatus ( int statusCode )**此方法可以设置任意的状态码。如果您的响应包含一个特殊的状态码和一个文档，请确保在用PrintWriter返回任何内容前调用setStatus方法|
|2|**public void sendRedirect(String url)**此方法产生302响应，同时产生一个 *Location* 头告诉URL 一个新的文档|
|3|**public void sendError(int code, String message)**此方法将一个状态码(通常为 404)和一个短消息，自动插入HTML文档中并发回给客户端|

## HTTP状态码程序示例

接下来的例子将会发送407错误码给浏览器，然后浏览器将会告诉您"Need authentication!!!"。
<html><head><title>Setting HTTP Status Code</title></head><body><%// 设置错误代码，并说明原因
   response.sendError(407,"Need authentication!!!");
%>
</body></html>
访问以上JSP页面，将会得到以下结果：
![js_http_status_codes](http://www.runoob.com/wp-content/uploads/2014/01/js_http_status_codes.jpg)
您也可以试试使用其他的状态码，看会不会得到什么意想不到结果。










