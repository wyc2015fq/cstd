# Maven学习六之利用mvn deploy命令上传包 - z69183787的专栏 - CSDN博客
2016年05月17日 11:02:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1945
mvn:deploy在整合或者发布环境下执行，将最终版本的包拷贝到远程的repository，使得其他的开发者或者工程可以共享。
以将ojdbc14传到nexus中的thirdparty为例
**一 配置settings.xml**
因为nexus是需要登陆操作，当然可以通过配置免登陆，这是后话。
在settings.xml的<servers></servers>
<server>   
<id>**thirdparty**</id>   
<username>admin</username>
<password>admin123</password>   
</server>
当然如果你要上传包去其他仓库，可依照此例，如
<server>   
<id>**central**</id>   
<username>admin</username>   
<password>admin123</password>   
</server>
如果进行deploy时返回Return code is: 401错误，则需要进行用户验证或者你已经验证的信息有误。
**二 cmd输入命令**
mvn deploy:deploy-file -DgroupId=com.xy.oracle -DartifactId=ojdbc14 -Dversion=10.2.0.4.0 -Dpackaging=jar -Dfile=E:\ojdbc14.jar -Durl=http://localhost:9090/nexus-2.2-01/content/repositories/thirdparty/ -DrepositoryId=**thirdparty**
DgroupId和DartifactId构成了该jar包在pom.xml的坐标，项目就是依靠这两个属性定位。自己起名字也行。
Dfile表示需要上传的jar包的绝对路径。
Durl私服上仓库的位置，打开nexus——>repositories菜单，可以看到该路径。
DrepositoryId服务器的表示id，在nexus的configuration可以看到。
Dversion表示版本信息，**怎样得到一个jar包准确的版本呢？**
解压该包，会发现一个叫MANIFEST.MF的文件，这个文件就有描述该包的版本信息。
比如Manifest-Version: 1.0可以知道该包的版本了。
上传成功后，在nexus界面点击3rd party仓库可以看到这包。
**三 一些问题**
**发现问题**
在敲击该命令的时候，有时候看到提示需要POM文件，但上传包是不需要pom文件的。
**可能原因**
最大可能是你语句打错了如多了一个空格和换行，这样语句直接截断到换行前面，cmd就找它可以认识的语句执行，比如直接执行
mvn，而mvn是对项目打包，是要pom文件的。
**解决办法**
耐心把命令重新敲一遍。
