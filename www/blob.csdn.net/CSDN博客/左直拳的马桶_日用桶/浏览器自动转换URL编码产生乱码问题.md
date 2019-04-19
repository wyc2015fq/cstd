# 浏览器自动转换URL编码产生乱码问题 - 左直拳的马桶_日用桶 - CSDN博客
2011年03月15日 18:53:00[左直拳](https://me.csdn.net/leftfist)阅读数：4718
如果网址中含有汉字，浏览器会自动将其转换成URL编码，而在接收端，它又自动转换回来。
大部分时候都没什么问题，但有时候就很讨厌。因为生成这种URL编码，编码时的字符集和解码字符集不一定相同，比如编码时用GB2312，解码用UTF-8，就出现乱码。
这个时候，提取到的参数，自然就乱。
解决之道是不要用Request.QueryString来提取，而是用Request.RawUrl来获取。后者是原始URL，没有自动解码的。自己获取，自己处理。
//scode = Request.QueryString["word"];
int p = Request.RawUrl
.IndexOf("word=");
string scode = Request.RawUrl.Substring(p + 5);
