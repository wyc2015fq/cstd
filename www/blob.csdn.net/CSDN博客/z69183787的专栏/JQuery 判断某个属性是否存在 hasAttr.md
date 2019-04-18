# JQuery 判断某个属性是否存在 hasAttr - z69183787的专栏 - CSDN博客
2013年12月04日 15:26:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21500
在JQuery编码中,我们会判断元素是否存在某个属性.比如是否包含 class="new" 的样式呢.JQuery判断就非常简单了,因为有 hasClass这个方法 $("input[name=new]").hasClass("new") 即可判断.
但是有时候我们需要判断别的属性,比如有的 a 链接包含 rel 属性,有的没有rel属性.这时该怎么判断呢?
这时就没有现成的方法了. 如果存在某个属性 $("#aid").attr("rel") 会返回 rel的值,如果不存在 rel属性则会返回"undefined"
undefined 就是 undefined类型 , if($("#aid").attr("rel")=="undefined") 这个判断可能不成立.
因为类型不相同.
建议使用 if(typeof($("#aid").attr("rel"))=="undefined") 即可
