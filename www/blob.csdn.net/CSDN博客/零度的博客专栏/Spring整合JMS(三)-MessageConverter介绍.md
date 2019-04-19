# Spring整合JMS(三)-MessageConverter介绍 - 零度的博客专栏 - CSDN博客
2016年05月25日 17:02:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：388
## 1.4     消息转换器MessageConverter
MessageConverter的作用主要有两方面，一方面它可以把我们的非标准化Message对象转换成我们的目标Message对象，这主要是用在发送消息的时候；另一方面它又可以把我们的Message对象转换成对应的目标对象，这主要是用在接收消息的时候。
下面我们就拿发送一个对象消息来举例，假设我们有这样一个需求：我们平台有一个发送邮件的功能，进行发送的时候我们只是把我们的相关信息封装成一个JMS消息，然后利用JMS进行发送，在对应的消息监听器进行接收到的消息处理时才真正的进行消息发送。
假设我们有这么一个Email对象：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class Email implements Serializable {  
- 
-     private static final long serialVersionUID = -658250125732806493L;  
- 
-     private String receiver;  
-     private String title;  
-     private String content;  
- 
-     public Email(String receiver, String title, String content) {  
-         this.receiver = receiver;  
-         this.title = title;  
-         this.content = content;  
-     }  
- 
-     public String getReceiver() {  
-         return receiver;  
-     }  
- 
-     public void setReceiver(String receiver) {  
-         this.receiver = receiver;  
-     }  
- 
-     public String getTitle() {  
-         return title;  
-     }  
- 
-     public void setTitle(String title) {  
-         this.title = title;  
-     }  
- 
-     public String getContent() {  
-         return content;  
-     }  
- 
-     public void setContent(String content) {  
-         this.content = content;  
-     }  
- 
-     @Override  
-     public String toString() {  
-         StringBuilder builder = new StringBuilder();  
-         builder.append("Email [receiver=").append(receiver).append(", title=")  
-                 .append(title).append(", content=").append(content).append("]");  
-         return builder.toString();  
-     }  
- 
- }  
public class Email implements Serializable {     private static final long serialVersionUID = -658250125732806493L;     private String receiver;    private String title;    private String content;     public Email(String receiver, String title, String content) {        this.receiver = receiver;        this.title = title;        this.content = content;    }     public String getReceiver() {        return receiver;    }     public void setReceiver(String receiver) {        this.receiver = receiver;    }     public String getTitle() {        return title;    }     public void setTitle(String title) {        this.title = title;    }     public String getContent() {        return content;    }     public void setContent(String content) {        this.content = content;    }     @Override    public String toString() {        StringBuilder builder = new StringBuilder();        builder.append("Email [receiver=").append(receiver).append(", title=")                .append(title).append(", content=").append(content).append("]");        return builder.toString();    }    }
       这个Email对象包含了一个简单的接收者email地址、邮件主题和邮件内容。我们在发送的时候就把这个对象封装成一个ObjectMessage进行发送。代码如下所示：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class ProducerServiceImpl implements ProducerService {  
- 
-     @Autowired  
-     private JmsTemplate jmsTemplate;      
- 
-     public void sendMessage(Destination destination, final Serializable obj) {  
-         jmsTemplate.send(destination, new MessageCreator() {  
- 
-             public Message createMessage(Session session) throws JMSException {  
-                 ObjectMessage objMessage = session.createObjectMessage(obj);  
-                 return objMessage;  
-             }  
- 
-         });  
-     }  
- 
- }  
public class ProducerServiceImpl implements ProducerService {     @Autowired    private JmsTemplate jmsTemplate;        public void sendMessage(Destination destination, final Serializable obj) {        jmsTemplate.send(destination, new MessageCreator() {             public Message createMessage(Session session) throws JMSException {                ObjectMessage objMessage = session.createObjectMessage(obj);                return objMessage;            }                    });    } }
       这是对应的在没有使用MessageConverter的时候我们需要new一个MessageCreator接口对象，然后在其抽象方法createMessage内部使用session创建一个对应的消息。在使用了MessageConverter的时候我们在使用JmsTemplate进行消息发送时只需要调用其对应的convertAndSend方法即可。如：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public void sendMessage(Destination destination, final Serializable obj) {  
-     //未使用MessageConverter的情况  
-     /*jmsTemplate.send(destination, new MessageCreator() { 
- 
-         public Message createMessage(Session session) throws JMSException { 
-             ObjectMessage objMessage = session.createObjectMessage(obj); 
-             return objMessage; 
-         } 
- 
-     });*/  
-     //使用MessageConverter的情况  
-     jmsTemplate.convertAndSend(destination, obj);  
- }  
    public void sendMessage(Destination destination, final Serializable obj) {        //未使用MessageConverter的情况        /*jmsTemplate.send(destination, new MessageCreator() {             public Message createMessage(Session session) throws JMSException {                ObjectMessage objMessage = session.createObjectMessage(obj);                return objMessage;            }                    });*/        //使用MessageConverter的情况        jmsTemplate.convertAndSend(destination, obj);    }
这样JmsTemplate就会在其内部调用预定的MessageConverter对我们的消息对象进行转换，然后再进行发送。
       这个时候我们就需要定义我们的MessageConverter了。要定义自己的MessageConverter很简单，只需要实现Spring为我们提供的MessageConverter接口即可。我们先来看一下MessageConverter接口的定义：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public interface MessageConverter {  
- 
-     Message toMessage(Object object, Session session) throws JMSException, MessageConversionException;  
- 
-     Object fromMessage(Message message) throws JMSException, MessageConversionException;  
- 
- }  
public interface MessageConverter {     Message toMessage(Object object, Session session) throws JMSException, MessageConversionException;     Object fromMessage(Message message) throws JMSException, MessageConversionException; }
       我们可以看到其中一共定义了两个方法fromMessage和toMessage，fromMessage是用来把一个JMS Message转换成对应的Java对象，而toMessage方法是用来把一个Java对象转换成对应的JMS Message。因为我们已经知道上面要发送的对象就是一个Email对象，所以在这里我们就简单地定义一个EmailMessageConverter用来把Email对象和对应的ObjectMessage进行转换，其代码如下：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- import javax.jms.JMSException;  
- import javax.jms.Message;  
- import javax.jms.ObjectMessage;  
- import javax.jms.Session;  
- 
- import org.springframework.jms.support.converter.MessageConversionException;  
- import org.springframework.jms.support.converter.MessageConverter;  
- 
- public class EmailMessageConverter implements MessageConverter {  
- 
-     public Message toMessage(Object object, Session session)  
-             throws JMSException, MessageConversionException {  
-         return session.createObjectMessage((Serializable) object);  
-     }  
- 
-     public Object fromMessage(Message message) throws JMSException,  
-             MessageConversionException {  
-         ObjectMessage objMessage = (ObjectMessage) message;  
-         return objMessage.getObject();  
-     }  
- 
- }  
import javax.jms.JMSException;import javax.jms.Message;import javax.jms.ObjectMessage;import javax.jms.Session; import org.springframework.jms.support.converter.MessageConversionException;import org.springframework.jms.support.converter.MessageConverter; public class EmailMessageConverter implements MessageConverter {     public Message toMessage(Object object, Session session)            throws JMSException, MessageConversionException {        return session.createObjectMessage((Serializable) object);    }     public Object fromMessage(Message message) throws JMSException,            MessageConversionException {        ObjectMessage objMessage = (ObjectMessage) message;        return objMessage.getObject();    } }
       这样当我们利用JmsTemplate的convertAndSend方法发送一个Email对象的时候就会把对应的Email对象当做参数调用我们定义好的EmailMessageConverter的toMessage方法。
       定义好我们的EmailMessageConverter之后就需要指定我们用来发送Email对象的JmsTemplate对象的messageConverter为EmailMessageConverter，这里我们在Spring的配置文件中定义JmsTemplate bean的时候就指定：
Xml代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- <!-- Spring提供的JMS工具类，它可以进行消息发送、接收等 -->  
- <bean id="jmsTemplate" class="org.springframework.jms.core.JmsTemplate">  
-     <!-- 这个connectionFactory对应的是我们定义的Spring提供的那个ConnectionFactory对象 -->  
-     <property name="connectionFactory" ref="connectionFactory"/>  
-     <!-- 消息转换器 -->  
-     <property name="messageConverter" ref="emailMessageConverter"/>  
- </bean>  
- <!-- 类型转换器 -->  
- <bean id="emailMessageConverter" class="com.tiantian.springintejms.converter.EmailMessageConverter"/>  
    <!-- Spring提供的JMS工具类，它可以进行消息发送、接收等 -->    <bean id="jmsTemplate" class="org.springframework.jms.core.JmsTemplate">        <!-- 这个connectionFactory对应的是我们定义的Spring提供的那个ConnectionFactory对象 -->        <property name="connectionFactory" ref="connectionFactory"/>        <!-- 消息转换器 -->        <property name="messageConverter" ref="emailMessageConverter"/>    </bean>    <!-- 类型转换器 -->    <bean id="emailMessageConverter" class="com.tiantian.springintejms.converter.EmailMessageConverter"/>
       到此我们的MessageConverter就定义好了，也能够进行使用了，接着我们来进行测试一下，定义测试代码如下所示：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- @Test  
- public void testObjectMessage() {  
-     Email email = new Email("zhangsan@xxx.com", "主题", "内容");  
-     producerService.sendMessage(destination, email);  
- }  
    @Test    public void testObjectMessage() {        Email email = new Email("zhangsan@xxx.com", "主题", "内容");        producerService.sendMessage(destination, email);    }
       上面destination对应的接收处理的MessageListener方法如下所示：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class ConsumerMessageListener implements MessageListener {  
- 
-     public void onMessage(Message message) {  
- 
-         if (message instanceof ObjectMessage) {  
-             ObjectMessage objMessage = (ObjectMessage) message;  
-             try {  
-                 Object obj = objMessage.getObject();  
-                 Email email = (Email) obj;  
-                 System.out.println("接收到一个ObjectMessage，包含Email对象。");  
-                 System.out.println(email);  
-             } catch (JMSException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- }  
public class ConsumerMessageListener implements MessageListener {     public void onMessage(Message message) {                if (message instanceof ObjectMessage) {            ObjectMessage objMessage = (ObjectMessage) message;            try {                Object obj = objMessage.getObject();                Email email = (Email) obj;                System.out.println("接收到一个ObjectMessage，包含Email对象。");                System.out.println(email);            } catch (JMSException e) {                e.printStackTrace();            }        }    } }
       之前说了MessageConverter有两方面的功能，除了把Java对象转换成对应的Jms Message之外还可以把Jms Message转换成对应的Java对象。我们看上面的消息监听器在接收消息的时候接收到的就是一个Jms Message，如果我们要利用MessageConverter来把它转换成对应的Java对象的话，只能是我们往里面注入一个对应的MessageConverter，然后在里面手动的调用，如：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class ConsumerMessageListener implements MessageListener {  
- 
-     private MessageConverter messageConverter;  
- 
-     public void onMessage(Message message) {  
- 
-         if (message instanceof ObjectMessage) {  
-             ObjectMessage objMessage = (ObjectMessage) message;  
-             try {  
-                 /*Object obj = objMessage.getObject(); 
-                 Email email = (Email) obj;*/  
-                 Email email = (Email) messageConverter.fromMessage(objMessage);  
-                 System.out.println("接收到一个ObjectMessage，包含Email对象。");  
-                 System.out.println(email);  
-             } catch (JMSException e) {  
-                 e.printStackTrace();  
-             }  
- 
-         }  
-     }  
- 
-     public MessageConverter getMessageConverter() {  
-         return messageConverter;  
-     }  
- 
-     public void setMessageConverter(MessageConverter messageConverter) {  
-         this.messageConverter = messageConverter;  
-     }  
- 
- }  
public class ConsumerMessageListener implements MessageListener {     private MessageConverter messageConverter;        public void onMessage(Message message) {                if (message instanceof ObjectMessage) {            ObjectMessage objMessage = (ObjectMessage) message;            try {                /*Object obj = objMessage.getObject();                Email email = (Email) obj;*/                Email email = (Email) messageConverter.fromMessage(objMessage);                System.out.println("接收到一个ObjectMessage，包含Email对象。");                System.out.println(email);            } catch (JMSException e) {                e.printStackTrace();            }                    }    }     public MessageConverter getMessageConverter() {        return messageConverter;    }     public void setMessageConverter(MessageConverter messageConverter) {        this.messageConverter = messageConverter;    } }
       当我们使用MessageListenerAdapter来作为消息监听器的时候，我们可以为它指定一个对应的MessageConverter，这样Spring在处理接收到的消息的时候就会自动地利用我们指定的MessageConverter对它进行转换，然后把转换后的Java对象作为参数调用指定的消息处理方法。这里我们再把前面讲解MessageListenerAdapter时定义的MessageListenerAdapter拿来做一个测试，我们指定它的MessageConverter为我们定义好的EmailMessageConverter。
Xml代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- <!-- 消息监听适配器 -->  
- <bean id="messageListenerAdapter" class="org.springframework.jms.listener.adapter.MessageListenerAdapter">  
-     <property name="delegate">  
-         <bean class="com.tiantian.springintejms.listener.ConsumerListener"/>  
-     </property>  
-     <property name="defaultListenerMethod" value="receiveMessage"/>  
-     <property name="messageConverter" ref="emailMessageConverter"/>  
- </bean>  
- <!-- 消息监听适配器对应的监听容器 -->  
- <bean id="messageListenerAdapterContainer" class="org.springframework.jms.listener.DefaultMessageListenerContainer">  
-     <property name="connectionFactory" ref="connectionFactory"/>  
-     <property name="destination" ref="adapterQueue"/>  
-     <property name="messageListener" ref="messageListenerAdapter"/><!-- 使用MessageListenerAdapter来作为消息监听器 -->  
- </bean>  
    <!-- 消息监听适配器 -->    <bean id="messageListenerAdapter" class="org.springframework.jms.listener.adapter.MessageListenerAdapter">        <property name="delegate">            <bean class="com.tiantian.springintejms.listener.ConsumerListener"/>        </property>        <property name="defaultListenerMethod" value="receiveMessage"/>        <property name="messageConverter" ref="emailMessageConverter"/>    </bean>    <!-- 消息监听适配器对应的监听容器 -->    <bean id="messageListenerAdapterContainer" class="org.springframework.jms.listener.DefaultMessageListenerContainer">        <property name="connectionFactory" ref="connectionFactory"/>        <property name="destination" ref="adapterQueue"/>        <property name="messageListener" ref="messageListenerAdapter"/><!-- 使用MessageListenerAdapter来作为消息监听器 -->    </bean>
       然后在我们的真正用于处理接收到的消息的ConsumerListener中添加一个receiveMessage方法，添加后其代码如下所示：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class ConsumerListener {  
- 
-     public void receiveMessage(String message) {  
-         System.out.println("ConsumerListener通过receiveMessage接收到一个纯文本消息，消息内容是：" + message);  
-     }  
- 
-     public void receiveMessage(Email email) {  
-         System.out.println("接收到一个包含Email的ObjectMessage。");  
-         System.out.println(email);  
-     }  
- 
- }  
public class ConsumerListener {     public void receiveMessage(String message) {        System.out.println("ConsumerListener通过receiveMessage接收到一个纯文本消息，消息内容是：" + message);    }        public void receiveMessage(Email email) {        System.out.println("接收到一个包含Email的ObjectMessage。");        System.out.println(email);    }    }
       然后我们定义如下测试代码：
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- @Test  
- public void testObjectMessage() {  
-     Email email = new Email("zhangsan@xxx.com", "主题", "内容");  
-     producerService.sendMessage(adapterQueue, email);  
- }  
    @Test    public void testObjectMessage() {        Email email = new Email("zhangsan@xxx.com", "主题", "内容");        producerService.sendMessage(adapterQueue, email);    }
       因为我们给MessageListenerAdapter指定了一个MessageConverter，而且是一个EmailMessageConverter，所以当MessageListenerAdapter接收到一个消息后，它会调用我们指定的MessageConverter的fromMessage方法把它转换成一个Java对象，根据定义这里会转换成一个Email对象，然后会把这个Email对象作为参数调用我们通过defaultListenerMethod属性指定的默认处理器方法，根据定义这里就是receiveMessage方法，但是我们可以看到在ConsumerListener中我们一共定义了两个receiveMessage方法，因为是通过转换后的Email对象作为参数进行方法调用的，所以这里调用的就应该是参数类型为Email的receiveMessage方法了。上述测试代码运行后会输出如下结果：
![](http://dl2.iteye.com/upload/attachment/0086/6469/ee3a2dc6-82ce-3d4c-a7a5-30f5adca1499.png)
        说到这里可能有读者就会有疑问了，说我们在之前讲解MessageListenerAdapter的时候不是没有指定对应的MessageConverter，然后发送了一个TextMessage，结果Spring还是把它转换成一个String对象，调用了ConsumerListener参数类型为String的receiveMessage方法吗？那你这个MessageConverter在MessageListenerAdapter进行消息接收的时候也没什么用啊。
       其实还是有用的，在我们使用MessageListenerAdapter时，在对其进行初始化也就是调用其构造方法时，它会默认new一个Spring已经为我们实现了的MessageConverter——SimpleMessageConverter作为其默认的MessageConverter，这也就是为什么我们在使用MessageListenerAdapter的时候不需要指定MessageConverter但是消息还是会转换成对应的Java对象的原因。所以默认情况下我们使用MessageListenerAdapter时其对应的MessageListener的处理器方法参数类型必须是一个普通Java对象，而不能是对应的Jms
 Message对象。
       那如果我们在处理Jms Message的时候想使用MessageListenerAdapter，然后又希望处理最原始的Message，而不是经过MessageConverter进行转换后的Message该怎么办呢？这个时候我们只需要在定义MessageListenerAdapter的时候指定其MessageConverter为空就可以了。
Xml代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- <!-- 消息监听适配器 -->  
- <bean id="messageListenerAdapter" class="org.springframework.jms.listener.adapter.MessageListenerAdapter">  
-     <property name="delegate">  
-         <bean class="com.tiantian.springintejms.listener.ConsumerListener"/>  
-     </property>  
-     <property name="defaultListenerMethod" value="receiveMessage"/>  
-     <property name="messageConverter">  
-         <null/>  
-     </property>  
- </bean>  
    <!-- 消息监听适配器 -->    <bean id="messageListenerAdapter" class="org.springframework.jms.listener.adapter.MessageListenerAdapter">        <property name="delegate">            <bean class="com.tiantian.springintejms.listener.ConsumerListener"/>        </property>        <property name="defaultListenerMethod" value="receiveMessage"/>        <property name="messageConverter">            <null/>        </property>    </bean>
       那么这个时候我们的真实MessageListener的处理器方法参数类型就应该是Jms Message或对应的Jms Message子类型了，不然就会调用不到对应的处理方法了。这里因为我们发送的是一个ObjectMessage，所以这里就添加一个对应的参数类型为ObjectMessage的receiveMessage方法了。
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public void receiveMessage(ObjectMessage message) throws JMSException {  
-     System.out.println(message.getObject());  
- }  
    public void receiveMessage(ObjectMessage message) throws JMSException {        System.out.println(message.getObject());    }
       刚刚讲到Spring已经为我们实现了一个简单的MessageConverter，即org.springframework.jms.support.converter.SimpleMessageConverter，其实Spring在初始化JmsTemplate的时候也指定了其对应的MessageConverter为一个SimpleMessageConverter，所以如果我们平常没有什么特殊要求的时候可以直接使用JmsTemplate的convertAndSend系列方法进行消息发送，而不必繁琐的在调用send方法时自己new一个MessageCreator进行相应Message的创建。
这里我们也来看一下SimpleMessageConverter的定义，如果觉得它不能满足你的要求，那我们可以对它里面的部分方法进行重写，或者是完全实现自己的MessageConverter。
Java代码 ![复制代码](http://haohaoxuexi.iteye.com/images/icon_copy.gif)![收藏代码](http://haohaoxuexi.iteye.com/images/icon_star.png)![](http://haohaoxuexi.iteye.com/images/spinner.gif)
- public class SimpleMessageConverter implements MessageConverter {  
- 
-     public Message toMessage(Object object, Session session) throws JMSException, MessageConversionException {  
-         if (object instanceof Message) {  
-             return (Message) object;  
-         }  
-         else if (object instanceof String) {  
-             return createMessageForString((String) object, session);  
-         }  
-         else if (object instanceof byte[]) {  
-             return createMessageForByteArray((byte[]) object, session);  
-         }  
-         else if (object instanceof Map) {  
-             return createMessageForMap((Map) object, session);  
-         }  
-         else if (object instanceof Serializable) {  
-             return createMessageForSerializable(((Serializable) object), session);  
-         }  
- 
-         else {  
-             throw new MessageConversionException("Cannot convert object of type [" +  
-                     ObjectUtils.nullSafeClassName(object) + "] to JMS message. Supported message " +  
-                     "payloads are: String, byte array, Map<String,?>, Serializable object.");  
-         }  
-     }  
- 
-     public Object fromMessage(Message message) throws JMSException, MessageConversionException {  
-         if (message instanceof TextMessage) {  
-             return extractStringFromMessage((TextMessage) message);  
-         }  
-         else if (message instanceof BytesMessage) {  
-             return extractByteArrayFromMessage((BytesMessage) message);  
-         }  
-         else if (message instanceof MapMessage) {  
-             return extractMapFromMessage((MapMessage) message);  
-         }  
-         else if (message instanceof ObjectMessage) {  
-             return extractSerializableFromMessage((ObjectMessage) message);  
-         }  
-         else {  
-             return message;  
-         }  
-     }  
- 
-     protected TextMessage createMessageForString(String text, Session session) throws JMSException {  
-         return session.createTextMessage(text);  
-     }  
- 
-     protected BytesMessage createMessageForByteArray(byte[] bytes, Session session) throws JMSException {  
-         BytesMessage message = session.createBytesMessage();  
-         message.writeBytes(bytes);  
-         return message;  
-     }  
- 
-     protected MapMessage createMessageForMap(Map<?, ?> map, Session session) throws JMSException {  
-         MapMessage message = session.createMapMessage();  
-         for (Map.Entry entry : map.entrySet()) {  
-             if (!(entry.getKey() instanceof String)) {  
-                 throw new MessageConversionException("Cannot convert non-String key of type [" +  
-                         ObjectUtils.nullSafeClassName(entry.getKey()) + "] to JMS MapMessage entry");  
-             }  
-             message.setObject((String) entry.getKey(), entry.getValue());  
-         }  
-         return message;  
-     }  
- 
-     protected ObjectMessage createMessageForSerializable(Serializable object, Session session) throws JMSException {  
-         return session.createObjectMessage(object);  
-     }  
- 
- 
-     protected String extractStringFromMessage(TextMessage message) throws JMSException {  
-         return message.getText();  
-     }  
- 
-     protected byte[] extractByteArrayFromMessage(BytesMessage message) throws JMSException {  
-         byte[] bytes = new byte[(int) message.getBodyLength()];  
-         message.readBytes(bytes);  
-         return bytes;  
-     }  
- 
-     protected Map extractMapFromMessage(MapMessage message) throws JMSException {  
-         Map<String, Object> map = new HashMap<String, Object>();  
-         Enumeration en = message.getMapNames();  
-         while (en.hasMoreElements()) {  
-             String key = (String) en.nextElement();  
-             map.put(key, message.getObject(key));  
-         }  
-         return map;  
-     }  
- 
-     protected Serializable extractSerializableFromMessage(ObjectMessage message) throws JMSException {  
-         return message.getObject();  
-     }  
- 
- } 
