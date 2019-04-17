# Spring系列学习之Spring Messaging消息支持 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月22日 17:36:39[boonya](https://me.csdn.net/boonya)阅读数：291标签：[Spring																[Messaging](https://so.csdn.net/so/search/s.do?q=Messaging&t=blog)](https://so.csdn.net/so/search/s.do?q=Spring&t=blog)
个人分类：[Spring](https://blog.csdn.net/boonya/article/category/1721331)








英文原文：[https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-messaging.html](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-messaging.html)

**目录**

[JMS](#JMS)

[ActiveMQ支持](#ActiveMQ%E6%94%AF%E6%8C%81)

[Artemis支持](#Artemis%E6%94%AF%E6%8C%81)

[使用JNDI ConnectionFactory](#%E4%BD%BF%E7%94%A8JNDI%20ConnectionFactory)

[发送消息](#%E5%8F%91%E9%80%81%E6%B6%88%E6%81%AF)

[接收消息](#%E6%8E%A5%E6%94%B6%E6%B6%88%E6%81%AF)

[AMQP](#AMQP)

[RabbitMQ支持](#RabbitMQ%E6%94%AF%E6%8C%81)

[发送消息](#%E5%8F%91%E9%80%81%E6%B6%88%E6%81%AF)

[接收消息](#%E6%8E%A5%E6%94%B6%E6%B6%88%E6%81%AF)

[Apache Kafka支持](#Apache%20Kafka%E6%94%AF%E6%8C%81)

[发送消息](#%E5%8F%91%E9%80%81%E6%B6%88%E6%81%AF)

[接收消息](#%E6%8E%A5%E6%94%B6%E6%B6%88%E6%81%AF)

[Kafka流](#Kafka%E6%B5%81)

[其他Kafka Properties](#%E5%85%B6%E4%BB%96Kafka%20Properties)

Spring Framework为与消息传递系统的集成提供了广泛的支持，从使用JmsTemplate简化JMS API的使用到异步接收消息的完整基础结构。 Spring AMQP为高级消息队列协议提供了类似的功能集。 Spring Boot还为RabbitTemplate和RabbitMQ提供自动配置选项。 Spring WebSocket本身包含对STOMP消息传递的支持，Spring Boot通过启动器和少量自动配置支持它。 Spring Boot也支持Apache Kafka。

# JMS

javax.jms.ConnectionFactory接口提供了一种创建javax.jms.Connection以与JMS代理进行交互的标准方法。 虽然Spring需要一个ConnectionFactory来与JMS一起工作，但是你通常不需要自己直接使用它，而是可以依赖更高级别的消息传递抽象。 （有关详细信息，请参阅Spring Framework参考文档的[相关部分](https://docs.spring.io/spring/docs/5.1.3.RELEASE/spring-framework-reference/integration.html#jms)。）Spring Boot还会自动配置发送和接收消息所需的基础结构。



## ActiveMQ支持

当[ActiveMQ](http://activemq.apache.org/)在类路径上可用时，Spring Boot也可以配置ConnectionFactory。 如果代理存在，则会自动启动并配置嵌入式代理（前提是未通过配置指定代理URL）。

如果使用spring-boot-starter-activemq，则提供连接或嵌入ActiveMQ实例的必要依赖项，以及与JMS集成的Spring基础结构。

ActiveMQ配置由spring.activemq.*中的外部配置属性控制。 例如，您可以在application.properties中声明以下部分：

```
spring.activemq.broker-url=tcp://192.168.1.210:9876
spring.activemq.user=admin
spring.activemq.password=secret
```

默认情况下，CachingConnectionFactory将本机ConnectionFactory包装为可由spring.jms.*中的外部配置属性控制的合理设置:
`spring.jms.cache.session-cache-size=5`
如果您更愿意使用本机池，则可以通过向org.messaginghub:pooled-jms添加依赖项并相应地配置JmsPoolConnectionFactory来实现，如以下示例所示：

```
spring.activemq.pool.enabled=true
spring.activemq.pool.max-connections=50
```

有关更多支持的选项，请参阅[ActiveMQProperties](https://github.com/spring-projects/spring-boot/tree/v2.1.1.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/jms/activemq/ActiveMQProperties.java)。 您还可以注册实现ActiveMQConnectionFactoryCustomizer的任意数量的bean，以进行更高级的自定义。

默认情况下，ActiveMQ会创建一个目标（如果它尚不存在），以便根据提供的名称解析目标。

## Artemis支持

Spring Boot可以在检测到Artemis在类路径上可用时自动配置ConnectionFactory。 如果存在代理，则会自动启动并配置嵌入式代理（除非已明确设置mode属性）。 嵌入支持的模式（明确表示需要嵌入式代理，如果代理在类路径上不可用则发生错误）和本机（使用netty传输协议连接到代理）。 配置后者后，Spring Boot会使用默认设置配置ConnectionFactory，该ConnectionFactory连接到在本地计算机上运行的代理。

如果使用spring-boot-starter-artemis，则会提供连接到现有Artemis实例的必要依赖项，以及与JMS集成的Spring基础结构。 将org.apache.activemq:artemis-jms-server添加到您的应用程序可让您使用嵌入模式。

Artemis配置由spring.artemis.*中的外部配置属性控制。 例如，您可以在application.properties中声明以下部分：

```
spring.artemis.mode=native
spring.artemis.host=192.168.1.210
spring.artemis.port=9876
spring.artemis.user=admin
spring.artemis.password=secret
```

嵌入代理时，您可以选择是否要启用持久性并列出应该可用的目标。 可以将这些指定为以逗号分隔的列表，以使用默认选项创建它们，也可以定义类型为org.apache.activemq.artemis.jms.server.config.JMSQueueConfiguration或org.apache.activemq的bean。 artemis.jms.server.config.TopicConfiguration，分别用于高级队列和主题配置。


默认情况下，CachingConnectionFactory将本机ConnectionFactory包装为可由spring.jms.*中的外部配置属性控制的合理设置：
`spring.jms.cache.session-cache-size=5`
如果您更愿意使用本机池，则可以通过向org.messaginghub:pooled-jms添加依赖项并相应地配置JmsPoolConnectionFactory来实现，如以下示例所示：

```
spring.artemis.pool.enabled=true
spring.artemis.pool.max-connections=50
```

有关更多支持的选项，请参阅[ArtemisProperties](https://github.com/spring-projects/spring-boot/tree/v2.1.1.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/jms/artemis/ArtemisProperties.java)。


不涉及JNDI查找，并使用Artemis配置中的name属性或通过配置提供的名称来解析目标名称。



## 使用JNDI ConnectionFactory

如果您在应用程序服务器中运行应用程序，Spring Boot会尝试使用JNDI找到JMS ConnectionFactory。 默认情况下，将检查java:/JmsXA和java:/XAConnectionFactory位置。 如果需要指定备用位置，可以使用spring.jms.jndi-name属性，如以下示例所示：
`spring.jms.jndi-name=java:/MyConnectionFactory`
## 发送消息

Spring的JmsTemplate是自动配置的，你可以直接将它自动装入你自己的bean中，如下例所示：

```java
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.stereotype.Component;

@Component
public class MyBean {

	private final JmsTemplate jmsTemplate;

	@Autowired
	public MyBean(JmsTemplate jmsTemplate) {
		this.jmsTemplate = jmsTemplate;
	}

	// ...

}
```

JmsMessagingTemplate可以以类似的方式注入。 如果定义了DestinationResolver或MessageConverter bean，它将自动关联到自动配置的JmsTemplate。

## 接收消息

当存在JMS基础结构时，可以使用@JmsListener对任何bean进行批注以创建侦听器端点。如果未定义JmsListenerContainerFactory，则会自动配置默认值。如果定义了DestinationResolver或MessageConverter bean，它将自动关联到默认工厂。


默认情况下，默认工厂是事务性的。如果您在存在JtaTransactionManager的基础结构中运行，则默认情况下它与侦听器容器相关联。如果不是，则启用sessionTransacted标志。在后一种情况下，您可以通过在侦听器方法（或其委托）上添加@Transactional，将本地数据存储事务与传入消息的处理相关联。这确保了在本地事务完成后确认传入消息。这还包括发送已在同一JMS会话上执行的响应消息。


以下组件在someQueue目标上创建一个侦听器端点：

```java
@Component
public class MyBean {

	@JmsListener(destination = "someQueue")
	public void processMessage(String content) {
		// ...
	}

}
```

有关更多详细信息，请参阅[@EnableJms的Javadoc](https://docs.spring.io/spring/docs/5.1.3.RELEASE/javadoc-api/org/springframework/jms/annotation/EnableJms.html)。

如果需要创建更多JmsListenerContainerFactory实例，或者如果要覆盖缺省值，Spring Boot会提供一个DefaultJmsListenerContainerFactoryConfigurer，您可以使用它来初始化DefaultJmsListenerContainerFactory，其设置与自动配置的设置相同。


例如，以下示例公开了另一个使用特定MessageConverter的工厂：

```java
@Configuration
static class JmsConfiguration {

	@Bean
	public DefaultJmsListenerContainerFactory myFactory(
			DefaultJmsListenerContainerFactoryConfigurer configurer) {
		DefaultJmsListenerContainerFactory factory =
				new DefaultJmsListenerContainerFactory();
		configurer.configure(factory, connectionFactory());
		factory.setMessageConverter(myMessageConverter());
		return factory;
	}

}
```

然后，您可以在任何@JmsListener-annotated方法中使用工厂，如下所示：

```java
@Component
public class MyBean {

	@JmsListener(destination = "someQueue", containerFactory="myFactory")
	public void processMessage(String content) {
		// ...
	}

}
```



# AMQP

高级消息队列协议（AMQP）是面向消息的中间件的平台中立的线级协议。 Spring AMQP项目将核心Spring概念应用于基于AMQP的消息传递解决方案的开发。 Spring Boot为通过RabbitMQ使用AMQP提供了一些便利，包括spring-boot-starter-amqp“Starter”。

## RabbitMQ支持

RabbitMQ是一个基于AMQP协议的轻量级，可靠，可扩展且可移植的消息代理。 Spring使用RabbitMQ通过AMQP协议进行通信。


RabbitMQ配置由spring.rabbitmq.*中的外部配置属性控制。 例如，您可以在application.properties中声明以下部分：

```
spring.rabbitmq.host=localhost
spring.rabbitmq.port=5672
spring.rabbitmq.username=admin
spring.rabbitmq.password=secret
```

如果上下文中存在ConnectionNameStrategy bean，它将自动用于命名由自动配置的ConnectionFactory创建的连接。 有关更多支持的选项，请参阅[RabbitProperties](https://github.com/spring-projects/spring-boot/tree/v2.1.1.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/amqp/RabbitProperties.java)。

有关详细信息，请参阅[了解AMQP，RabbitMQ使用的协议](https://spring.io/blog/2010/06/14/understanding-amqp-the-protocol-used-by-rabbitmq/)。

## 发送消息

Spring的AmqpTemplate和AmqpAdmin是自动配置的，您可以将它们直接自动装入自己的bean中，如下例所示：

```java
import org.springframework.amqp.core.AmqpAdmin;
import org.springframework.amqp.core.AmqpTemplate;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class MyBean {

	private final AmqpAdmin amqpAdmin;
	private final AmqpTemplate amqpTemplate;

	@Autowired
	public MyBean(AmqpAdmin amqpAdmin, AmqpTemplate amqpTemplate) {
		this.amqpAdmin = amqpAdmin;
		this.amqpTemplate = amqpTemplate;
	}

	// ...

}
```

[RabbitMessagingTemplate](https://docs.spring.io/spring-amqp/docs/current/api/org/springframework/amqp/rabbit/core/RabbitMessagingTemplate.html)可以以类似的方式注入。 如果定义了MessageConverter bean，它将自动关联到自动配置的AmqpTemplate。

如有必要，任何定义为bean的org.springframework.amqp.core.Queue都会自动用于在RabbitMQ实例上声明相应的队列。


要重试操作，可以在AmqpTemplate上启用重试（例如，在代理连接丢失的情况下）：

```
spring.rabbitmq.template.retry.enabled=true
spring.rabbitmq.template.retry.initial-interval=2s
```

默认情况下禁用重试。 您还可以通过声明RabbitRetryTemplateCustomizer bean以编程方式自定义RetryTemplate。

## 接收消息

当Rabbit基础结构存在时，可以使用@RabbitListener注释任何bean以创建侦听器端点。 如果未定义RabbitListenerContainerFactory，则会自动配置默认的SimpleRabbitListenerContainerFactory，您可以使用spring.rabbitmq.listener.type属性切换到直接容器。 如果定义了MessageConverter或MessageRecoverer bean，它将自动与默认工厂关联。


以下示例组件在someQueue队列上创建一个侦听器端点：

```java
@Component
public class MyBean {

	@RabbitListener(queues = "someQueue")
	public void processMessage(String content) {
		// ...
	}

}
```

有关更多详细信息，请参阅[@EnableRabbit的Javadoc](https://docs.spring.io/spring-amqp/docs/current/api/org/springframework/amqp/rabbit/annotation/EnableRabbit.html)。

如果需要创建更多RabbitListenerContainerFactory实例，或者如果要覆盖缺省值，Spring Boot提供了一个SimpleRabbitListenerContainerFactoryConfigurer和一个DirectRabbitListenerContainerFactoryConfigurer，您可以使用它来初始化SimpleRabbitListenerContainerFactory和DirectRabbitListenerContainerFactory，其设置与自动配置使用的工厂相同。

您选择的容器类型无关紧要。 这两个bean通过自动配置公开。

例如，以下配置类公开了另一个使用特定MessageConverter的工厂：

```java
@Configuration
static class RabbitConfiguration {

	@Bean
	public SimpleRabbitListenerContainerFactory myFactory(
			SimpleRabbitListenerContainerFactoryConfigurer configurer) {
		SimpleRabbitListenerContainerFactory factory =
				new SimpleRabbitListenerContainerFactory();
		configurer.configure(factory, connectionFactory);
		factory.setMessageConverter(myMessageConverter());
		return factory;
	}

}
```

然后，您可以在任何@RabbitListener-annotated方法中使用工厂，如下所示：

```java
@Component
public class MyBean {

	@RabbitListener(queues = "someQueue", containerFactory="myFactory")
	public void processMessage(String content) {
		// ...
	}

}
```

您可以启用重试来处理侦听器抛出异常的情况。 默认情况下，使用RejectAndDontRequeueRecoverer，但您可以定义自己的MessageRecoverer。 当重试耗尽时，如果代理配置了这样做，则拒绝该消息并将其丢弃或路由到死信交换。 默认情况下，禁用重试。 您还可以通过声明RabbitRetryTemplateCustomizer bean以编程方式自定义RetryTemplate。

重要：默认情况下，如果禁用重试并且侦听器抛出异常，则会无限期地重试传递。 您可以通过两种方式修改此行为：将defaultRequeueRejected属性设置为false，以便尝试零重新传递或抛出AmqpRejectAndDontRequeueException以指示应拒绝该消息。 后者是启用重试并且达到最大传递尝试次数时使用的机制。

# Apache Kafka支持

通过提供spring-kafka项目的自动配置来支持[Apache Kafka](https://kafka.apache.org/)。


Kafka配置由spring.kafka.*中的外部配置属性控制。 例如，您可以在application.properties中声明以下部分：

```
spring.kafka.bootstrap-servers=localhost:9092
spring.kafka.consumer.group-id=myGroup
```

要在启动时创建主题，请添加NewTopic类型的Bean。 如果主题已存在，则忽略该bean。

有关更多支持的选项，请参阅[KafkaProperties](https://github.com/spring-projects/spring-boot/tree/v2.1.1.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/kafka/KafkaProperties.java)。

## 发送消息

Spring的KafkaTemplate是自动配置的，您可以直接在自己的bean中自动装配它，如下例所示：

```java
@Component
public class MyBean {

	private final KafkaTemplate kafkaTemplate;

	@Autowired
	public MyBean(KafkaTemplate kafkaTemplate) {
		this.kafkaTemplate = kafkaTemplate;
	}

	// ...

}
```

如果定义了属性spring.kafka.producer.transaction-id-prefix，则会自动配置KafkaTransactionManager。 此外，如果定义了RecordMessageConverter bean，它将自动关联到自动配置的KafkaTemplate。

## 接收消息

当存在Apache Kafka基础结构时，可以使用@KafkaListener注释任何bean以创建侦听器端点。 如果未定义KafkaListenerContainerFactory，则会使用spring.kafka.listener.*中定义的键自动配置默认值。


以下组件在someTopic主题上创建一个侦听器端点：

```java
@Component
public class MyBean {

	@KafkaListener(topics = "someTopic")
	public void processMessage(String content) {
		// ...
	}

}
```

如果定义了KafkaTransactionManager bean，它将自动关联到容器工厂。 同样，如果定义了RecordMessageConverter，ErrorHandler或AfterRollbackProcessor bean，它将自动关联到默认工厂。

自定义ChainedKafkaTransactionManager必须标记为@Primary，因为它通常引用自动配置的KafkaTransactionManager bean。

## Kafka流

Spring for Apache Kafka提供了一个工厂bean来创建StreamsBuilder对象并管理其流的生命周期。 只要kafka-streams在类路径上并且通过@EnableKafkaStreams注释启用Kafka Streams，Spring Boot就会自动配置所需的KafkaStreamsConfiguration bean。


启用Kafka Streams意味着必须设置应用程序ID和引导程序服务器。 可以使用spring.kafka.streams.application-id配置前者，如果未设置则默认为spring.application.name。 后者可以全局设置或专门为流而重写。


使用专用属性可以使用其他几个属性; 可以使用spring.kafka.streams.properties命名空间设置其他任意Kafka属性。 有关更多信息，另请参见[第33.3.4节“其他Kafka属性”](https://docs.spring.io/spring-boot/docs/current/reference/html/boot-features-messaging.html#boot-features-kafka-extra-props)。


要使用工厂bean，只需将StreamsBuilder连接到@Bean，如以下示例所示：

```java
@Configuration
@EnableKafkaStreams
static class KafkaStreamsExampleConfiguration {

	@Bean
	public KStream<Integer, String> kStream(StreamsBuilder streamsBuilder) {
		KStream<Integer, String> stream = streamsBuilder.stream("ks1In");
		stream.map((k, v) -> new KeyValue<>(k, v.toUpperCase())).to("ks1Out",
				Produced.with(Serdes.Integer(), new JsonSerde<>()));
		return stream;
	}

}
```

默认情况下，由其创建的StreamBuilder对象管理的流将自动启动。 您可以使用spring.kafka.streams.auto-startup属性自定义此行为。

## 其他Kafka Properties

自动配置支持的属性[显示在附录A，常见应用程序属性](https://docs.spring.io/spring-boot/docs/current/reference/html/common-application-properties.html)中。 请注意，在大多数情况下，这些属性（连字符或camelCase）直接映射到Apache Kafka点状属性。 有关详细信息，请参阅Apache Kafka文档。


这些属性中的前几个适用于所有组件（生产者，使用者，管理员和流），但如果您希望使用不同的值，则可以在组件级别指定。 Apache Kafka指定重要性为HIGH，MEDIUM或LOW的属性。 Spring Boot自动配置支持所有HIGH重要性属性，一些选择的MEDIUM和LOW属性，以及任何没有默认值的属性。


只有Kafka支持的属性的子集可以直接通过KafkaProperties类获得。 如果您希望使用不直接支持的其他属性配置生产者或使用者，请使用以下属性：

```
spring.kafka.properties.prop.one=first
spring.kafka.admin.properties.prop.two=second
spring.kafka.consumer.properties.prop.three=third
spring.kafka.producer.properties.prop.four=fourth
spring.kafka.streams.properties.prop.five=fifth
```

这将常见的prop.one Kafka属性设置为第一个（适用于生产者，消费者和管理员），prop.two管理员属性为第二个，prop.three消费者属性为第三个，prop.four生产者属性为第四个和prop.five streams属性为第五。

您还可以按如下方式配置Spring Kafka JsonDeserializer：

```
spring.kafka.consumer.value-deserializer=org.springframework.kafka.support.serializer.JsonDeserializer
spring.kafka.consumer.properties.spring.json.value.default.type=com.example.Invoice
spring.kafka.consumer.properties.spring.json.trusted.packages=com.example,org.acme
```

同样，您可以禁用JsonSerializer在标头中发送类型信息的默认行为：

```
spring.kafka.producer.value-serializer=org.springframework.kafka.support.serializer.JsonSerializer
spring.kafka.producer.properties.spring.json.add.type.headers=false
```

重要:以这种方式设置的属性将覆盖Spring Boot明确支持的任何配置项。




