# Spring Batch 在大型企业中的最佳实践 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [ThoughtWorks](http://www.jobbole.com/members/thoughtworkschina) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
在大型企业中，由于业务复杂、数据量大、数据格式不同、数据交互格式繁杂，并非所有的操作都能通过交互界面进行处理。而有一些操作需要定期读取大批量的数据，然后进行一系列的后续处理。这样的过程就是“批处理”。
![database-schema](http://insights.thoughtworkers.org/wp-content/uploads/2017/01/database-schema-1024x894.jpg)
批处理应用通常有以下特点：
- 数据量大，从数万到数百万甚至上亿不等；
- 整个过程全部自动化，并预留一定接口进行自定义配置；
- 这样的应用通常是周期性运行，比如按日、周、月运行；
- 对数据处理的准确性要求高，并且需要容错机制、回滚机制、完善的日志监控等。
### 什么是Spring batch
Spring batch是一个轻量级的全面的批处理框架，它专为大型企业而设计，帮助开发健壮的批处理应用。Spring batch为处理大批量数据提供了很多必要的可重用功能，比如日志追踪、事务管理、job执行统计、重启job和资源管理等。同时它也提供了优化和分片技术用于实现高性能的批处理任务。
它的核心功能包括：
- 事务管理
- 基于块的处理过程
- 声明式的输入/输出操作
- 启动、终止、重启任务
- 重试/跳过任务
- 基于Web的管理员接口
笔者所在的部门属于国外某大型金融公司的CRM部门，在日常工作中我们经常需要开发一些批处理应用，对Spring Batch有着丰富的使用经验。近段时间笔者特意总结了这些经验。
![spring batch](http://insights.thoughtworkers.org/wp-content/uploads/2017/01/spring-batch.jpg)
### 使用Spring Batch 3.0以及Spring Boot
在使用Spring Batch时推荐使用最新的Spring Batch 3.0版本。相比Spring Batch2.2，它做了以下方面的提升：
- 支持JSR-352标准
- 支持Spring4以及Java8
- 增强了Spring Batch Integration的功能
- 支持JobScope
- 支持SQLite
支持Spring4和Java8是一个重大的提升。这样就可以使用Spring4引入的Spring boot组件，从而开发效率方面有了一个质的飞跃。引入Spring-batch框架只需要在build.gradle中加入一行代码即可：


```
compile("org.springframework.boot:spring-boot-starter-batch")
```
而增强Spring Batch Integration的功能后，我们就可以很方便的和Spring家族的其他组件集成，还可以以多种方式来调用job，也支持远程分区操作以及远程块处理。
而支持JobScope后我们可以随时为对象注入当前Job实例的上下文信息。只要我们指定Bean的scope为job scope，那么就可以随时使用jobParameters和jobExecutionContext等信息。


```
@Component
@JobScope
public class CustomClass {
    @Value("#{jobParameters[jobDate]}")
    private String jobDate;
    @Value("#{jobExecutionContext['input.name']}.")
    private String fileName;
}
```
### 使用Java Config而不是xml的配置方式
之前我们在配置job和step的时候都习惯用xml的配置方式，但是随着时间的推移发现问题颇多。
- xml文件数急剧膨胀，配置块长且复杂，可读性很差；
- xml文件缺少语法检查，有些低级错误只有在运行集成测试的时候才能发现；
- 在xml文件中进行代码跳转时IDE的支持力度不够；
我们渐渐发现使用纯Java类的配置方式更灵活，它是类型安全的，而且IDE的支持更好。在构建job或step时采用的流式语法相比xml更加简洁易懂。


```
@Bean
public Step step(){
    return stepBuilders.get("step")
        .chunk(1)
        .reader(reader())
        .processor(processor())
        .writer(writer())
        .listener(logProcessListener())
        .faultTolerant()
        .skipLimit(10)
        .skip(UnknownGenderException.class)
        .listener(logSkipListener())
        .build();
}
```
在这个例子中可以很清楚的看到该step的配置，比如reader/processor/writer组件，以及配置了哪些listener等。
### 本地集成测试中使用内存数据库
Spring batch在运行时需要数据库支持，因为它需要在数据库中建立一套schema来存储job和step运行的统计信息。而在本地集成测试中我们可以借助Spring batch提供的内存Repository来存储Spring batch的任务执行信息，这样既避免了在本地配置一个数据库，又可以加快job的执行。先为Job的配置类添加扩展类：DefaultBatchConfigurer。


```
public class CustomJobConfiguration extends DefaultBatchConfigurer {
    ...
}
```
我们在build.gradle中加入对hsqldb的依赖：


```
runtime(‘org.hsqldb:hsqldb:2.3.2’)
```
然后在测试类中添加对DataSource的配置。


```
@EnableAutoConfiguration
@EnableBatchProcessing
@DataJpaTest
@Import({DataSourceAutoConfiguration.class, BatchAutoConfiguration.class})
public class TestConfiguration {
}
```
并且在applicaton.properties配置中添加初始化Database的配置：


```
spring.batch.initializer.enable=true
```
### 合理的使用Chunk机制
Spring batch在配置Step时采用的是基于Chunk的机制。即每次读取一条数据，再处理一条数据，累积到一定数量后再一次性交给writer进行写入操作。这样可以最大化的优化写入效率，整个事务也是基于Chunk来进行。
当我们在需要将数据写入到文件、数据库中之类的操作时可以适当设置Chunk的值以满足写入效率最大化。但有些场景下我们的写入操作其实是调用一个web service或者将消息发送到某个消息队列中，那么这些场景下我们就需要设置Chunk的值为1，这样既可以及时的处理写入，也不会由于整个Chunk中发生异常后，在重试时出现重复调用服务或者重复发送消息的情况。
### 使用Listener来监视job执行情况并及时做相应的处理
Spring batch提供了大量的Listener来对job的各个执行环节进行全面的监控。
在job层面Spring batch提供了JobExecutionListener接口，其支持在Job开始或结束时进行一些额外处理。在step层面Spring batch提供了StepExecutionListener，ChunkListener，ItemReadListener，ItemProcessListener，ItemWriteListener，SkipListener等接口，同时对Retry和Skip操作也提供了RetryListener及SkipListener。
通常我们会为每个job都实现一个JobExecutionListener，在afterJob操作中我们输出job的执行信息，包括执行时间、job参数、退出代码、执行的step以及每个step的详细信息。这样无论是开发、测试还是运维人员都对整个job的执行情况了如指掌。
如果某个step会发生skip的操作，我们也会为其实现一个SkipListener，并在其中记录skip的数据条目，用于下一步的处理。
实现Listener有两种方式，一种是继承自相应的接口，比如继承JobExecutionListener接口，另一种是使用annoation（注解）的方式。经过实践我们认为使用注解的方式更好一些，因为使用接口你需要实现接口的所有方法，而使用注解则只需要对相应的方法添加annoation即可。
下面的这个类采用了继承接口的方式，我们看到其实我们只用到了第一个方法，第二个和第三个都没有用到。但是我们必须提供一个空的实现。


```
public class CustomSkipListener implements SkipListener {
    @Override
    public void onSkipInRead(Throwable t) {
        // business logic
    }
    @Override
    public void onSkipInWrite(String item, Throwable t) {
        // no need
    }
    @Override
    public void onSkipInProcess(String item, Throwable t) {
        // no need
    }
}
```
而使用annoation的方式可以简写为：


```
public class CustomSkipListener {
    @OnSkipInRead
    public void onSkipInRead(Throwable t) {
        // business logic
    }
}
```
### 使用Retry和Skip增强批处理工作的健壮性
在处理百万级的数据过程过程中难免会出现异常。如果一旦出现异常而导致整个批处理工作终止的话那么会导致后续的数据无法被处理。Spring Batch内置了Retry（重试）和Skip（跳过）机制帮助我们轻松处理各种异常。我们需要将异常分为三种类型。第一种是需要进行Retry的异常，它们的特点是该异常可能会随着时间推移而消失，比如数据库目前有锁无法写入、web服务当前不可用、web服务满载等。所以对它们适合配置Retry机制。第二种是需要Skip的异常，比如解析文件的某条数据出现异常等，因为对这些异常即使执行Retry每次的结果也都是相同，但又不想由于某条数据出错而停止对后续数据的处理。第三种异常是需要让整个Job立刻失败的异常，比如如果出现了OutOfMemory的异常，那么需要整个Job立刻终止运行。
一般来说需要Retry的异常也要配置Skip选项，从而保证后续的数据能够被继续处理。我们也可以配置SkipLimit选项保证当Skip的数据条目达到一定数量后及时终止整个Job。
有时候我们需要在每次Retry中间隔做一些操作，比如延长Retry时间，恢复操作现场等，Spring Batch提供了BackOffPolicy来达到目的。下面是一个配置了Retry机制、Skip机制以及BackOffPolicy的step示例。


```
@Bean
public Step step(){
    return stepBuilders.get("step")
        .chunk(1)
        .reader(reader())
        .processor(processor())
        .writer(writer())
        .listener(logProcessListener())
        .faultTolerant()
        .skipLimit(10)
        .skip(UnknownGenderException.class)
        .skip(ServiceUnavailableException.class)
        .retryLimit(5)
        .retry(ServiceUnavailableException.class)
        .backOffPolicy(backoffPolicy)
        .listener(logSkipListener())
        .build();
}
```
### 使用自定义的Decider来实现Job flow
在Job执行过程中不一定都是顺序执行的，我们经常需要根据某个job的输出数据或执行结果来决定下一步的走向。以前我们会把一些判断放置在下游step中进行，这样可能会导致有些step实际运行了，但其实并没有做任何事情。比如一个step执行过程中会将失败的数据条目记录到一个报告中，而下一个step会判断有没有生成报告，如果生成了报告则将该报告发送给指定联系人，如果没有则不做任何事情。这种情况下可以通过Decider机制来实现Job的执行流程。在Spring batch 3.0中Decider已经从Step中独立出来，和Step处于同一级别。


```
public class ReportDecider implements JobExecutionDecider {
    @Override
    public FlowExecutionStatus decide(JobExecution jobExecution, StepExecution stepExecution) {
        if (report.isExist()) {
            return new FlowExecutionStatus(“SEND");
        }
        return new FlowExecutionStatus(“SKIP");
    }
}
```
而在job配置中可以这样来使用Decider。这样整个Job的执行流程会更加清晰易懂。


```
public Job job() {
    return new JobBuilder("petstore")
        .start(orderProcess())
        .next(reportDecider)
        .on("SEND").to(sendReportStep)
        .on("SKIP").end().build()
        .build();
}
```
### 采用多种机制加速Job的执行
批处理工作处理的数据量大，而执行窗口一般又要求比较小。所以必须要通过多种方式来加速Job的执行。一般我们有四种方式来实现：
- 在单个step中多线程执行任务
- 并行执行不同的Step
- 并行执行同一个Step
- 远程执行Chunk任务
单个step多线程执行任务可以借助于taskExecutor来实现。这种情况适合于reader、writer是线程安全且是无状态的场景。我们还可以设置线程数量。


```
public Step step() {
    return stepBuilders.get("step")
        .tasklet(tasklet)
        .throttleLimit(20)
        .build();
}
```
上述示例中的tasklet需要实现TaskExecutor，Spring Batch提供了一个简单的多线程TaskExecutor供我们使用：SimpleAsyncTaskExecutor。
并行执行不同的Step在Spring batch中很容易实现，以下是一个示例：


```
public Job job() {
    return stepBuilders.get("parallelSteps")
        .start(step1)
        .split(asyncTaskExecutor).add(flow1, flow2)
        .next(step3)
        .build();
}
```
在这个示例中我们先执行step1，然后并行执行flow1和flow2，最后再执行step3。
Spring batch提供了PartitionStep来实现对同一个step在多个进程中实现并行处理。通过PartitonStep再配合PartitionHandler可以将一个step扩展到多个Slave上实现并行运行。
远程执行Chunk任务则是将某个Step的processer操作分割到多个进程中，多个进程通过一些中间件进行通讯（比如采用消息的方式）。这种方式适合于Processer是瓶颈而Reader和Writer不是瓶颈的场景。
## 结语
Spring Batch对批处理场景进行了合理的抽象，封装了大量的实用功能，使用它来开发批处理应用可以达到事半功倍的效果。在使用的过程中我们仍需要坚持总结一些最佳实践，从而能够交付高质量的可维护的批处理应用，满足企业级应用的苛刻要求。
