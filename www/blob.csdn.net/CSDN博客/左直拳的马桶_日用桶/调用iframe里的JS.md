# 调用iframe里的JS - 左直拳的马桶_日用桶 - CSDN博客
2017年08月12日 14:59:29[左直拳](https://me.csdn.net/leftfist)阅读数：942
操作iframe里的DOM，都很熟悉了，但如果是调用iframe里的JS呢？ 
（ 
是我记错了，操作iframe里的DOM，其实一点儿也说不上很熟悉。对于iframe里对象而言，访问外层的父对象确实很容易，比如top、parent，方便得很。top、parent都是window对象。
但如果要访问iframe里的对象，该如何做？答案就是通过contentWindow、contentDocument来进行。其中contentWindow是iframe的window，contentDocument是iframe的document。
`var div1 = document.getElementById("frameSlider").contentDocument.getElementById("div1");//获得当前页面里的iframe对象（id="frameSlider"）里的某个DIV(id='div1')`
）
好像中文技术文章都找不到答案，尽管标榜着诸如“不得不说的秘密”之类的标签，但然并卵，找不到本题提问的答案。我一度认为这个是实现不了的。
后来还是在stack-overflow里看到了。这样子：
`document.getElementById('targetFrame').contentWindow.targetFunction();`
关键是这个contentWindow对象。
[https://stackoverflow.com/questions/251420/invoking-javascript-code-in-an-iframe-from-the-parent-page](https://stackoverflow.com/questions/251420/invoking-javascript-code-in-an-iframe-from-the-parent-page)
