# rabbitmq+JAVA中发送端的区别 - zp704393004的专栏 - CSDN博客





2018年05月07日 10:16:47[原来未知](https://me.csdn.net/zp704393004)阅读数：64








发送端


方式一：

     //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null); 
        //发送消息  

        channel.basicPublish("", QUEUE_NAME, null, msg.getBytes());  


方式二：

 //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null);  
        channel.exchangeDeclare(EXCHANGE_NAME, "direct",true); 
        channel.queueBind(QUEUE_NAME, EXCHANGE_NAME, KEY_NAME); 
        //发送消息  
        channel.basicPublish(EXCHANGE_NAME, KEY_NAME, null, msg.getBytes());  


接收端：

channel.queueDeclare(QUEUE_NAME, false, false, false, null);  
        channel.exchangeDeclare(EXCHANGE_NAME, "direct",true); 
        channel.queueBind(QUEUE_NAME, EXCHANGE_NAME, KEY_NAME); 
channel.basicConsume(QUEUE_NAME, true, consumer);


参考：https://blog.csdn.net/phantomes/article/details/47284829





