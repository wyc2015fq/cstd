# 使用HttpClient时出错NoClassDefFoundError - Orisun - 博客园







# [使用HttpClient时出错NoClassDefFoundError](https://www.cnblogs.com/zhangchaoyang/articles/2093489.html)





HttpClient的使用依赖于Apache commons项目中的两个组件——commons-logging.jar、commons-codec.jar。所以要在Eclipse中添加外部Jar包

commons-logging.jar和commons-codec.jar。包括

HttpClient.jar已经都在eclipse的plugin里面了，不需要另外下载。

网上普遍讲的NoClassDefFoundError出错的原因是ClassPath没设好。












