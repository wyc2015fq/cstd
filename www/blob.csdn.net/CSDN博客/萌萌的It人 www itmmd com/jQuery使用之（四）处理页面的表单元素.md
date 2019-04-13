
# jQuery使用之（四）处理页面的表单元素 - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月19日 11:37:56[Jlins](https://me.csdn.net/dyllove98)阅读数：1170



表单是一个特殊的页面元素，value的值是最受关注的，jQuery提供了强大的val()方法来处理相关的操作。
1.获取表单元素的值。
直接调用val()方法时可以获取选择器的 中的第一个元素的value值。例如：
$("[name=radioGroup]:checked").val;
以上代码直接获取属性为radioGroup的表单元素中被选中的value值，十分便捷。对于某些表单元素，例如<option><button>没有value值将获取其显示的文本值。
如果选择器中的第一个表单元素是多选的（例如下拉菜单），val()将返回由选中项value值组成的数组
[http://www.cnblogs.com/ahthw/p/4221608.html](http://www.cnblogs.com/ahthw/p/4221608.html)使用过DOM的方式获取了单选和多选情况下的选值，相比jQuery，代码十分长。使用val()可以直接获取表单的元素值，不用考虑下拉菜单是单选还是多选。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">functiondisplayVals() {//直接获取选中项的value值varsingleValues=$("\#constellation1").val();varmultipleValues=$("\#constellation2").val()||[];//因为存在不选的情况$("span").html("<b>Single:</b>"+singleValues+"<br><b>Multiple:</b>"+multipleValues.join(","));
            }
            $(function() {//当修改选中项时调用$("select").change(displayVals);
                displayVals();
            });</script><selectid="constellation1"><optionvalue="Aries">白羊</option><optionvalue="Taurus">金牛</option><optionvalue="Gemini">双子</option><optionvalue="Cancer">巨蟹</option><optionvalue="Leo">狮子</option><optionvalue="Virgo">处女</option><optionvalue="Libra">天秤</option><optionvalue="Scorpio">天蝎</option><optionvalue="Sagittarius">射手</option><optionvalue="Capricorn">摩羯</option><optionvalue="Aquarius">水瓶</option><optionvalue="Pisces">双鱼</option></select><selectid="constellation2"multiple="multiple"style="height:120px;"><optionvalue="Aries">白羊</option><optionvalue="Taurus">金牛</option><optionvalue="Gemini">双子</option><optionvalue="Cancer">巨蟹</option><optionvalue="Leo">狮子</option><optionvalue="Virgo">处女</option><optionvalue="Libra">天秤</option><optionvalue="Scorpio">天蝎</option><optionvalue="Sagittarius">射手</option><optionvalue="Capricorn">摩羯</option><optionvalue="Aquarius">水瓶</option><optionvalue="Pisces">双鱼</option></select><span></span>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码直接使用val()获取了select的值，jQuery方式大大的缩短了代码的长度。
2.设置表单元素的值。
与attr()和css()一样，val()也可以设置value的值，使用方法也大同小异。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scripttype="text/javascript">$(function() {
                $("input[type=button]").click(function() {varsValue=$(this).val();//先获取按钮的value值$("input[type=text]").val(sValue);//赋给文本框});
            });</script><p><inputtype="button"value="Feed"><inputtype="button"value="the"><inputtype="button"value="Input"></p><p><inputtype="text"value="click a button"></p>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码使用到了两次val()方法，一次是获取button的value值，另一次是将获取的文本赋值给input文本框里。
写博客不容易，欢迎大家给评论以给鼓励，分享是快乐！欢迎大家拍砖和点赞。（JavaScript、ajax、jQuery系列文章不断更新，关注我即可随时关注更新）

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章




