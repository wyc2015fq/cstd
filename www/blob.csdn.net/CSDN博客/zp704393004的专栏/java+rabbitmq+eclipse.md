# java+rabbitmq+eclipse - zp704393004的专栏 - CSDN博客





2018年05月03日 14:37:31[原来未知](https://me.csdn.net/zp704393004)阅读数：193








1、新建project->maven->maven Project

2、pom.xml里加入：

<dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>3.8.1</version>
      <scope>test</scope>
    </dependency>
    <dependency>//从这里开始是加入的内容
            <groupId>com.rabbitmq</groupId>
           <artifactId>amqp-client</artifactId>
            <version>3.6.5</version>


        </dependency>//加入内容结束

3、消费者：

package RabbitMQMaven_P.test;

/**
 * Hello world!
 *
 */
import java.io.IOException;  
import java.util.UUID;  
import java.util.concurrent.ExecutorService;  
import java.util.concurrent.Executors;  
import java.util.concurrent.ScheduledExecutorService;  
import java.util.concurrent.TimeUnit;  
import java.util.concurrent.TimeoutException;  

import com.rabbitmq.client.Channel;  
import com.rabbitmq.client.Connection;  
import com.rabbitmq.client.ConnectionFactory; 
import com.rabbitmq.client.ConsumerCancelledException;  
import com.rabbitmq.client.QueueingConsumer;  
import com.rabbitmq.client.QueueingConsumer.Delivery;  
import com.rabbitmq.client.ShutdownSignalException;  
import com.rabbitmq.client.AMQP;  
import com.rabbitmq.client.Consumer;  
import com.rabbitmq.client.DefaultConsumer;  
import com.rabbitmq.client.Envelope; 

public class App 
{
    private final static String QUEUE_NAME = "hello";  
    public static void main( String[] args )
    {
        ConnectionFactory cf = new ConnectionFactory();  
        //rabbitmq监听IP  
        cf.setHost("localhost");  
        //rabbitmq默认监听端口，注意要记得开启端口  
        cf.setPort(5672);  

        //设置访问的用户  
        cf.setUsername("admin");  
        cf.setPassword("admin");  

        try
        {
            //建立连接
        Connection conn = cf.newConnection();
            //创建消息通道  
        Channel channel = conn.createChannel();
        //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null);  
        System.out.println(" Waiting for msg....");  
        //创建消费者，并接受消息  
        Consumer consumer = new DefaultConsumer(channel) {  
            @Override  
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body)  
                    throws IOException {  
                String msg = new String(body, "UTF-8");  
                System.out.println("Received is = '" + msg + "'");  
            }  
        };  
        channel.basicConsume(QUEUE_NAME, true, consumer);
        }catch(Exception e1)
        {
            System.out.println( "try 1" );
        }
        System.out.println( "Hello World!" );
    }


}

4、生产者：

package RabbitMQMaven_P.test_P;

/**
 * Hello world!
 *
 */
import com.rabbitmq.client.Channel;  
import com.rabbitmq.client.Connection;  
import com.rabbitmq.client.ConnectionFactory;  
public class App 
{
    private final static String QUEUE_NAME = "hello";  
    public static void main( String[] args )
    {
        ConnectionFactory cf = new ConnectionFactory();  
        //rabbitmq监听IP  
        cf.setHost("localhost");  
        //rabbitmq默认监听端口，注意要记得开启端口  
        cf.setPort(5672);  

        //设置访问的用户  
        cf.setUsername("admin");  
        cf.setPassword("admin");  
        try
        {
        //建立连接  
        Connection conn = cf.newConnection();  
        //创建消息通道  
        Channel channel = conn.createChannel();  

        String msg = "hello world!!!! 你好啊~";  
        //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null);  

        //发送消息  
        channel.basicPublish("", QUEUE_NAME, null, msg.getBytes());  
        System.out.println("send msg "+ msg + " to ["+ QUEUE_NAME +"] queue !");  

        channel.close();  
        conn.close();  
        }catch(Exception e1)
        {
            System.out.println( "try 2" );
        }
        System.out.println( "Hello World2!" );
    }


}

备注：

如果不是try{}catch(){}，会出现timeout等问题

参考：

1、https://blog.csdn.net/qq1052441272/article/details/53743183

2、










