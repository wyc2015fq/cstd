# 输出JS链接文件再执行 - 左直拳的马桶_日用桶 - CSDN博客
2008年07月08日 18:41:00[左直拳](https://me.csdn.net/leftfist)阅读数：1667
输出JS链接文件再执行
左直拳
象<scriptlanguage="JavaScript"src="outer.js"type="text/javascript"></script>这类的句子只能够一早老老实实的写好放在页面里加载执行吗？
错。
一样可以通过document.write()的形式输出，并且浏览器还是一视同仁的执行，不打任何折扣，IE也好，FF也好，都没有问题。
只是，“<scriptlanguage="JavaScript"src="outer.js"type="text/javascript"></script>”这种句子是非常特殊的，不能当作一般的字符串来对待，象
var script = “<scriptlanguage="JavaScript"src="outer.js"type="text/javascript"></script>”;
一执行，分分钟报错。浏览器说存在未结束的字符串常量。究其原因，<script>是特殊的关键字，虽然包含在双引号里，但浏览器仍然认为它是一个表达式。
所以应该做一些处理再输出。
服务器端：
protected string content = “”;
Regex re = newRegex(@"script", RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.IgnorePatternWhitespace);
content = re.Replace(content, "scrip+t");//script à scrip+t
re = newRegex(@"<!--|//-->", RegexOptions.IgnoreCase | RegexOptions.Multiline | RegexOptions.IgnorePatternWhitespace);//去掉注释符号
content = re.Replace(content, "");
客户端：
var content = <%=content%>;
var r = /scrip/+t/gi;
content = content.replace(r,"script");//恢复过来再输出
document.write(content);
