# maven的pom报plugins却是的解决方法2 - weixin_33985507的博客 - CSDN博客
2015年05月01日 13:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
Failure to transfer org.apache.maven.plugins:maven-plugins:pom:25 from https://repo.maven.apache.org/maven2 was cached in the local repository, resolution will not be reattempted until the update interval of central has elapsed or updates are forced. Original error: Could not transfer artifact org.apache.maven.plugins:maven-plugins:pom:25 from/to central (https://repo.maven.apache.org/maven2): Connection reset pom.xml /omcs-common line 1 Maven Configuration Problem
maven的pom报plugins却是的解决方法.
引用
Failure to transfer org.apache.maven.plugins:maven-surefire-plugin:pom:2.7.1 from http://repo1.maven.org/maven2 was cached in the local repository, resolution will not be reattempted until the update interval of central has elapsed or updates are forced
碰到一个类似以上的错误.
原因是maven的plugin并未下载到本地.
如果你查看
引用
~\.m2\repository\org\apache\maven\plugins\maven-surefire-plugin\2.7.1
的话,会发现里面只有一个
maven-surefire-plugin-2.7.1.pom.lastUpdated
的文件.
而并没有maven-surefire-plugin-2.7.1.jar
解决办法:
1.删除所有以lastUpdated结尾的文件
2.右键点击project -> Maven - Update Dependencies（Update Project...）
如果点击更新后发现
~\.m2\repository\org\apache\maven\plugins\maven-surefire-plugin\2.7.1
下还是没有maven-surefire-plugin-2.7.1.jar
去http://repo1.maven.org/maven2/org/apache/maven/plugins/maven-surefire-plugin/2.7.1/
下载
maven-surefire-plugin-2.7.1.jar maven-surefire-plugin-2.7.1.jar.sha1 maven-surefire-plugin-2.7.1.pom maven-surefire-plugin-2.7.1.pom.sha1
包再重复步骤2.
[http://repo1.maven.org/maven2/org/apache/maven/plugins/maven-surefire-plugin/](http://repo1.maven.org/maven2/org/apache/maven/plugins/maven-surefire-plugin/)
