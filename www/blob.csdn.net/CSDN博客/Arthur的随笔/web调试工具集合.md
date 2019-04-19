# web调试工具集合 - Arthur的随笔 - CSDN博客
2011年12月12日 15:29:48[largetalk](https://me.csdn.net/largetalk)阅读数：587标签：[工具																[web																[firebug																[firefox																[chrome																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=chrome&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=firebug&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
个人分类：[django](https://blog.csdn.net/largetalk/article/category/716448)
做一个web系统，在开发和最后发布前都需要一系列的工具来帮助你
在开发中首推的组合是firefox+firebug, 因为firefox支持的js和css都还算标准，这样写出来的代码也比较好，而且firebug中一些特性如查看网络，对json的支持都能加快开发
在发布前可能要作一些兼容性测试，ie有ietester， 或者可以用这个网站 [http://browsershots.org/](http://browsershots.org/) 来帮你作
window平台下查看http内容的有fiddler2
chrome内置的开发人员工具也不错
有一个基于firebug的插件google page speed，可以给你优化页面提供一些建议
ps. 一个ie下不能保存cookie的bug让我纠结了好半天，原来是测试机系统时间设置不对导致的，不够细心阿，所以工具是其次，用心最重要
