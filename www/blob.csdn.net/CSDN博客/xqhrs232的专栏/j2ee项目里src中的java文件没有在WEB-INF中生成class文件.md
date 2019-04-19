# j2ee项目里src中的java文件没有在WEB-INF中生成class文件 - xqhrs232的专栏 - CSDN博客
2018年10月19日 14:58:41[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：39
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://zhidao.baidu.com/question/76376879.html](https://zhidao.baidu.com/question/76376879.html)
无法在WEB-INF中生成class文件是myeclipse无法自动编译导致的。
　　解决方法如下：
　　1. 先确认下 project->build automatically是否勾选上
　　2. 如果选上了,还不能class文件, 就手动编译:选中工程，然后 project->clean....->选第2个Clean projects selected below, 选择OK。
　　3.如果以上2步都无法编译出class文件，那就是项目设置出问题了。project->properties->java build path->source->src，将Default output folder设置为[项目名]/WebRoot/WEB-INF/classes,点击OK。
　　4.如果以上3步依然无法解决问题，首先请检查项目是否报错，以及引用的jar包是否路径都正确。jar路径不对的话，project->properties->java build path->Libraries,将jar路径不正确的引用remove了，然后Add Extenal JARs，重新添加jar包。
　　以上几个步骤，就能够解决无法编译出class文件的问题。
