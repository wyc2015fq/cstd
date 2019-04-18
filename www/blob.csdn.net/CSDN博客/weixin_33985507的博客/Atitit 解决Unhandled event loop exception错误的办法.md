# Atitit 解决Unhandled event loop exception错误的办法 - weixin_33985507的博客 - CSDN博客
2016年10月06日 13:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Atitit 解决[Unhandled
 event loop exception错误的办法](http://blog.csdn.net/rj042/article/details/22851233)
查看workspace/.metadata/.log
org.eclipse.swt.SWTError: No more handles
查看handle,已经7w了...
Tasklist manager,属性,表明句柄数量显示..exploror这个桌面程序使用了1w的handle.走关闭哪..sezo  ok 兰..
原因：：：handle over。。Os bug
作者:: 绰号:老哇的爪子 （全名：：Attilax Akbar Al Rapanui 阿提拉克斯 阿克巴 阿尔 拉帕努伊 ）
汉字名：艾提拉（艾龙），  EMAIL:1466519819@qq.com
转载请注明来源： http://blog.csdn.net/attilax
atiend
