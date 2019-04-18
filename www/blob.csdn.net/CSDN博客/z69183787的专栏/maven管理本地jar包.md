# maven管理本地jar包 - z69183787的专栏 - CSDN博客
2016年05月20日 14:55:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：633
# systemPath方式
有些不通用的包，maven仓库没有，只能通过本地包依赖，就像下面方式：
在需要依赖的项目建lib文件夹，如下：
![](https://img-blog.csdn.net/20140423170936609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3pwMTEyMTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在pom.xml项目管理文件里面加入本地依赖，如下
![](https://img-blog.csdn.net/20140423171150562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3pwMTEyMTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这种情况可以解决问题，但是有些时候会产生莫名的问题，比如项目开发过程中会出现项目虽然打包成功(mvn clean install –Dmaven.test.skip=true)，但是运行过程中会报包依赖错误。
# 本地jar安装至maven仓库
一般不建议通过这种方式配置依赖，通常做法建议你把本地包安装到maven仓库，命令如下：
mvn install:install-file-DgroupId=com.httpmime -DartifactId=httpmime4.0 -Dversion=1.0 -Dpackaging=jar-Dfile=httpmime-4.0.jar
运行命令后，httpmime-4.0.jar已经加入到本地仓库了
![](https://img-blog.csdn.net/20140423171219343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3pwMTEyMTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后把依赖配置里的<systemPath>以及<scope>移出掉，就可以了。
# Maven生产的包copy至父项目lib
上面虽然把本地jar包安装至本地仓库了，但是因为你本地仓库其他开发人员依然不能引用，导致他人依赖出现问题。这个时候需要把安装本地的jar包按照maven仓库的路径原封不动的copy一份父项目(打包类型为pom)的文件夹lib里。具体见下图：
![](https://img-blog.csdn.net/20140423171302703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3pwMTEyMTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样其他开发人员下载后，运行构建命令，这些包会自动生成至他本地的仓库。
