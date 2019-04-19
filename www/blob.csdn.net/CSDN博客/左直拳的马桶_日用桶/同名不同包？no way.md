# 同名不同包？no way - 左直拳的马桶_日用桶 - CSDN博客
2019年01月30日 19:46:51[左直拳](https://me.csdn.net/leftfist)阅读数：117
spring boot中，两个类，如果名字相同，但位于不同的包，不行的，运行会报错。
这真是咄咄怪事。怎么会这样子？生活也是如此，本来以为已经百尺竿头更进一步了，没想到前面是万丈深渊。
怎么办？
加注解咯。俺家spring boot，最喜欢注解了！
```java
//控制器1
package com.ibm.lanbase.modules.res.controller;
@Controller("resIndexController")//注解，相当于别名？
@RequestMapping(value = "/slider/res")
public class IndexController {
}
//控制器2
package com.ibm.lanbase.modules.tj.controller;
@Controller("tjIndexController")
@RequestMapping(value = "/slider/tj")
public class IndexController {
}
```
附录2.声明Bean的注解
如何吸引Spring容器的注意而“有幸”成为Spring 容器管理的Bean呢？
在Spring Boot中就依靠注解，Spring提供了多个注解来声明Bean为Spring容器管理的Bean，注解不同代表的含义不同，但是对Spring容器来说都是Spring管理的Bean
声明Bean的注解有：
@Component 没有明确角色的组件
@Service 在业务逻辑层（Service层）使用
@Repositpry 在数据访问层（dao层）使用
@Controller 用于标注控制层组件
@RestController
大概，这些bean,后面都可以带上注解的吧？
