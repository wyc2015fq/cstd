# 在js脚本中找到控件 - 左直拳的马桶_日用桶 - CSDN博客
2006年09月21日 22:32:00[左直拳](https://me.csdn.net/leftfist)阅读数：2149
**用****js****脚本找到它**
左直拳
在脚本里想找到一个控件，有几种方法。
如果是FORM里面的控件，如textbox，checkbox，select，等等，可以用
document.Form NAME.控件NAME 的形式访问。
但很多时候一些控件都不属于FORM，又怎样访问？
以前，可以用 document.all.控件ID 的方法，例如，有个<input type=”button” id=”cmdSure” value=”确定”/>，用
document.all.cmdSure.click() 就可以触发该按牛的电击事件。
不过，用document.all现在好象不够标准（今天是样样讲求标准的时代），微软的VS。NET2005 的IDE环境里里已经找不到这种写法了，不过也不会出错。
标准的写法是用
1、document.getElementByID(控件ID值)
2、document.getElementsByName(控件名)
3、document.getElementsByTagName(标签名)
注意第2、3种是返回数组，因为在一个页面中，同名的控件可能不止一个（如radioButton），同标签的就更不用说了。
所谓的标签，就是指HTML标签，比如，想找出页面中所有的DIV，就可以用3，
document.getElementsByTagName(“div”)
