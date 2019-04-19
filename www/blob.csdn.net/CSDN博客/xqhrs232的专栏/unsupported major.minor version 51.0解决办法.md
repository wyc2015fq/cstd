# unsupported major.minor version 51.0解决办法 - xqhrs232的专栏 - CSDN博客
2018年10月19日 14:41:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：197
原文地址::[https://blog.csdn.net/rogerjava/article/details/53020300](https://blog.csdn.net/rogerjava/article/details/53020300)
今天偶然间遇到一个问题，也加深了自己对eclipse中build path和java compiler compliance level的理解。
解决Unsupported major.minor version 51.0错误
最近新安装使用了jdk7，编译了一些类替换到原来正常运行的项目中，替换之后发生了Unsupported major.minor version 51.0错误。经过网上搜索发现了问题产生的原因：用jdk7编译的class文件放到基于jdk6运行在tomcat之中，就会报这个错。 
解决起来也很方便：打开exclipse中项目上的属性—java compiler–选择一个合适的版本后重新编译即可。 
具体步骤 
解决：项目——>右键——>属性——>Java Compiler——>Compiler Compliance Level——>选择你使用的JDK版本——>应用。
总结：不同的JDK版本使用的major.minor不同，所以会导致这个错误。在项目中要使用当前电脑配置的JDK版本，切忌张冠李戴。
知识扩充：major.minor version，它相当于一个软件的主次版本号，只是在这里是标识的一个Java Class的主版本号和次版本号。
