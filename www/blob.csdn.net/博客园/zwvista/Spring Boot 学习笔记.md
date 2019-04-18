# Spring Boot 学习笔记 - zwvista - 博客园

## [Spring Boot 学习笔记](https://www.cnblogs.com/zwvista/p/9669821.html)

### Spring Boot

Spring Boot 是当前最流行的后端框架之一。

下面我们用 Spring Boot 快速开发一个显示 'Hello World!' 的网页应用。

### 创建工程

打开 [SPRING INITIALIZR](https://start.spring.io/)，配置应用程序。

工程种类选 Gradle Project

语言选 Kotlin

Spring Boot 版本选 2.0.5

在 Search for dependencies 键入并选中 Web 和 Thymeleaf

也可以切换到 full version 勾选这两项。

点击 Generate Project 生成并下载工程 demo.zip 文件。

### 导入工程

在任意位置解压 demo.zip 文件，生成 demo 文件夹。

打开 Intellij Idea, File / New / Project from Existing Sources... 选择 demo 文件夹。

在 Import Project 向导的第一页选 import project from external model / Gradle

在第二页点击 Finish 导入工程。

### HelloController

在 src/main/kotlin/com/example/demo 文件夹下添加 HelloController.kt 文件，内容如下：

```
package com.example.demo

import org.springframework.stereotype.Controller
import org.springframework.ui.Model
import org.springframework.web.bind.annotation.RequestMapping

@Controller
class HelloController {
    @RequestMapping("/hello")
    internal fun hello(model: Model): String {
        model.addAttribute("hello", "Hello World!")
        return "hello"
    }
}
```

### hello.html

在 src/main/resource/templates 文件夹下添加 hello.html 文件，内容如下：

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
[[${hello}]]
</body>
</html>
```

### 输出结果

运行 DemoApplication 这个 Spring Boot 应用程序，打开 http://localhost:8080/hello 应该可以看到 Hello World! 字样。


