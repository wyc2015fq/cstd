# MQTT Paho Getting Started - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年09月10日 21:04:15[boonya](https://me.csdn.net/boonya)阅读数：1149








![](http://www.eclipse.org/paho/images/mqttorg-glow.png)


MQTT is a light-weight publish/subscribe messaging protocol, originally created by IBM and Arcom (later to become part of Eurotech) around 1998. The[MQTT
 3.1.1 specification](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html) has now been standardised by the [OASIS consortium](https://www.oasis-open.org/committees/mqtt/charter.php). The standard is available in a variety of [formats](https://www.oasis-open.org/standards#mqttv3.1.1). 

More information about the protocol can be found on the [MQTT.org community site](http://mqtt.org). 

There is a publically accessible sandbox server for the Eclipse IoT projects available at`iot.eclipse.org`, port`1883`.









### Getting Started

The Paho project consists of a number of clients and utilities for working with MQTT, each of which comes with its own getting started guide.

Follow the links below for the component you're interested in. 









### MQTT Clients
- C/C++ Clients
- [C for Posix and Windows](http://www.eclipse.org/paho/clients/c/)
- [C++ for Posix and Windows](http://www.eclipse.org/paho/clients/cpp/)
- [C/C++ for embedded systems](http://www.eclipse.org/paho/clients/c/embedded)

- Java Clients
- [J2SE Client](http://www.eclipse.org/paho/clients/java/)
- [Android Service](http://www.eclipse.org/paho/clients/android/)

- [JavaScript Client](http://www.eclipse.org/paho/clients/js/)
- [Python Client](http://www.eclipse.org/paho/clients/python/)
- [Go Client](http://www.eclipse.org/paho/clients/golang/)
- [C# .Net and WinRT Client](http://www.eclipse.org/paho/clients/dotnet/)



### Utilities
- [MQTT Conformance/Interoperability Testing](http://www.eclipse.org/paho/clients/testing)



### MQTT-SN Clients
- C/C++ Clients
- [C for embedded systems](http://www.eclipse.org/paho/clients/c/embedded-sn)








Java Getting Started Sample



### Java Getting Started





```java
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

public class MqttPublishSample {

    public static void main(String[] args) {

        String topic        = "MQTT Examples";
        String content      = "Message from MqttPublishSample";
        int qos             = 2;
        String broker       = "tcp://iot.eclipse.org:1883";
        String clientId     = "JavaSample";
        MemoryPersistence persistence = new MemoryPersistence();

        try {
            MqttClient sampleClient = new MqttClient(broker, clientId, persistence);
            MqttConnectOptions connOpts = new MqttConnectOptions();
            connOpts.setCleanSession(true);
            System.out.println("Connecting to broker: "+broker);
            sampleClient.connect(connOpts);
            System.out.println("Connected");
            System.out.println("Publishing message: "+content);
            MqttMessage message = new MqttMessage(content.getBytes());
            message.setQos(qos);
            sampleClient.publish(topic, message);
            System.out.println("Message published");
            sampleClient.disconnect();
            System.out.println("Disconnected");
            System.exit(0);
        } catch(MqttException me) {
            System.out.println("reason "+me.getReasonCode());
            System.out.println("msg "+me.getMessage());
            System.out.println("loc "+me.getLocalizedMessage());
            System.out.println("cause "+me.getCause());
            System.out.println("excep "+me);
            me.printStackTrace();
        }
    }
}
```







