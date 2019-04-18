# escape()、encodeURI()、encodeURIComponent()区别详解 - z69183787的专栏 - CSDN博客
2012年10月25日 14:22:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1081
JavaScript中有三个可以对字符串编码的函数，分别是： escape,encodeURI,encodeURIComponent，相应3个解码函数：unescape,decodeURI,decodeURIComponent 。
下面简单介绍一下它们的区别
1 escape()函数
定义和用法 
escape() 函数可对字符串进行编码，这样就可以在所有的计算机上读取该字符串。
语法 
escape(string)
参数  描述  
string  必需。要被转义或编码的字符串。 
返回值 
已编码的 string 的副本。其中某些字符被替换成了十六进制的转义序列。
说明 
该方法不会对 ASCII 字母和数字进行编码，也不会对下面这些 ASCII 标点符号进行编码： - _ . ! ~ * ' ( ) 。其他所有的字符都会被转义序列替换。
2 encodeURI()函数 
定义和用法 
encodeURI() 函数可把字符串作为 URI 进行编码。
语法 
encodeURI(URIstring)
参数  描述  
URIstring  必需。一个字符串，含有 URI 或其他要编码的文本。 
返回值 
URIstring 的副本，其中的某些字符将被十六进制的转义序列进行替换。
说明 
该方法不会对 ASCII 字母和数字进行编码，也不会对这些 ASCII 标点符号进行编码： - _ . ! ~ * ' ( ) 。
该方法的目的是对 URI 进行完整的编码，因此对以下在 URI 中具有特殊含义的 ASCII 标点符号，encodeURI() 函数是不会进行转义的：;/?:@&=+$,#
3 encodeURIComponent() 函数
定义和用法 
encodeURIComponent() 函数可把字符串作为 URI 组件进行编码。
语法 
encodeURIComponent(URIstring)
参数  描述  
URIstring  必需。一个字符串，含有 URI 组件或其他要编码的文本。 
返回值 
URIstring 的副本，其中的某些字符将被十六进制的转义序列进行替换。
说明 
该方法不会对 ASCII 字母和数字进行编码，也不会对这些 ASCII 标点符号进行编码： - _ . ! ~ * ' ( ) 。
其他字符（比如 ：;/?:@&=+$,# 这些用于分隔 URI 组件的标点符号），都是由一个或多个十六进制的转义序列替换的。
提示和注释 
提示：请注意 encodeURIComponent() 函数 与 encodeURI() 函数的区别之处，前者假定它的参数是 URI 的一部分（比如协议、主机名、路径或查询字符串）。因此 encodeURIComponent() 函数将转义用于分隔 URI 各个部分的标点符号。
4 总结：
 通过对三个函数的分析，我们可以知道：escape()除了 ASCII 字母、数字和特定的符号外，对传进来的字符串全部进行转义编码，因此如果想对URL编码，最好不要使用此方法。而encodeURI() 用于编码整个URI,因为URI中的合法字符都不会被编码转换。encodeURIComponent方法在编码单个URIComponent（指请求参数）应当是最常用的，它可以讲参数中的中文、特殊字符进行转义，而不会影响整个URL。
5 示例：
**1 escape()**
<script type="text/javascript">
document.write(escape("[http://www.w3school.com.cn/](http://www.w3school.com.cn/)") + "<br />")
document.write(escape("?!=()#%&"))
</script>输出：
http%3A//www.w3school.com.cn
%3F%21%3D%28%29%23%25%26
**2 encodeURI()**
<script type="text/javascript">
document.write(encodeURI("[http://www.w3school.com.cn/](http://www.w3school.com.cn/)")+ "<br />")
document.write(encodeURI("[http://www.w3school.com.cn/My](http://www.w3school.com.cn/My) first/"))
document.write(encodeURI(",/?:@&=+$#"))
</script>输出：
[http://www.w3school.com.cn/](http://www.w3school.com.cn/)
[http://www.w3school.com.cn/My%20first/](http://www.w3school.com.cn/My%20first/)
,/?:@&=+$#
对整个URL进行编码，而URL的特定标识符不会被转码。
**3 encodeURIComponent()**
例1：
<script type="text/javascript">
document.write(encodeURIComponent("[http://www.w3school.com.cn/](http://www.w3school.com.cn/)"))
document.write("<br />")
document.write(encodeURIComponent("[http://www.w3school.com.cn/p](http://www.w3school.com.cn/p) 1/"))
document.write("<br />")
document.write(encodeURIComponent(",/?:@&=+$#"))
</script输出：
http%3A%2F%2Fwww.w3school.com.cn 
http%3A%2F%2Fwww.w3school.com.cn%2Fp%201%2F 
%2C%2F%3F%3A%40%26%3D%2B%24%23
例2：<script language="javascript">document.write('
<a href="[http://passport.baidu.com/?logout&aid=7&u='+encodeURIComponent("http://cang.baidu.com/bruce42")+'](http://passport.baidu.com/?logout&aid=7&u=' rel=)">退出</a>');</script>
对URL中的参数进行编码，因为参数也是一个URL，如果不编码会影响整个URL的跳转。
