# Docker maven插件方式构建微服务镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月05日 18:10:00[boonya](https://me.csdn.net/boonya)阅读数：838








Docker maven插件方式构建微服务镜像有两种方式：a.maven集成构建docker镜像  b.外部Dockerfile构建docker镜像。本文介绍的docker插件只适用于微服务jar执行的方式，对于war的构建问题，后面再加以验证补充。插件的Github地址：[https://github.com/spotify/docker-maven-plugin](https://github.com/spotify/docker-maven-plugin)

## 方式一：直接在Maven构建服务镜像



```
<build>
  <plugins>
    ...
    <plugin>
      <groupId>com.spotify</groupId>
      <artifactId>docker-maven-plugin</artifactId>
      <version>VERSION GOES HERE</version>
      <configuration>
        <imageName>example</imageName>
        <baseImage>java</baseImage>
        <entryPoint>["java", "-jar", "/${project.build.finalName}.jar"]</entryPoint>
        <!-- copy the service's jar file from target into the root directory of the image --> 
        <resources>
           <resource>
             <targetPath>/</targetPath>
             <directory>${project.build.directory}</directory>
             <include>${project.build.finalName}.jar</include>
           </resource>
        </resources>
      </configuration>
    </plugin>
    ...
  </plugins>
</build>
```
注：VERSION GOES HERE改为最新的版本即可。


## 方式二：外部Dockerfile构建服务镜像



```
<build>
  <plugins>
    ...
    <plugin>
      <groupId>com.spotify</groupId>
      <artifactId>docker-maven-plugin</artifactId>
      <version>VERSION GOES HERE</version>
      <configuration>
        <imageName>example</imageName>
        <dockerDirectory>docker</dockerDirectory>
        <resources>
           <resource>
             <targetPath>/</targetPath>
             <directory>${project.build.directory}</directory>
             <include>${project.build.finalName}.jar</include>
           </resource>
        </resources>
      </configuration>
    </plugin>
    ...
  </plugins>
</build>
```




## Docker 微服务插件构建实例




请参考：   [使用Maven插件构建Docker镜像](https://github.com/eacdy/spring-cloud-book/blob/master/3%20%E4%BD%BF%E7%94%A8Docker%E6%9E%84%E5%BB%BA%E5%BE%AE%E6%9C%8D%E5%8A%A1/3.7%20%E4%BD%BF%E7%94%A8Maven%E6%8F%92%E4%BB%B6%E6%9E%84%E5%BB%BADocker%E9%95%9C%E5%83%8F.md)





在用惯了Tomcat的人看来，是否也可以通过此插件实现呢，这是一个疑问，插件的开发者并没有提及，实际开发和部署过程中也有此要求。请参考问题#238：

[Can some one provide a working example for building images in dockerhub using this plugin to deploy war on tomact](https://github.com/spotify/docker-maven-plugin/issues/238)


此外也可以顺便看看阿里云栖社区的文章：[用 Docker 构建、运行、发布一个 Spring Boot 应用](https://yq.aliyun.com/articles/47344)



更多官方详情请查阅Github：[https://github.com/spotify/docker-maven-plugin](https://github.com/spotify/docker-maven-plugin)













