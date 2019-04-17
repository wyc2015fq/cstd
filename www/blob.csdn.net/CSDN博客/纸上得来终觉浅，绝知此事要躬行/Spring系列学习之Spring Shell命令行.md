# Spring系列学习之Spring Shell命令行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:31:30[boonya](https://me.csdn.net/boonya)阅读数：899








英文原文：[https://projects.spring.io/spring-shell/](https://projects.spring.io/spring-shell/)

**目录**

[介绍](#%E4%BB%8B%E7%BB%8D)

[特性](#%E7%89%B9%E6%80%A7)

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[示例和相关项目](#samples--related-projects)

Spring Shell项目提供了一个交互式shell，允许您使用基于Spring的编程模型插入自己的自定义命令。



## 介绍

Spring Shell项目的用户可以通过依赖Spring Shell jar并添加他们自己的命令（作为spring bean上的方法）轻松构建一个功能齐全的shell（也就是命令行）应用程序。创建命令行应用程序可能是有用的，例如与项目的REST API交互，或使用本地文件内容。

## 特性

Spring Shell的功能包括：
-     一个简单的，注释驱动的编程模型，用于提供自定义命令
-     使用Spring Boot自动配置功能作为命令插件策略的基础
-     选项卡完成，着色和脚本执行
-     自定义命令提示符，shell历史文件名，结果和错误的处理
-     基于域特定标准动态启用命令
-     与bean验证API集成
-     已经内置命令，如清晰的屏幕，华丽的帮助，退出
-     ASCII艺术表，带格式，对齐，花式边框等。

## 快速开始

在项目中使用spring-shell的推荐方法是使用依赖关系管理系统 - 下面的代码片段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。(可导航到英文页面选择对应的版本和依赖方式)

Maven

```
<dependencies>
    <dependency>
        <groupId>org.springframework.shell</groupId>
        <artifactId>spring-shell-starter</artifactId>
        <version>2.0.0.RELEASE</version>
    </dependency>
</dependencies>
```

Gradle

```
dependencies {
    compile 'org.springframework.shell:spring-shell-starter:2.0.0.RELEASE'
}
```

然后创建一个可以作为调用的简单命令

```
shell:>translate "hello world!" --from en_US --to fr_FR
bonjour monde!
```

假设您可以访问某些与Locales一起使用的翻译服务：

```java
package foo;

@ShellComponent
public class TranslationCommands {

    private final TranslationService service;

    @Autowired
    public TranslationCommands(TranslationService service) {
      this.service = service;
    }

    @ShellMethod"Translate text from one language to another.")
    public String translate(
      @ShellOption(mandatory = true) String text,
      @ShellOption(mandatory = true, defaultValue = "en_US") Locale from,
      @ShellOption(mandatory = true) Locate to
    ) {
      // invoke service
      return service.translate(text, from, to);
    }
}
```

Spring Shell

Release

Documentation

2.0.1

[Reference](http://docs.spring.io/spring-shell/docs/2.0.1.BUILD-SNAPSHOT/reference/htmlsingle)[API](http://docs.spring.io/spring-shell/docs/2.0.1.BUILD-SNAPSHOT/api/)

2.0.0

[Reference](http://docs.spring.io/spring-shell/docs/2.0.0.RELEASE/reference/htmlsingle)[API](http://docs.spring.io/spring-shell/docs/2.0.0.RELEASE/api/)

## 示例和相关项目
- [Spring Shell Samples](https://github.com/spring-projects/spring-shell/tree/master/spring-shell-samples)
- [Spring Cloud Skipper Shell](https://github.com/spring-cloud/spring-cloud-skipper/tree/master/spring-cloud-skipper-shell-commands)



