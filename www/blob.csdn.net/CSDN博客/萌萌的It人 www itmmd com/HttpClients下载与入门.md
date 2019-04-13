
# HttpClients下载与入门 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年04月11日 09:57:42[Jlins](https://me.csdn.net/dyllove98)阅读数：2216


Http协议的重要性相信不用我多说了，HttpClient相比传统JDK自带的URLConnection，增加了易用性和灵活性（具体区别，日后我们再讨论），它不仅是客户端发送Http请求变得容易，而且也方便了开发人员测试接口（基于Http协议的），即提高了开发的效率，也方便提高代码的健壮性。因此熟练掌握HttpClient是很重要的必修内容，掌握HttpClient后，相信对于Http协议的了解会更加深入。
你可以从官方获取最新版本：[http://hc.apache.org/index.html](http://hc.apache.org/index.html)。也可以从这里获取官方使用入门示例。
以下示例，输出所有头内容，和页面返回的内容。
[?](http://javacui.com/opensource/322.html#)
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|import|java.io.ByteArrayOutputStream;
|import|java.io.InputStream;
|import|java.util.ArrayList;
|import|java.util.List;
|import|org.apache.http.Header;
|import|org.apache.http.HttpEntity;
|import|org.apache.http.NameValuePair;
|import|org.apache.http.client.entity.UrlEncodedFormEntity;
|import|org.apache.http.client.methods.CloseableHttpResponse;
|import|org.apache.http.client.methods.HttpGet;
|import|org.apache.http.client.methods.HttpPost;
|import|org.apache.http.impl.client.CloseableHttpClient;
|import|org.apache.http.impl.client.HttpClients;
|import|org.apache.http.message.BasicNameValuePair;
|public|class|QuickStart {
|public|static|void|main(String[] args) {
|try|{
|CloseableHttpClient httpclient = HttpClients.createDefault();
|HttpGet httpGet =|new|HttpGet(|"|[http://javacui.com](http://javacui.com/)|"|);
|CloseableHttpResponse response1 = httpclient.execute(httpGet);
|try|{
|System.out.println(response1.getStatusLine());|// 读取状态信息
|Header[] hd = response1.getAllHeaders();|// 所有头信息
|for|(Header h : hd){
|System.out.println(h.getName() +|"："|+ h.getValue());
|}
|HttpEntity entity1 = response1.getEntity();
|System.out.println(EntityUtils.toString(entity1));
|}|finally|{
|response1.close();
|}
|HttpPost httpPost =|new|HttpPost(|"|[http://javacui.com](http://javacui.com/)|"|);
|List <NameValuePair> paras =|new|ArrayList <NameValuePair>();|// 设置表单参数
|paras.add(|new|BasicNameValuePair(|"username"|,|"name"|));
|paras.add(|new|BasicNameValuePair(|"password"|,|"pass"|));
|httpPost.setEntity(|new|UrlEncodedFormEntity(paras));
|CloseableHttpResponse response2 = httpclient.execute(httpPost);
|try|{
|System.out.println(response1.getStatusLine());|// 读取状态信息
|Header[] hd = response1.getAllHeaders();|// 所有头信息
|for|(Header h : hd){
|System.out.println(h.getName() +|"："|+ h.getValue());
|}
|HttpEntity entity1 = response1.getEntity();
|System.out.println(EntityUtils.toString(entity1));
|}|finally|{
|response2.close();
|}
|}|catch|(Exception e) {
|e.printStackTrace();
|}
|}
|/**
|* 读取流
|*/
|public|static|byte|[] readStream(InputStream inStream)|throws|Exception {
|ByteArrayOutputStream outSteam =|new|ByteArrayOutputStream();
|byte|[] buffer =|new|byte|[|1024|];
|int|len = -|1|;
|while|((len = inStream.read(buffer)) != -|1|) {
|outSteam.write(buffer,|0|, len);
|}
|outSteam.close();
|inStream.close();
|return|outSteam.toByteArray();
|}
|}
|// 结束
|

GET和POST请求都输出了获取到的服务器内容，POST请求设置了请求的表单参数。

参考资料：


## [jquery
 ajax 设置json类型的返回串问题](http://www.itmmd.com/201504/694.html)
## http://www.itmmd.com/201504/694.html
## [java
 jsp实现万年历 + 日程管理，已经设计好u](http://www.itmmd.com/201504/693.html)
## http://www.itmmd.com/201504/693.html

## [jquery
 动态添加和删除 ul li列表](http://www.itmmd.com/201504/692.html)
## http://www.itmmd.com/201504/692.html
更多信息请访问：[萌萌的it人](http://www.itmmd.com)



