# CSS样式嵌套 - 左直拳的马桶_日用桶 - CSDN博客
2007年09月18日 17:54:00[左直拳](https://me.csdn.net/leftfist)阅读数：4648
CSS样式嵌套
左直拳
如果在一个<TD>、<DIV>里包含有一个链接<A>，如：
<tdclass="tdCSS"><ahref="penis.htm">长洲宾客人数多</a></td>
其中TD制订的CSS样式tdCSS对这个链接A是不起作用的。比如说，明明定好了这个TD内的文字多大，什么颜色，现在加了一个〈A〉后，就一切都变了样。
亲爱的，要怎么样你才能起作用？
一切尽在掌握。写CSS的时候可以这样写：
.tdCSS{width:100px; background-color:#f3f9ff;font-size:11pt; color:#3F6798;font-weight:bold;border:solid1px#a3aBb2; text-align:center;cursor:hand;}
.tdCSSA:link {color:#3F6798;text-decoration: none; font-size:11pt;}
.tdCSSA:visited {color:#3F6798; text-decoration: none; font-size:11pt;}
.tdCSSA:hover {color: red; text-decoration: underline; font-size:11pt;}
.tdCSSA:active {color:#3F6798; text-decoration: none; font-size:11pt;}
