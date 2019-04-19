# 学习笔记——WPF - 左直拳的马桶_日用桶 - CSDN博客
2014年03月06日 10:31:05[左直拳](https://me.csdn.net/leftfist)阅读数：1944
WPF,Windows Presentation Foundation。主要作用在于界面呈现，但Presentation却是介绍的意思。
WPF采用XAML + 代码相结合的方式，感觉跟ASP.NET很相像。互联网时代，一切向WEB方向发展，所谓B/S、c/s，界线已经越来越模糊，没有本质上的区别了。其实，所谓B/S结构，本质上也算是C/S结构，只不过，是一种比较特殊的C/S结构罢了：
客户端：浏览器
服务器端：WEB服务器
C/S结构并非像我们印象中，只有客户端、服务器端2层，也有3层、多层的，如图
普通C/S结构图
![](https://img-blog.csdn.net/20140306102419281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3层C/S结构图
![](https://img-blog.csdn.net/20140306102455640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
B/S
![](https://img-blog.csdn.net/20140306102516828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
但是，微软总是喜欢自己搞一套，自己搞了一个XAML。虽然以XML为基础，但是毕竟是个新东西。XML ,HTML5这些已经非常完善和流行了，为什么还要搞这么个东西呢。你看现在流行的mongoDB,node.js，都支持javascript,json这些，这样不更有利于和WEB结合吗？
javascript，我99年毕业出来就已经在用这个东西。javascript一路走来，饱受诟病，什么不安全啦，不兼容啦，没想到生命顽强，且有越来越火的趋势。如果以后java、c#都式微、消亡了，只剩javascript独大，一点都不奇怪。
