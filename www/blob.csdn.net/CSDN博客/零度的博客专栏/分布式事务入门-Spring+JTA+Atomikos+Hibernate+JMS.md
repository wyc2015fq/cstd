# 分布式事务入门-Spring+JTA+Atomikos+Hibernate+JMS - 零度的博客专栏 - CSDN博客
2016年04月07日 13:47:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：642
本地事务和分布式事务的区别在于：本地事务只用于处理单一数据源事务（比如单个数据库），分布式事务可以处理多种异构的数据源，比如某个业务操作中同时包含了JDBC和JMS或者某个操作需要访问多个不同的数据库。
Java通过JTA完成分布式事务，JTA本身只是一种规范，不同的应用服务器都包含有自己的实现（比如JbossJTA），同时还存在独立于应用服务器的单独JTA实现，比如本篇中要讲到的Atomikos。对于JTA的原理，这里不细讲，读者可以通过
[这篇文章](http://www.ibm.com/developerworks/cn/java/j-lo-jta/index.html) 了解相关知识。 
在本篇文章中，我们将实现以下一个应用场景：你在网上购物，下了订单之后，订单数据将保存在系统的数据库中，同时为了安排物流，订单信息将以消息（Message）的方式发送到物流部门以便送货。
以上操作同时设计到数据库操作和JMS消息发送，为了使整个操作成为一个原子操作，我们只能选择分布式事务。我们首先设计一个service层，定义OrderService接口：
```java
package davenkin;
public interface OrderService {
    public void makeOrder(Order order);
}
```
为了简单起见，我们设计一个非常简单的领域对象Order：
```java
@XmlRootElement(name = "Order")
@XmlAccessorType(XmlAccessType.FIELD)
public class Order {
    @XmlElement(name = "Id",required = true)
    private long id;
    @XmlElement(name = "ItemName",required = true)
    private String itemName;
    @XmlElement(name = "Price",required = true)
    private double price;
    @XmlElement(name = "BuyerName",required = true)
    private String buyerName;
    @XmlElement(name = "MailAddress",required = true)
    private String mailAddress;
    public Order() {
    }
```
为了采用JAXB对Order对象进行Marshal和Unmarshal，我们在Order类中加入了JAXB相关的Annotation。 我们将使用Hibernate来完成数据持久化，然后使用Spring提供的JmsTemplate将Order转成xml后以TextMessage的形式发送到物流部门的ORDER.QUEUE中。
（一）准备数据库
为了方便，我们将采用Spring提供的embedded数据库，默认情况下Spring采用HSQL作为后台数据库，虽然在本例中我们将采用HSQL的非XA的DataSource，但是通过Atomikos包装之后依然可以参与分布式事务。
SQL脚本包含在createDB.sql文件中：
```
CREATE TABLE USER_ORDER(
ID INT NOT NULL,
ITEM_NAME VARCHAR (100) NOT NULL UNIQUE,
PRICE DOUBLE NOT NULL,
BUYER_NAME CHAR (32) NOT NULL,
MAIL_ADDRESS VARCHAR(500) NOT NULL,
PRIMARY KEY(ID)
);
```
在Spring中配置DataSource如下：
<jdbc:embedded-databaseid="dataSource"><jdbc:scriptlocation="classpath:createDB.sql"/></jdbc:embedded-database>
（二）启动ActiveMQ
我们将采用embedded的ActiveMQ，在测试之前启动ActiveMQ提供的BrokerService，在测试执行完之后关闭BrokerService。
@BeforeClasspublicstaticvoid startEmbeddedActiveMq()throws Exception {
        broker = new BrokerService();
        broker.addConnector("tcp://localhost:61616");
        broker.start();
    }
    @AfterClasspublicstaticvoid stopEmbeddedActiveMq()throws Exception {
        broker.stop();
    }
（三）实现OrderService
创建一个DefaultOrderService，该类实现了OrderService接口，并维护一个JmsTemplate和一个Hibernate的SessionFactory实例变量，分别用于Message的发送和数据库处理。
```java
package davenkin;
import org.hibernate.SessionFactory;
import org.hibernate.classic.Session;
import org.springframework.beans.factory.annotation.Required;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.transaction.annotation.Transactional;
public class DefaultOrderService  implements OrderService{
    private JmsTemplate jmsTemplate;
    private SessionFactory sessionFactory;
    @Override
    @Transactional
    public void makeOrder(Order order) {
        Session session = sessionFactory.getCurrentSession();
        session.save(order);
        jmsTemplate.convertAndSend(order);
    }
    @Required
    public void setJmsTemplate(JmsTemplate jmsTemplate) {
        this.jmsTemplate = jmsTemplate;
    }
    @Required
    public void setSessionFactory(SessionFactory sessionFactory) {
        this.sessionFactory = sessionFactory;
    }
}
```
（四）创建Order的Mapping配置文件
<?xml version="1.0"?><!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD 3.0//EN"
        "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd"><hibernate-mapping><classname="davenkin.Order"table="USER_ORDER"><idname="id"type="long"><columnname="ID"/><generatorclass="increment"/></id><propertyname="itemName"type="string"><columnname="ITEM_NAME"/></property><propertyname="price"type="double"><columnname="PRICE"/></property><propertyname="buyerName"type="string"><columnname="BUYER_NAME"/></property><propertyname="mailAddress"type="string"><columnname="MAIL_ADDRESS"/></property></class></hibernate-mapping>
（五）配置Atomikos事务
在Spring的IoC容器中，我们需要配置由Atomikos提供的UserTransaction和TransactionManager，然后再配置Spring的JtaTransactionManager：
<beanid="userTransactionService"class="com.atomikos.icatch.config.UserTransactionServiceImp"init-method="init"destroy-method="shutdownForce"><constructor-arg><props><propkey="com.atomikos.icatch.service">com.atomikos.icatch.standalone.UserTransactionServiceFactory</prop></props></constructor-arg></bean><beanid="atomikosTransactionManager"class="com.atomikos.icatch.jta.UserTransactionManager"init-method="init"destroy-method="close"depends-on="userTransactionService"><propertyname="forceShutdown"value="false"/></bean><beanid="atomikosUserTransaction"class="com.atomikos.icatch.jta.UserTransactionImp"depends-on="userTransactionService"><propertyname="transactionTimeout"value="300"/></bean><beanid="jtaTransactionManager"class="org.springframework.transaction.jta.JtaTransactionManager"depends-on="userTransactionService"><propertyname="transactionManager"ref="atomikosTransactionManager"/><propertyname="userTransaction"ref="atomikosUserTransaction"/></bean><tx:annotation-driventransaction-manager="jtaTransactionManager"/>
（六）配置JMS
对于JMS，为了能使ActiveMQ加入到分布式事务中，我们需要配置ActiveMQXAConnectionFactory，而不是ActiveMQConnectionFactory，然后再配置JmsTemplate，此外还需要配置MessageConvertor在Order对象和XML之间互转。
<beanid="jmsXaConnectionFactory"class="org.apache.activemq.ActiveMQXAConnectionFactory"><propertyname="brokerURL"value="tcp://localhost:61616"/></bean><beanid="amqConnectionFactory"class="com.atomikos.jms.AtomikosConnectionFactoryBean"init-method="init"><propertyname="uniqueResourceName"value="XAactiveMQ"/><propertyname="xaConnectionFactory"ref="jmsXaConnectionFactory"/><propertyname="poolSize"value="5"/></bean><beanid="jmsTemplate"class="org.springframework.jms.core.JmsTemplate"><propertyname="connectionFactory"ref="amqConnectionFactory"/><propertyname="receiveTimeout"value="2000"/><propertyname="defaultDestination"ref="orderQueue"/><propertyname="sessionTransacted"value="true"/><propertyname="messageConverter"ref="oxmMessageConverter"/></bean><beanid="orderQueue"class="org.apache.activemq.command.ActiveMQQueue"><constructor-argvalue="ORDER.QUEUE"/></bean><beanid="oxmMessageConverter"class="org.springframework.jms.support.converter.MarshallingMessageConverter"><propertyname="marshaller"ref="marshaller"/><propertyname="unmarshaller"ref="marshaller"/></bean><oxm:jaxb2-marshallerid="marshaller"><oxm:class-to-be-boundname="davenkin.Order"/></oxm:jaxb2-marshaller>
（七）测试
在测试中，我们首先通过（二）中的方法启动ActiveMQ，再调用DefaultOrderService，最后对数据库和QUEUE进行验证：
   @Test
    publicvoid makeOrder(){
        orderService.makeOrder(createOrder());
        JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
        assertEquals(1, jdbcTemplate.queryForInt("SELECT COUNT(*) FROM USER_ORDER"));
        String dbItemName = jdbcTemplate.queryForObject("SELECT ITEM_NAME FROM USER_ORDER", String.class);
        String messageItemName = ((Order) jmsTemplate.receiveAndConvert()).getItemName();
        assertEquals(dbItemName, messageItemName);
    }
    @Test(expected = IllegalArgumentException.class)
    publicvoid failToMakeOrder()
    {
        orderService.makeOrder(null);
        JdbcTemplate jdbcTemplate = new JdbcTemplate(dataSource);
        assertEquals(0, jdbcTemplate.queryForInt("SELECT COUNT(*) FROM USER_ORDER"));
        assertNull(jmsTemplate.receiveAndConvert());
    }
