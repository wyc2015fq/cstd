# Spring系列学习之Spring Statemachine状态机 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 22:42:54[boonya](https://me.csdn.net/boonya)阅读数：327








英文原文：[https://projects.spring.io/spring-statemachine/](https://projects.spring.io/spring-statemachine/)

**目录**

[快速开始](#%E5%BF%AB%E9%80%9F%E5%BC%80%E5%A7%8B)

[Builder](#builder)

[JavaConfig](#javaconfig)

[版本](#%E7%89%88%E6%9C%AC)

[资源](#%E8%B5%84%E6%BA%90)

Spring Statemachine是应用程序开发人员在Spring应用程序中使用状态机概念的框架。

Spring Statemachine旨在提供以下功能：
-     易于使用的扁平单级状态机，用于简单的使用案例。
-     分层状态机结构，以简化复杂的状态配置。
-     状态机区域提供更复杂的状态配置。
-     使用触发器，转换，警卫和操作。
-     键入安全配置适配器。
-     生成器模式，用于在Spring Application上下文之外使用的简单实例化
-     通常用例的食谱
-     基于Zookeeper的分布式状态机
-     状态机事件监听器。
-     UML Eclipse Papyrus建模。
-     将计算机配置存储在永久存储中。
-     Spring IOC集成将bean与状态机关联起来。


状态机功能强大，因为行为始终保证一致，使调试相对容易。这是因为操作规则是在机器启动时写成的。这个想法是你的应用程序可能存在于有限数量的状态中，某些预定义的触发器可以将你的应用程序从一个状态转移到另一个状态。此类触发器可以基于事件或计时器。


在应用程序之外定义高级逻辑然后依靠状态机来管理状态要容易得多。您可以通过发送事件，侦听更改或仅请求当前状态来与状态机进行交互。


你想问一个问题吗？转到[StackOverflow](https://stackoverflow.com/)并使用标签[spring-statemachine](https://stackoverflow.com/questions/tagged/spring-statemachine)或[Gitter](https://gitter.im/spring-projects/spring-statemachine)。

## 快速开始

在项目中使用spring-statemachine的推荐方法是使用依赖关系管理系统 - 下面的代码片段可以复制并粘贴到您的构建中。 需要帮忙？ 请参阅我们的Maven和Gradle构建入门指南。(导航到英文页面可以选择版本和依赖方式)

Maven

```
<dependencies>
    <dependency>
        <groupId>org.springframework.statemachine</groupId>
        <artifactId>spring-statemachine-core</artifactId>
        <version>2.0.3.RELEASE</version>
    </dependency>
</dependencies>
```

Gradle

```
dependencies {
    compile 'org.springframework.statemachine:spring-statemachine-core:2.0.3.RELEASE'
}
```

以下示例应该了解如何配置和使用状态机。 假设我们有状态STATE1，STATE2和事件EVENT1，EVENT2。

![](https://img-blog.csdnimg.cn/2018122222393690.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

```
static enum States {
    STATE1, STATE2
}

static enum Events {
    EVENT1, EVENT2
}
```



## Builder

```
public StateMachine<States, Events> buildMachine() throws Exception {
    Builder<States, Events> builder = StateMachineBuilder.builder();

    builder.configureStates()
        .withStates()
            .initial(States.STATE1)
            .states(EnumSet.allOf(States.class));

    builder.configureTransitions()
        .withExternal()
            .source(States.STATE1).target(States.STATE2)
            .event(Events.EVENT1)
            .and()
        .withExternal()
            .source(States.STATE2).target(States.STATE1)
            .event(Events.EVENT2);

    return builder.build();
}
```

```
StateMachine<States, Events> stateMachine = buildMachine();
stateMachine.start();
stateMachine.sendEvent(Events.EVENT1);
stateMachine.sendEvent(Events.EVENT2);
```

## JavaConfig

```
@Configuration
@EnableStateMachine
static class Config1 extends EnumStateMachineConfigurerAdapter<States, Events> {

    @Override
    public void configure(StateMachineStateConfigurer<States, Events> states)
            throws Exception {
        states
            .withStates()
                .initial(States.STATE1)
                .states(EnumSet.allOf(States.class));
    }

    @Override
    public void configure(StateMachineTransitionConfigurer<States, Events> transitions)
            throws Exception {
        transitions
            .withExternal()
                .source(States.STATE1).target(States.STATE2)
                .event(Events.EVENT1)
                .and()
            .withExternal()
                .source(States.STATE2).target(States.STATE1)
                .event(Events.EVENT2);
    }
}
```

```
@WithStateMachine
static class MyBean {

    @OnTransition(target = "STATE1")
    void toState1() {
    }

    @OnTransition(target = "STATE2")
    void toState2() {
    }
}
```

```
static class MyApp {

    @Autowired
    StateMachine<States, Events> stateMachine;

    void doSignals() {
        stateMachine.start();
        stateMachine.sendEvent(Events.EVENT1);
        stateMachine.sendEvent(Events.EVENT2);
    }
}
```



## 版本

Spring Statemachine

Release

Documentation

2.1.0 M1

[Reference](http://docs.spring.io/spring-statemachine/docs/2.1.0.M1/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/2.1.0.M1/api)

2.1.0

[Reference](http://docs.spring.io/spring-statemachine/docs/2.1.0.BUILD-SNAPSHOT/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/2.1.0.BUILD-SNAPSHOT/api)

2.0.4

[Reference](http://docs.spring.io/spring-statemachine/docs/2.0.4.BUILD-SNAPSHOT/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/2.0.4.BUILD-SNAPSHOT/api)

2.0.3

[Reference](http://docs.spring.io/spring-statemachine/docs/2.0.3.RELEASE/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/2.0.3.RELEASE/api)

1.2.13

[Reference](http://docs.spring.io/spring-statemachine/docs/1.2.13.BUILD-SNAPSHOT/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/1.2.13.BUILD-SNAPSHOT/api)

1.2.12

[Reference](http://docs.spring.io/spring-statemachine/docs/1.2.12.RELEASE/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/1.2.12.RELEASE/api)

1.1.1

[Reference](http://docs.spring.io/spring-statemachine/docs/1.1.1.RELEASE/reference/htmlsingle/)[API](http://docs.spring.io/spring-statemachine/docs/1.1.1.RELEASE/api)

## 资源

[https://github.com/spring-projects/spring-statemachine/tree/master/spring-statemachine-samples](https://github.com/spring-projects/spring-statemachine/tree/master/spring-statemachine-samples)



