# AndroidStudio导入项目一直卡在Building gradle project info最快速解决方案 - weixin_33985507的博客 - CSDN博客
2016年09月01日 21:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
转自：[http://www.cnblogs.com/baron89/p/4843113.html](https://link.jianshu.com?t=http://www.cnblogs.com/baron89/p/4843113.html)
AndroidStudio导入项目一直卡在Building gradle project info，实际上是因为你导入的这个项目使用的gradle与你已经拥有的gradle版本不一致，导致需要下载该项目需要的gradle版本，不知是被墙了还是什么原因，反正就是会一直卡住，直至下载完成(如果能下载完成的话，233)
网上也提供了方法，就是去官网下载gradle的版本，然后放到本地，我就不在这里介绍了，我的解决方法更简单一些，就是直接修改gradle-wrapper.properties文件，无需去下载gradle
解决方案：
1.随便找一个你能运行的as项目
2.打开gradle-wrapper.properties，文件目录：项目/gradle/wrapper/gradle-wrapper.properties
3.复制distributionUrl这一整行的内容，eg: distributionUrl=[https://services.gradle.org/distributions/gradle-2.4-all.zip](https://link.jianshu.com?t=https://services.gradle.org/distributions/gradle-2.4-all.zip)
4.打开你要导入的项目的gradle-wrapper.properties，具体步骤与步骤2相同
5.把步骤3复制的内容，替换你要导入的项目的gradle-wrapper.properties文件的distributionUrl这一行
6.再重启as，导入项目就可以了
