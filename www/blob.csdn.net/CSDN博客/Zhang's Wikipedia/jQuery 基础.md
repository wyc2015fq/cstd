
# jQuery 基础 - Zhang's Wikipedia - CSDN博客


2018年03月03日 23:42:38[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：159个人分类：[web](https://blog.csdn.net/lanchunhui/article/category/6256551)



## 1. 简介
jQuery是 JavaScript 的一个集成库，语法简洁，它紧密集成了DOM，
提供了方便的ajax的辅助方法、令人震撼的界面效果，
可插拔的体系结构。
<scripttype="text/javascript"src="https://cdn.bootcss.com/jquery/3.2.1/jquery.min.js"></script>
## 2. 在线 jQuery.min.js 的引用
[在线jquery.min.js引用](http://blog.csdn.net/superit401/article/details/72334129)
在分类上主要分为 ；
jQuery
jQuery mobile
jQuery UI
## 3. 选择器
选择网页里的全部<p>元素：$("p")
选择某 id：$("\#num1")（id 前要加 \#）
在选取语句后面，通常会加上处理动作，以设置 id 为 num1 字段的值为例：$("\#num1").val(12);
jQuery 的$ 函数其实是附加在浏览器 window 对象（BOM）下的全局函数，其中的$是 jQuery 的别名，故如下的三种写法均正确：
window.jQuery(选择器) ⇒window.$(选择器) ⇒$(选择器)

