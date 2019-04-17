# Jib为Java应用构建容器镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年08月02日 11:31:47[boonya](https://me.csdn.net/boonya)阅读数：532








文章来自:[https://www.oschina.net/p/jib](https://www.oschina.net/p/jib)

Jib 是 Google 开发的可以直接构建 Java 应用的 Docker 和 OCI 镜像的类库，以 Maven 和 Gradle 插件形式提供。

通过 Jib，Java 开发者可以使用他们熟悉的 Java 工具来构建容器。Jib 是一个快速而简单的容器镜像构建工具，它负责处理将应用程序打包到容器镜像中所需的所有步骤。它不需要你编写 Dockerfile 或安装 Docker，而且可以直接集成到 Maven 和 Gradle中 —— 只需要将插件添加到构建中，就可以立即将 Java 应用程序容器化。

**Docker 构建流程**

![](https://static.oschina.net/uploads/space/2018/0710/155927_vHQt_2720166.png)

**Jib 构建流程**

![](https://static.oschina.net/uploads/space/2018/0710/155918_xYRX_2720166.png)

**目标**
- 
快速 — 快速部署您的更改。Jib 将你的应用程序分成多个层，从类中分离依赖项。你不必等待 Docker 重建整个 Java 应用程序 —— 只需部署更改的层即可

- 
可重现 — 使用相同内容重建容器镜像

- 
Daemonless — 减少 CLI 依赖性。从 Maven 或 Gradle 中构建 Docker 镜像，然后推送到你选择的任何注册表，无需编写 Dockerfiles 并调用 docker build/push


在 Maven 中使用 Jib：

```
<plugin>   
 <groupId>com.google.cloud.tools</groupId>  
 <artifactId>jib-maven-plugin</artifactId>  
 <version>0.9.0</version>  
 <configuration>     
   <to>       
     <image>gcr.io/my-project/image-built-with-jib</image>   
   </to> 
 </configuration> 
</plugin>
```

```
# Builds to a container image registry. 
$ mvn compile jib:build 
# Builds to a Docker daemon. 
$ mvn compile jib:dockerBuild
```

在 Gradle 中使用 Jib：

```
plugins {
  id 'com.google.cloud.tools.jib' version '0.9.0'
}
jib.to.image = 'gcr.io/my-project/image-built-with-jib'

# Builds to a container image registry.
$ gradle jib
# Builds to a Docker daemon.
$ gradle jibDockerBuil
```

Gitee ：[https://gitee.com/mirrors/Jib](https://gitee.com/mirrors/Jib)



