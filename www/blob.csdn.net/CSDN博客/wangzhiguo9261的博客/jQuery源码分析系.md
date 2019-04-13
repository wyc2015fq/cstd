
# jQuery源码分析系 - wangzhiguo9261的博客 - CSDN博客


2018年11月22日 17:03:38[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：23个人分类：[js](https://blog.csdn.net/wangzhiguo9261/article/category/7508784)



# [jQuery源码分析系列](https://www.cnblogs.com/aaronjs/p/3279314.html)
声明：本文为原创文章，如需转载，请注明来源并保留原文链接[Aaron](http://www.cnblogs.com/aaronjs/)，谢谢！
版本截止到2013.8.24 jQuery官方发布最新的的2.0.3为准
附上每一章的源码注释分析 ：[https://github.com/JsAaron/jQuery](https://github.com/JsAaron/jQuery)
**[正在编写的书 - jQuery架构设计与实现](https://github.com/JsAaron/jQuery/blob/master/README.md)**
**本人在慕课网的教程（完结）**
**jQuery源码解析（架构与依赖模块) 64课时**
**[jQuery源码解析（DOM与核心模块）](http://www.imooc.com/learn/222)****64课时**
**jQuery源码分析目录（完结）**
[jQuery源码分析系列(01) : 整体架构](http://www.cnblogs.com/aaronjs/p/3278578.html)
[jQuery源码分析系列(02) : 选择器](http://www.cnblogs.com/aaronjs/p/3281911.html)
[jQuery源码分析系列(03) : 选择器 Sizzle引擎 - 词法解析](http://www.cnblogs.com/aaronjs/p/3300797.html)
[jQuery源码分析系列(04) : 选择器 Sizzle引擎 - 解析原理](http://www.cnblogs.com/aaronjs/p/3310937.html)
[jQuery源码分析系列(05) : 选择器 Sizzle引擎 - 编译函数](http://www.cnblogs.com/aaronjs/p/3322466.html)
[jQuery源码分析系列(06) : 选择器 Sizzle引擎 - 超级匹配](http://www.cnblogs.com/aaronjs/p/3332805.html)
[jQuery源码分析系列(07) : 筛选器 Sizzle引擎 - 位置伪类](http://www.cnblogs.com/aaronjs/p/3484545.html)
[jQuery源码分析系列(08) : 选择器 Sizzle引擎 - 高效查询](http://www.cnblogs.com/aaronjs/p/3337531.html)
[jQuery源码分析系列(09) : 回调对象 - Callbacks](http://www.cnblogs.com/aaronjs/p/3342344.html)
[jQuery源码分析系列(10) : 回调对象 - Callback深入](http://www.cnblogs.com/aaronjs/p/3713750.html)
[jQuery源码分析系列(11) : 延时对象 - Deferred概念](http://www.cnblogs.com/aaronjs/p/3348569.html)
[jQuery源码分析系列(12) : 延时对象 - Deferred源码剖析](http://www.cnblogs.com/aaronjs/p/3356505.html)
[jQuery源码分析系列(13) : 数据缓存 - Cache](http://www.cnblogs.com/aaronjs/p/3370176.html)
[jQuery源码分析系列(14) : 回溯魔法 - end和pushStack](http://www.cnblogs.com/aaronjs/p/3387278.html)
[jQuery源码分析系列(15) : 钩子机制 - 属性操作](http://www.cnblogs.com/aaronjs/p/3387906.html)
[jQuery源码分析系列(16) : 深入浏览器兼容 细数jQuery Hooks 属性篇](http://www.cnblogs.com/aaronjs/p/3434830.html)
[jQuery源码分析系列(17) : 事件绑定 - bind/live/delegate/on](http://www.cnblogs.com/aaronjs/p/3440647.html)
[jQuery源码分析系列(18) : 事件绑定 - 体系结构](http://www.cnblogs.com/aaronjs/p/3441320.html)
[jQuery源码分析系列(19) : 事件绑定 - 绑定设计](http://www.cnblogs.com/aaronjs/p/3444874.html)
[jQuery源码分析系列(20) : 事件绑定 - 委托设计](http://www.cnblogs.com/aaronjs/p/3447483.html)
[jQuery源码分析系列(21) : 事件绑定 - 自定义设计](http://www.cnblogs.com/aaronjs/p/3452279.html)
[jQuery源码分析系列(22) : 事件绑定 - 模拟事件](http://www.cnblogs.com/aaronjs/p/3481075.html)
[jQuery源码分析系列(23) : DOM操作核心 - domManip](http://www.cnblogs.com/aaronjs/p/3508190.html)
[jQuery源码分析系列(24) : DOM操作核心 – buildFragment](http://www.cnblogs.com/aaronjs/p/3510768.html)
[jQuery源码分析系列(25) : DOM操作方法（一）](http://www.cnblogs.com/aaronjs/p/3516081.html)
[jQuery源码分析系列(26) : DOM操作方法（二）html,text,val](http://www.cnblogs.com/aaronjs/p/3520383.html)
[jQuery源码分析系列(27) : 样式操作 – CSS](http://www.cnblogs.com/aaronjs/p/3559310.html)
[jQuery源码分析系列(28) : 样式操作 – addClass](http://www.cnblogs.com/aaronjs/p/3433358.html)
[jQuery源码分析系列(29) : 元素操作 – 元素大小](http://www.cnblogs.com/aaronjs/p/3565253.html)
[jQuery源码分析系列(30) : 元素操作 – 窗口尺寸](http://www.cnblogs.com/aaronjs/p/3564562.html)
[jQuery源码分析系列(31) : Ajax - 整体结构](http://www.cnblogs.com/aaronjs/p/3683925.html)
[jQuery源码分析系列(32) : Ajax - deferred实现](http://www.cnblogs.com/aaronjs/p/3713016.html)
[jQuery源码分析系列(33) : Ajax - 前置过滤器和请求分发器](http://www.cnblogs.com/aaronjs/p/3777292.html)
[jQuery源码分析系列(34) : Ajax - 预处理jsonp](http://www.cnblogs.com/aaronjs/p/3781337.html)
[jQuery源码分析系列(35) : Ajax - jsonp的实现与原理](http://www.cnblogs.com/aaronjs/p/3785646.html)
[jQuery源码分析系列(36) : Ajax - 类型转化器](http://www.cnblogs.com/aaronjs/p/3790820.html)
[jQuery源码分析系列(37) : Ajax - 总结](http://www.cnblogs.com/aaronjs/p/3798868.html)
[jQuery源码分析系列(38) : 队列操作](http://www.cnblogs.com/aaronjs/p/3800619.html)
[jQuery源码分析系列(39) : 动画队列](http://www.cnblogs.com/aaronjs/p/3813237.html)
[jQuery源码分析系列(40) : 动画设计](http://www.cnblogs.com/aaronjs/p/4278660.html)
[jQuery源码分析系列(41) : 动画实现](http://www.cnblogs.com/aaronjs/p/4285447.html)

---
jQuery2.1.1 分析碎片
[sizzle分析记录：getAttribute和getAttributeNode](http://www.cnblogs.com/aaronjs/p/3837082.html)
[sizzle分析记录：词法分析器(tokenize)](http://www.cnblogs.com/aaronjs/p/3842352.html)
[sizzle分析记录：关于querySelectorAll兼容问题](http://www.cnblogs.com/aaronjs/p/3847964.html)
[sizzle分析记录：属性选择器](http://www.cnblogs.com/aaronjs/p/3855346.html)
[sizzle分析记录：分解流程](http://www.cnblogs.com/aaronjs/p/3857899.html)
[sizzle分析记录 : 自定义伪类选择器](http://www.cnblogs.com/aaronjs/p/3859884.html)

---
早起的源码分析是2.0.3版本的，本人也是边看边写，对全局的掌控与细节还有欠缺！
目前正在整理最新的2.1.1的源码，本人自己也将实现一遍，之后会作为配套教材，由浅入深提供给大家学习！

如果您看完本篇文章感觉不错，请点击一下右下角的**【****推荐****】**来支持一下博主，谢谢！
如果是原创文章，转载请注明出处！！！
by Aaron：[https://www.cnblogs.com/aaronjs/p/3279314.html](https://www.cnblogs.com/aaronjs/p/3279314.html)

