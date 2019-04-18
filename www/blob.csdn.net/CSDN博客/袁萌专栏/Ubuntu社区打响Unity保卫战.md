# Ubuntu社区打响Unity保卫战 - 袁萌专栏 - CSDN博客
2012年03月14日 04:12:17[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1013标签：[ubuntu																[bugs																[linux																[算法																[api																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=bugs&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
    大家知道，灵巧HUD是Ubuntu
 12.04新版本的标志性特性，我们可以说，新版本的成败就在此一举。......HUD怎么了？
在Unity环境中，半透明的HUD窗口显得比较脆弱，容易受到其他因素（比如，使用其他“快捷键”）的”干扰“，系统不够稳定，而且，HUD反应有点儿迟钝。问题出在何处？起初，有人怀疑HUD算法的正确性（模糊匹配），但是，问题到底出在哪里？难道是HUD的大方向完全搞错了？
2月14日，HUD正式露面，接受人们的“盯视”（或“审视”）。可以说，HUD的理论基础已经反复推敲了无数次，应该没有什么大问题了，需要把眼光放远一些，看看HUD周边的其他环境因素的影响。难道是Unity本身的问题？实际上，Unity的技术基础就是3D窗口管理器Compiz，而Compiz又是什么呢？
实际上，2006年1月，原Novell首先提出3D窗口管理器Compiz的概念，并且做了一些初步的探讨。直到2010年7月4日，Compiz开发者Sam
 Spilsbury还对外宣布：“the
 release of Compiz 0.9.0, still in unstable release”，而且，“with
 a new API, rewritten in C++”。后来，Canonical公司很有远见地聘用了他，让他领导Compiz团队为Ubuntu继续贡献力量。
在上述大背景之下，3月12日，Ubuntu社区解决了一个潜于Compiz系统内部深处的大Bug，“which
 reduces the compiz CPU usage by an astounding（令人惊奇）42
 percent”，意思是说，排除了Compiz吃CPU、效率低下的大缺陷（Bug），找出了问题关键之所在。至此，Unity得救了。
与此同时，也解决了100多个有关Compiz的Bugs，“with
 more than 15-25 of them being Critical and 20-30 being High”，而且，“Unity
 5.6 has been released which brought a lot of bug fixes to HUD”，意思是说，已经排除了某些快捷键对HUD的干扰影响。至此，终于雨过天晴，皆大欢喜。
如今，HUD与Unity终于“融合”在一起了，十分完美，无可挑剔。现今，Ubuntu
 12.04已经进入Beta测试阶段，采用滚动更新（Rolling
 update）模式，昼夜不停，热火朝天。
总的看起来，Ubuntu
 12.04 LTS版本是Ubuntu发展的一个历史性里程碑。至此，人工智能技术与Linux桌面完美“结合”，成为一家人。至今，有人对Ubuntu还是“挑三捡四”，左也不是，右也不是，怎么也不行。我们要知道一点：Ubuntu是自由软件，中国人口多，尤其需要Ubuntu自由操作系统，需要LibreOffice自由办公套件。近日，我听说，国家又把钱（启动经费）发下去了，要搞什么“国产办公套件”。国家的钱花在这上面（闭门研发“自主办公套件”）到底值不值？......你那“电子表格”是从哪里来的？那是完全自主开发的吗？谁敢站出来打个“赌”？我准备奉陪到底！
说明：国人办事喜欢靠“关系”，科学、良知统统地不要。

