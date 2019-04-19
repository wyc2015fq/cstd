# 如何将一个普通项目改造成spring boot项目 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月16日 11:52:37[左直拳](https://me.csdn.net/leftfist)阅读数：70
我这种表述正确吗？我也不清楚，反正就是这么个意思。
话说，我从网上拿了个开源的项目来学习和改造。这是个普普通通的JAVA项目，没有用到spring boot，也没有web。我就想将这些都加进去。
怎么加？
我想了一下，有2种途径：
1、新建一个spring boot项目，然后将现在这个项目的代码移植过去
2、直接在目前这个项目里，把spring boot和WEB加进去
途径1难度应该不大，不过没有学习的价值。JAVA世界的缺点，同时也是优点，就是折腾。职业生涯不休，折腾不止。几番折腾下来，理解自然就深刻了，技能自然就长进了。
本次折腾记录凭记忆如下：
**1、改造程序入口main函数**
```java
@SpringBootApplication
public class Application extends SpringBootServletInitializer {
    public static void main(String[] args) {
        SpringApplication.run(Application.class, args);
    }
    /**
     *重写configure
     * @param builder
     * @return
     */
    @Override
    protected SpringApplicationBuilder configure(SpringApplicationBuilder builder) {
        return builder.sources(Application.class);
    }
}
```
**2、改造pom.xml（或build.gradle）**
参照以下例子，加入相关内容。注意别漏了头部区域的
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <!-- parent务必要加上 -->
  <parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>1.5.6.RELEASE</version>
    <relativePath/> <!-- lookup parent from repository -->
  </parent>
  <groupId>cc.eguid.ffch4j</groupId>
  <artifactId>ffch4j</artifactId>
  <version>18.12.02-SNAPSHOT</version>
  <properties>
    <java.version>1.8</java.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-test</artifactId>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.assertj</groupId>
      <artifactId>assertj-core</artifactId>
      <version>2.6.0</version>
      <scope>compile</scope>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-web</artifactId>
    </dependency>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-web</artifactId>
      <version>4.3.11.RELEASE</version>
    </dependency>
    <dependency>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-starter-thymeleaf</artifactId>
    </dependency>
  </dependencies>
  <build>
    <plugins>
      <plugin>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-maven-plugin</artifactId>
      </plugin>
    </plugins>
  </build>
</project>
```
**3、自己手动加入src\java\resources等东东**
这大概是一种约定吧。
完毕。不行的话，自己再调调，折腾一下。
现在，我接触JAVA已经有一段时间了。其实JAVA有狭义和广义之分。狭义的JAVA就是JAVA本尊，单纯的JAVA这种语言；而广义上的JAVA，是指那些个框架、工具和其他生态，林林总总。广义上的JAVA其实与JAVA本身并没有多大关系，只不过是打着JAVA的旗号而已。所谓的JAVA难学，就难在这些框架、工具上，单纯的JAVA，只不过是一种语言而已，有什么难不难的。
为什么这些框架、工具会让人感到困难重重？原因是，它背后是一整套思想。你要用这个框架，就要接受它的条条框框，以及框架设计者的意图、思想，心甘情愿地被它洗脑。这其中，会有一个痛苦的过程。
真是讽刺，框架和工具，本意应该是方便开发，但恰恰造成了学习成本高昂，和开发中的困惑。正如有什么充血模型，贫血模型，最后又要搞个POJO一样，简单可能就是最好的。
看到一篇有点相关的文章：
[停止学习框架](https://zhuanlan.zhihu.com/p/52814937)
