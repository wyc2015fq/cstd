# Pads logic 中如何将一个大的元器件分成好几部分来画 - xqhrs232的专栏 - CSDN博客
2013年07月10日 11:09:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1799
原文地址::[http://zhidao.baidu.com/question/277859012.html](http://zhidao.baidu.com/question/277859012.html)
你可以在原理图中tools--parts editor--editor electrical--gates下，添加几个（你想分成几部分，就添加几个）CAE decal（随便什么图形均可），然后点击ok退出。点击edit graphics可以进入各部分的编辑界面，通过系统自带的向导（CAE decal Wizard），可以随意定义引脚的数量以及各引脚的功能。每个gate定义好之后要记得通过file--return to part退出，否则系统不予保存（我以前经常忘记，直接就关，结果没有保存。很伤人的！），最后你想要的各个模块在你的发挥下就一个一个定义好了，在原理图中调用的时候也就一个一个的被调用出来了，随你想放在哪里。不过他们还是一个PART，只是被分成了好几个gate而已。
