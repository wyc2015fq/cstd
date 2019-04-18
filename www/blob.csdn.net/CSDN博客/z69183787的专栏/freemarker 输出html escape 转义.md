# freemarker 输出html escape 转义 - z69183787的专栏 - CSDN博客
2015年07月31日 10:08:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12311
字符串插入指南：不要忘了转义！ 如果插值在文本区（也就是说，不再字符串表达式中），如果escapse指令起作用了，即将被插入的字符串会被自动转义。如果你要生成HTML，那么强烈建议你利用它来阻止跨站脚本攻击和非格式良好的HTML页面。这里有一个示例：
<#escape x as x?html>
...
<p>Title: ${book.title}</p>
<p>Description: <#noescape>${book.description}</#noescape></p>
<h2>Comments:</h2>
<#list comments as comment>
<div class="comment">
${comment}
</div>
</#list>
...
</#escape>
这个示例展示了当生成HTML时，你最好将完整的模板放入到escape指令中。那么，如果book.title包含&，它就会在输出中被替换成&，而页面还会保持格式良好的HTML。如果用户注释包含如<iframe>（或其它的元素）的标记，那么就会被转义成如<iframe>的样子，使他们没有任何有害点。但是有时在数据模型中真的需要HTML，比如我们假设上面的book.description在数据库中的存储是HTML格式的，那么此时你不得不使用noescape来抵消escape的转义，模板就会像这样了：
这和之前示例的效果是一样的，但是这里你可能会忘记一些?html内建函数，就会有安全上的问题了。在之前的示例中，你可能忘记一些noescape，也会造成不良的输出，但是起码是没有安全隐患的。
