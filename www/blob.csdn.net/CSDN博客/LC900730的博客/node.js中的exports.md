# node.js中的exports - LC900730的博客 - CSDN博客
2017年08月26日 19:29:10[lc900730](https://me.csdn.net/LC900730)阅读数：146
## 区别
1.module.exports初始值为一个空对象{}; 
2.exports是指向module.exports的引用； 
3.require返回的是module.exports而不是exports。 
可以常见： 
exports=module.exports=something; 
原理很简单：当module.exports指向新的对象时候，exports断开了与module.exports的引用。 
exports是引用module.exports的值。module.exports被改变的时候，exports不会被改变，而模块导出的时候，真正导出的是执行是module.exports而不是exports。
