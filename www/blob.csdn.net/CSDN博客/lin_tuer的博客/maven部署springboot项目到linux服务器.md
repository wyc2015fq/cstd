
# maven部署springboot项目到linux服务器 - lin_tuer的博客 - CSDN博客


2017年12月27日 20:15:00[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：1336


假设我们已经有了一下东西
1. maven已经安装
2. springboot项目已经建立好了，并且在本地可以正常运行
3. linux服务器上面也已经有了相应的环境，java，mysql等等
接下来，我们就可以把项目迁移到服务器发布了
# 通过maven打包项目，生成jar文件
首先，要在我们的pom.xml文件中添加如下内容：
`<``build``>``<``plugins``>``<``plugin``>``<``groupId``>``org.springframework.boot``</``groupId``>``<``artifactId``>``spring-boot-maven-plugin``</``artifactId``>``<``executions``>``<``execution``>``<``goals``>``<``goal``>``repackage``</``goal``>``</``goals``>``</``execution``>``</``executions``>``</``plugin``>``<``plugin``>``<``groupId``>``org.apache.maven.plugins``</``groupId``>``<``artifactId``>``maven-jar-plugin``</``artifactId``>``<``configuration``>``<``archive``>``<``manifest``>``<``addClasspath``>``true``</``addClasspath``>``<``mainClass``>``com.dazhi.Application``</``mainClass``>``</``manifest``>``</``archive``>``</``configuration``>``</``plugin``>``</``plugins``>``</``build``>`这是用来打包项目的配置，然后在项目的根目录执行：
`mvn``package`你会发现 target/ 文件夹下面生成了一个.jar文件
![这里写图片描述](https://img-blog.csdn.net/20171227195332706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGluX3R1ZXI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个jar包的名称和我们在pom.xml中配置的是一样的
---上传到服务器

---紧接着，把上述jar包上传到服务器，有很多种方法，最简单的就是用工具，我用的mobaxterm,

---直接找到执行目录，将文件上传，可视化操作，不再赘述。

---在服务器运行项目

---到达jar包放置的位置，执行：

---java -jar jar包名字

---.jar

---去浏览器访问一下，发现已经可以啦

---在服务器后台运行项目并输出日志到指定文件

---上面的运行方式，直接在前端窗口执行，窗口关了，服务就停了，显然不太满足我们的要求，且其日志也直接输出到了前台，不方面后期处理查看

---我们通过以下命令可以将项目后台运行，且输出日志到指定文件

---nohup java -jar /usr/webapp/web

---.jar

--->/logs/web

---.log

---&

---那么如果我们想停止该服务呢？

---job

---job命令可以查看后台运行的进程，看到进行的代号后，通过

---kill

---%

---num

---可以把代号为 num 的进程停止~，就 ok 啦

---把固定命令写到shell脚本，避免重复性输入

---为了方便每次的更新部署，我们新建一个.sh文件，保存我们的命令，每次需要时，运行该shell脚本就好啦

---依次执行如下命令

---touch execute

---.sh

---vim execute

---.sh

---在 execute.sh 写入要执行的命令,保存退出后执行

---source

---execute.sh

---OK 啦


