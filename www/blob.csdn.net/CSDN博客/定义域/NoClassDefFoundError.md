# NoClassDefFoundError - 定义域 - CSDN博客
2015年09月09日 16:42:19[定义域](https://me.csdn.net/u010278882)阅读数：269标签：[exception](https://so.csdn.net/so/search/s.do?q=exception&t=blog)
个人分类：[API](https://blog.csdn.net/u010278882/article/category/5802859)
## 意义
ClassNotFoundExcpection是编译时虚拟机找不到该类 
NoClassDefFoundError是运行时虚拟机找不到该类
## 原因
出现第一种错误是因为项目中没有引入相关的jar文件 
出现第二种错误是因为项目的导出（生产文件）中在构建时没有导出相关jar文件
## 解决
解决第一种错误，只要在eclipse项目中导入相关jar到项目中即可（如下图1） 
解决第二种错误，也需要配置（如下图2）
![图1](https://img-blog.csdn.net/20150909154304081)
通过Add JARs将项目中的jar包添加 
通过Add External JARs将项目外的jar包添加
![图2](https://img-blog.csdn.net/20150909154441813)
> 
这里列出项目中编译时使用到的所有jar，如要导出某个jar到生产物中，则选中相应的文件即可。
