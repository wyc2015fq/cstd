
# maven 细节 —— scope、坐标 - Zhang's Wikipedia - CSDN博客


2018年05月31日 23:25:01[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：455


对于 idea 开发环境，测试代码便是在 src/test/java（该java目录会在创建时标注为测试文件夹） 目录下的 .java 代码为测试代码：

![这里写图片描述](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 1. scope
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)scope的分类
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)compile：默认值他表示被依赖项目需要参与当前项目的编译，还有后续的测试，运行周期也参与其中，是一个比较强的依赖。打包的时候通常需要包含进去
test：依赖项目仅仅参与测试相关的工作，包括测试代码的编译和执行，不会被打包，例如：junit
<dependency><groupId>junit</groupId><artifactId>junit</artifactId><version>4.11</version><scope>test</scope></dependency>必须在测试代码（测试目录下，src/test/java）中才会识别 org.junit.* 下的类和成员；
runtime：表示被依赖项目无需参与项目的编译，不过后期的测试和运行周期需要其参与。与compile相比，跳过了编译而已。例如JDBC驱动，适用运行和测试阶段
provided：打包的时候可以不用包进去，别的设施会提供。事实上该依赖理论上可以参与编译，测试，运行等周期。相当于compile，但是打包阶段做了exclude操作
system：从参与度来说，和provided相同，不过被依赖项不会从maven仓库下载，而是从本地文件系统拿。需要添加systemPath的属性来定义路径
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2. 坐标：groupid 与 artifactid
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)groupid和artifactId被统称为“坐标”是为了保证项目唯一性而提出的，如果你要把你项目弄到**maven 本地仓库**去，你想要找到你的项目就必须根据这两个id去查找。
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)groupId一般分为多个段，这里只说两段，第一段为域，第二段为公司名称。域又分为 org、com、cn 等等许多，其中org为非营利组织，com为商业组织。举个apache公司的tomcat项目例子：这个项目的groupId是org.apache，它的域是org（因为tomcat是非营利项目），公司名称是apache，artigactId是tomcat。
比如我创建一个项目，我一般会将groupId设置为cn.snowin，cn表示域为中国，snowin是我个人姓名缩写，artifactId设置为testProj，表示你这个项目的名称是testProj，依照这个设置，你的包结构最好是cn.snowin.testProj打头的，如果有个StudentDao，它的全路径就是cn.snowin.testProj.dao.StudentDao
[](https://img-blog.csdn.net/20180531232317697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

