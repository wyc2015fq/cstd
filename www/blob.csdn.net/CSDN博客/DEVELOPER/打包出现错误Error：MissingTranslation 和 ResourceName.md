# 打包出现错误Error：MissingTranslation 和 ResourceName - DEVELOPER - CSDN博客





2017年07月25日 12:57:21[学术袁](https://me.csdn.net/u012827205)阅读数：1494








今天安卓应用打包的时候出现了俩个报错的问题，在这里记录总结一下：

问题一：


``Error:(5) Error: "xxx" is not translated in "en" (English), "es" (Spanish) [MissingTranslation]


![](https://img-blog.csdn.net/20170725113833043)


像这种类型的问题：Error:(5) Error: "hello_blank_fragment" is not translated in "en" (English), "es" (Spanish) [MissingTranslation]

解决方案就是在 ./values/strings 文件中的根节点<resource>中添加：


xmlns:tools="http://schemas.android.com/tools" tools:ignore="MissingTranslation"
问题二：




Error:Resource named 'xxx' does not start with the project's resource prefix 'xxx'; rename to 'xxx'?[ResourceName]像这种类型的问题：Error:Resource named 'activity_img_sel' does not start with the project's resource prefix 'imgsel'; rename to 'imgselActivity_img_sel'?[ResourceName]

![](https://img-blog.csdn.net/20170725125434278)


像这种类型的问题：Error:Resource named 'activity_img_sel' does not start with the project's resource prefix 'imgsel'; rename to 'imgselActivity_img_sel'?[ResourceName]


解决方案：![](https://img-blog.csdn.net/20170725125337031)







