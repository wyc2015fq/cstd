# 关于JQuery Class选择器的一点 - =朝晖= - 博客园
# [关于JQuery Class选择器的一点](https://www.cnblogs.com/dhcn/p/7100132.html)
    当某个元素的Class为为两个字符串的时候,那用class选择器的时候就必须把两个字符串都写上否则无效
```html;gutter
<div class="cla clb">11111<div>  
<script language="<a href="http://lib.csdn.net/base/javascript" class='replace_word' title="JavaScript知识库" target='_blank' style='color:#df3434; font-weight:bold;'>JavaScript</a>">  
alert($(.cla clb).text()+"1");  
alert($(.clb).text()+"2");  
<script>
```
这也就意味着[jQuery](http://lib.csdn.net/base/jquery)的文档结构采用正常一般XML DOM结构，而不是浏览器HTML文档结构。
jQeury版本：1.3.2

