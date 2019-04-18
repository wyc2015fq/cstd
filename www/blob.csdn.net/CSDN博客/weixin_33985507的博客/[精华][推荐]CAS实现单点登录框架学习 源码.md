# [精华][推荐]CAS实现单点登录框架学习 源码 - weixin_33985507的博客 - CSDN博客
2018年02月28日 11:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
**1.通过下载稳定版本的方式下载cas的相关源码包，如下：**
![8560519-cb962c11420de8e5..png](https://upload-images.jianshu.io/upload_images/8560519-cb962c11420de8e5..png)
直接选择4.2.1的稳定代码即可
**2.我们项目中的版本版本使用maven apereo远程库去下载**
通过远程maven库下载cas-server-webapp的war包，步骤如下：
**(一)访问Maven地址： http://mvnrepository.com/，并搜索cas-server-webapp**
![8560519-6e9cea1862230116..png](https://upload-images.jianshu.io/upload_images/8560519-6e9cea1862230116..png)
**(二)点击cas-server-webapp链接地址，下载最新的版本**
![8560519-71fabc6e1db1a757..png](https://upload-images.jianshu.io/upload_images/8560519-71fabc6e1db1a757..png)
![8560519-c2c42e8ada9aaa47..png](https://upload-images.jianshu.io/upload_images/8560519-c2c42e8ada9aaa47..png)
**3.下载cas-client-core的jar包，如下：**
![8560519-803a2055e5955c73..png](https://upload-images.jianshu.io/upload_images/8560519-803a2055e5955c73..png)
4**.CAS document文档，cas使用spring mvc开发，支持的协议，支持的开发语言等**
CAS的架构图：
![8560519-25ae2c50ca9e38e3..png](https://upload-images.jianshu.io/upload_images/8560519-25ae2c50ca9e38e3..png)
![8560519-4c5119ea805c2bb8..png](https://upload-images.jianshu.io/upload_images/8560519-4c5119ea805c2bb8..png)
Cas的安装要求（我们项目实例使用server版本4.2.7，clent版本是3.3）：
![8560519-1973a1c60bce272d..png](https://upload-images.jianshu.io/upload_images/8560519-1973a1c60bce272d..png)
![8560519-c32f73983057607b..png](https://upload-images.jianshu.io/upload_images/8560519-c32f73983057607b..png)
这是通过sso cas做的详细教程，里面有很多的文档和源码，源码可以直接运行查看效果。
![8560519-e86ea5ef05522046..jpg](https://upload-images.jianshu.io/upload_images/8560519-e86ea5ef05522046..jpg)
[资料和源码来源](https://link.jianshu.com?t=https%3A%2F%2Fitem.taobao.com%2Fitem.htm%3Fspm%3Da230r.1.14.1.31ca6413OLAQJx%26id%3D564986008305%26ns%3D1%26abbucket%3D6%23detail)
