# 注解和注解处理器 Part 2 - 定义域 - CSDN博客
2016年03月03日 17:12:52[定义域](https://me.csdn.net/u010278882)阅读数：214
个人分类：[原理](https://blog.csdn.net/u010278882/article/category/6107617)
## 注解处理器示例
> 
来自《Java编程思想（第4版）》
- 用例跟踪
- 数据库表生成
- Java 代码生成
用例跟踪使用注解来描述用例的完成情况。 
1. 定义描述用例（完成情况）的注解 `UseCase`
2. 定义用例类`PasswordUtil`并注解 
3. 定义注解处理器`UseCaseTracker`
执行方式：Main方法，通过反射获取注解的类/类成员的信息。
数据库表生成使用注解描述数据库表。 
1. 定义描述数据库表的注解 
    1. 描述数据库表 `DBTable`
    2. 描述数据库约束 `Constraints`
    3. 描述数据库字段类型 `SQLString`
2. 定义实体类`Member`并注解 
3. 定义注解处理器`TableCreator`
执行方式：Main方法，通过反射获取注解的类/类成员的信息。
Java 代码生成示例将类提取为接口（提取 public 方法）。 
1. 定义描述接口的注解`InterfaceExtractor`
2. 定义待提取类`Multiplier`并注解 
3. 定义注解处理器`InterfaceExtractorProcessor`
4. 定义注解处理器工厂类`InterfaceExtractorProcessorFactory`
执行方式：apt 扫描注解并查找对应的注解处理器，注解处理器要通过注解处理器工厂注册给 apt，apt 提供执行环境给处理器访问环境资源。内部原理不明（因为没有执行编译被注解的类，所以不是通过反射进行的处理）。
## API 支持
- apt + com.sun.mirror API：JDK 5 引入注解，同时配备该API进行注解处理; JDK 7 中不再支持。
- javac + javax.annotation.processing API：JDK 6 对注解处理进行了标准化，apt 工具被集成到 javac，原先的 API（tools.jar，不属于 Java 标准API）也被标准化。
> 
IDE 对注解处理的支持
`eclipse`[支持注解处理](https://www.eclipse.org/jdt/apt/introToAPT.php)，但是对注解处理器的调试仅能通过将项目作为插件的形式进行。`Android Studio` 默认不支持，需要引入外部依赖进行。
