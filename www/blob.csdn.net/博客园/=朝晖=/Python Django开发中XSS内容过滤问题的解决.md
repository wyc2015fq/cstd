# Python Django开发中XSS内容过滤问题的解决 - =朝晖= - 博客园
# [Python Django开发中XSS内容过滤问题的解决](https://www.cnblogs.com/dhcn/p/7105546.html)
from：[http://stackoverflow.com/questions/699468/python-html-sanitizer-scrubber-filter](http://stackoverflow.com/questions/699468/python-html-sanitizer-scrubber-filter)
通过下面这个代码就可以把内容过滤成干净的HTML内容,说明，这个代码来自上面Stackoverflow的回答
Use [`lxml.html.clean`](http://lxml.de/lxmlhtml.html#cleaning-up-html)! It's VERY easy!
```
from lxml.html.clean import clean_html
print clean_html(html)
```
```
<html>
 <head>
   <script type="text/javascript" src="evil-site"></script>
   <link rel="alternate" type="text/rss" src="evil-rss">
   <style>
     body {background-image: url(javascript:do_evil)};
     div {color: expression(evil)};
   </style>
 </head>
 <body onload="evil_function()">
    <!-- I am interpreted for EVIL! -->
   <a href="javascript:evil_function()">a link</a>
   <a href="#" onclick="evil_function()">another link</a>
   <p onclick="evil_function()">a paragraph</p>
   <div style="display: none">secret EVIL!</div>
   <object> of EVIL! </object>
   <iframe src="evil-site"></iframe>
   <form action="evil-site">
     Password: <input type="password" name="password">
   </form>
   <blink>annoying EVIL!</blink>
   <a href="evil-site">spam spam SPAM!</a>
   <image src="evil!">
 </body>
</html>
```
结果是：
```
<html>
  <body>
    <div>
      <style>/* deleted */</style>
      <a href="">a link</a>
      <a href="#">another link</a>
      <p>a paragraph</p>
      <div>secret EVIL!</div>
      of EVIL!
      Password:
      annoying EVIL!
      <a href="evil-site">spam spam SPAM!</a>
      <img src="evil!">
    </div>
  </body>
</html>
```
You can customize the elements you want to clean and whatnot.
关于Web开发中的安全过滤问题，摘引一下OWASP的ESAPI( [http://owasp-esapi-java.googlecode.com/svn/trunk_doc/latest/index.html](http://owasp-esapi-java.googlecode.com/svn/trunk_doc/latest/index.html) )的过滤方法接口列表，以供大家有所参考：
[Java](http://lib.csdn.net/base/java).lang.String canonicalize(java.lang.String input) 
          This method is equivalent to calling
 java.lang.String canonicalize(java.lang.String input, boolean strict) 
          This method is the equivalent to calling
 java.lang.String canonicalize(java.lang.String input, boolean restrictMultiple, boolean restrictMixed) 
          Canonicalization is simply the operation of reducing a possibly encoded string down to its simplest form.
 java.lang.String decodeForHTML(java.lang.String input) 
          Decodes HTML entities.
 byte[] decodeFromBase64(java.lang.String input) 
          Decode data encoded with BASE-64 encoding.
 java.lang.String decodeFromURL(java.lang.String input) 
          Decode from URL.
 java.lang.String encodeForBase64(byte[] input, boolean wrap) 
          Encode for Base64.
 java.lang.String encodeForCSS(java.lang.String input) 
          Encode data for use in Cascading Style Sheets (CSS) content.
 java.lang.String encodeForDN(java.lang.String input) 
          Encode data for use in an LDAP distinguished name.
 java.lang.String encodeForHTML(java.lang.String input) 
          Encode data for use in HTML using HTML entity encoding
 java.lang.String encodeForHTMLAttribute(java.lang.String input) 
          Encode data for use in HTML attributes.
 java.lang.String encodeForJavaScript(java.lang.String input) 
          Encode data for insertion inside a data value or function argument in [JavaScript](http://lib.csdn.net/base/javascript).
 java.lang.String encodeForLDAP(java.lang.String input) 
          Encode data for use in LDAP queries.
 java.lang.String encodeForOS(Codec codec, java.lang.String input) 
          Encode for an operating system command shell according to the selected codec (appropriate codecs include the WindowsCodec and UnixCodec).
 java.lang.String encodeForSQL(Codec codec, java.lang.String input) 
          Encode input for use in a SQL query, according to the selected codec (appropriate codecs include the MySQLCodec and OracleCodec).
 java.lang.String encodeForURL(java.lang.String input) 
          Encode for use in a URL.
 java.lang.String encodeForVBScript(java.lang.String input) 
          Encode data for insertion inside a data value in a Visual Basic script.
 java.lang.String encodeForXML(java.lang.String input) 
          Encode data for use in an XML element.
 java.lang.String encodeForXMLAttribute(java.lang.String input) 
          Encode data for use in an XML attribute.
 java.lang.String encodeForXPath(java.lang.String input) 
          Encode data for use in an XPath query.

