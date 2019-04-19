# Failed to read artifact descriptor for org.apache.maven.plugins:maven-resources-plugin:jar:2.7 - xqhrs232的专栏 - CSDN博客
2018年11月30日 23:07:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：164
原文地址::[https://blog.csdn.net/roughman9999/article/details/64467141](https://blog.csdn.net/roughman9999/article/details/64467141)
相关文章
1、解决eclipse载入已有maven工程时出现的fail to read artifact descriptor....问题----[https://blog.csdn.net/manlyzpk/article/details/43475299](https://blog.csdn.net/manlyzpk/article/details/43475299)
2、eclipse加载maven工程提示pom.xml无法解析org.apache.maven.plugins:maven-resources-plugin:2.4.3解决方案----[https://blog.csdn.net/imlmy/article/details/8268293](https://blog.csdn.net/imlmy/article/details/8268293)
新建或导入maven项目失败，并出现如下错误：
Could not calculate build plan: Plugin org.apache.maven.plugins:maven-resources-plugin:2.7 or one of its dependencies could not be resolved: Failed to read artifact descriptor for org.apache.maven.plugins:maven-resources-plugin:jar:2.7
Plugin org.apache.maven.plugins:maven-resources-plugin:2.7 or one of its dependencies could not be resolved: Failed to read artifact descriptor for org.apache.maven.plugins:maven-resources-plugin:jar:2.7
原因可能如下：
1.未对maven进行配置。
Window->Preferences->Maven->User Settings->选择maven安装目录，例如：D:\work\apache-maven-3.0.5\conf\settings.xml
2.maven-resources-plugin版本异常，需要删除后刷新maven配置
右键工程节点，选择 Maven -> Update Project
如果更新失败，可以把(userhome)\repository\org\apache\maven\plugins\maven-resources-plugin\2.7目录删除，然后再次更新。
3.上网方式为代理上网，但是没有对maven设置代理。
修改D:\work\apache-maven-3.0.5\conf\settings.xml
在<proxies></proxies>中间加上：
<proxy>
< id>optional</id>
< active>true</active>
< protocol>http</protocol>
< host>代理的ip地址</host>
< port>端口</port>
< /proxy>
--------------------- 
作者：roughman9999 
来源：CSDN 
原文：https://blog.csdn.net/roughman9999/article/details/64467141 
版权声明：本文为博主原创文章，转载请附上博文链接！
