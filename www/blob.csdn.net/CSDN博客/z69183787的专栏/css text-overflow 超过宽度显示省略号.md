# css text-overflow 超过宽度显示省略号 - z69183787的专栏 - CSDN博客
2014年04月10日 06:10:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：941
个人分类：[前端-Css](https://blog.csdn.net/z69183787/article/category/2175041)
使用text-overflow:ellipsis对溢出文本显示[省略号](http://zhidao.baidu.com/search?word=%E7%9C%81%E7%95%A5%E5%8F%B7&fr=qb_search_exp&ie=utf8)有两个好处，一是不用通过程序限定字数；二是有利于SEO。需要使用对对溢出文本显示[省略号](http://zhidao.baidu.com/search?word=%E7%9C%81%E7%95%A5%E5%8F%B7&fr=qb_search_exp&ie=utf8)的通常是文章标题列表，这样处理对搜索引擎更友好，因为标题实际上并未被截字，而是局限于宽度而未被显示而已。
通常的做法是这样的：
1.overflow:hidden;
2.text-overflow:ellipsis;
3.-o-text-overflow:ellipsis;
4.white-space:nowrap;
5.width:100%;
其中，overflow: hidden和white-space: nowrap都是必须的否则不会显示[省略号](http://zhidao.baidu.com/search?word=%E7%9C%81%E7%95%A5%E5%8F%B7&fr=qb_search_exp&ie=utf8)；-o-text-overflow: ellipsis针对Opera；而宽度的设定主要是针对IE6；
该方法支持Internet Explorer, Safari, Chrome 和 Opera，但FF并不支持，不过可以通过Jquery来实现类似的效果。
下载这个Jquery插件：jQuery ellipsis plugin
调用方法：
1.$(document).ready(function() {
2.    $('.ellipsis').ellipsis();
3.}
