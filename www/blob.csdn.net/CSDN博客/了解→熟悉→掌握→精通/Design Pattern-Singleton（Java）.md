
# Design Pattern - Singleton（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:31:11[Chimomo](https://me.csdn.net/chimomo)阅读数：40


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Ensure a class has only one instance and provides a global point of access to it.
# Participants
The classes and/or objects participating in this pattern are:
Singleton(LoadBalancer)Defines an Instance operation that lets clients access its unique instance. Instance is a class operation.
Responsible for creating and maintaining its own unique instance.

# Sample Code in Java
---
This structural code demonstrates the Singleton pattern which assures only a single instance (the singleton) of the class can be created.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.sample;
/**
 * Startup class for Structural Singleton Design Pattern.
 *
 * @author Chimomo
 */
class MainApp {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        // Constructor is protected, cannot use new.
        Singleton s1 = Singleton.instance();
        Singleton s2 = Singleton.instance();
        // Test for same instance.
        if (s1 == s2) {
            System.out.println("Objects are the same instance.");
        }
    }
}
/*
Output:
Objects are the same instance.
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.sample;
/**
 * The 'Singleton' class.
 *
 * @author Chimomo
 */
public class Singleton {
    // The instance.
    private static Singleton instance;
    /**
     * Initializes a new instance of the Singleton class. Constructor is
     * 'protected'.
     */
    protected Singleton() {
    }
    /**
     * Get instance.
     *
     * @return
     */
    public static Singleton instance() {
        // Uses lazy initialization.  
        // Note: this is not thread safe.  
        if (instance == null) {
            instance = new Singleton();
        }
        return instance;
    }
}
```
---
This real-world code demonstrates the Singleton pattern as a LoadBalancing object. Only a single instance (the singleton) of the class can be created because servers may dynamically come on-line or off-line and every request must go through the one object that has knowledge about the state of the (web) farm.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.realworld;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
/**
 * The 'Singleton' class. Simple, but effective random load balancer.
 *
 * @author Chimomo
 */
class LoadBalancer {
    // The instance.
    private static LoadBalancer instance;
    // The sync lock.
    private static final Object SYNC_LOCK_OBJECT = new Object();
    // The random.
    private final Random random = new Random();
    // The servers.
    private final List<String> servers = new ArrayList<>();
    /**
     * Initializes a new instance of the "LoadBalancer" class.
     */
    protected LoadBalancer() {
        // List of available servers
        this.servers.add("ServerI");
        this.servers.add("ServerII");
        this.servers.add("ServerIII");
        this.servers.add("ServerIV");
        this.servers.add("ServerV");
    }
    /**
     * Get the server.
     *
     * @return
     */
    public String getServer() {
        int r = this.random.nextInt(this.servers.size());
        return this.servers.get(r);
    }
    public static LoadBalancer getLoadBalancer() {
        if (instance == null) {
            synchronized (SYNC_LOCK_OBJECT) {
                if (instance == null) {
                    instance = new LoadBalancer();
                }
            }
        }
        return instance;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.realworld;
/**
 * Startup class for Real-World Singleton Design Pattern.
 *
 * @author Chimomo
 */
public class MainApp {
    public static void main(String[] args) {
        LoadBalancer b1 = LoadBalancer.getLoadBalancer();
        LoadBalancer b2 = LoadBalancer.getLoadBalancer();
        LoadBalancer b3 = LoadBalancer.getLoadBalancer();
        LoadBalancer b4 = LoadBalancer.getLoadBalancer();
        // Same instance?
        if (b1 == b2 && b2 == b3 && b3 == b4) {
            System.out.println("Same instance.");
        }
        // Load balance 15 server requests
        LoadBalancer balancer = LoadBalancer.getLoadBalancer();
        for (int i = 0; i < 15; i++) {
            String server = balancer.getServer();
            System.out.println("Dispatch Request to: " + server);
        }
    }
}
/*
Output:
Same instance.
Dispatch Request to: ServerIII
Dispatch Request to: ServerI
Dispatch Request to: ServerIV
Dispatch Request to: ServerIV
Dispatch Request to: ServerII
Dispatch Request to: ServerV
Dispatch Request to: ServerIV
Dispatch Request to: ServerII
Dispatch Request to: ServerII
Dispatch Request to: ServerIII
Dispatch Request to: ServerV
Dispatch Request to: ServerIV
Dispatch Request to: ServerII
Dispatch Request to: ServerIII
Dispatch Request to: ServerIV
 */
```
---
This optimized code demonstrates the same code as above but uses more modern, built-in features.
Here an elegant specific solution is offered. The Singleton pattern simply uses a private constructor and a static final instance variable that is eagerly initialized. Thread safety is guaranteed by the compiler.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.optimized;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
/**
 * The 'Singleton' class. Simple, but effective load balancer.
 *
 * @author Chimomo
 */
final class LoadBalancer {
    // The instance.
    private static final LoadBalancer INSTANCE = new LoadBalancer();
    // The random.
    private final Random random = new Random();
    // The servers.
    private List<Server> servers;
    /**
     * Prevents a default instance of the "LoadBalancer" class from being
     * created.
     */
    private LoadBalancer() {
        // Load list of available servers.
        Server server1 = new Server("Server I", "120.14.220.18");
        Server server2 = new Server("Server II", "120.14.220.19");
        Server server3 = new Server("Server III", "120.14.220.20");
        Server server4 = new Server("Server IV", "120.14.220.21");
        Server server5 = new Server("Server V", "120.14.220.22");
        this.servers = new ArrayList<>();
        this.servers.add(server1);
        this.servers.add(server2);
        this.servers.add(server3);
        this.servers.add(server4);
        this.servers.add(server5);
    }
    /**
     * Gets the next server.
     *
     * @return
     */
    public Server getNextServer() {
        int r = this.random.nextInt(this.servers.size());
        return this.servers.get(r);
    }
    /**
     * Get load balancer.
     *
     * @return
     */
    public static LoadBalancer getLoadBalancer() {
        return INSTANCE;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.optimized;
/**
 * Startup class for Java optimized Singleton Design Pattern.
 *
 * @author Chimomo
 */
class MainApp {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        LoadBalancer b1 = LoadBalancer.getLoadBalancer();
        LoadBalancer b2 = LoadBalancer.getLoadBalancer();
        LoadBalancer b3 = LoadBalancer.getLoadBalancer();
        LoadBalancer b4 = LoadBalancer.getLoadBalancer();
        // Confirm these are the same instance.
        if (b1 == b2 && b2 == b3 && b3 == b4) {
            System.out.println("Same instance.");
        }
        // Next, load balance 15 requests for a server.
        LoadBalancer balancer = LoadBalancer.getLoadBalancer();
        for (int i = 0; i < 15; i++) {
            String serverName = balancer.getNextServer().getName();
            System.out.println("Dispatch request to: " + serverName);
        }
    }
}
/*
Output:
Same instance.
Dispatch request to: Server I
Dispatch request to: Server IV
Dispatch request to: Server V
Dispatch request to: Server IV
Dispatch request to: Server IV
Dispatch request to: Server I
Dispatch request to: Server II
Dispatch request to: Server IV
Dispatch request to: Server IV
Dispatch request to: Server I
Dispatch request to: Server V
Dispatch request to: Server IV
Dispatch request to: Server I
Dispatch request to: Server I
Dispatch request to: Server II
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.singleton.optimized;
/**
 * Represents a server machine.
 *
 * @author Chimomo
 */
class Server {
    private String ip;
    private String name;
    public Server(String name, String ip) {
        this.name = name;
        this.ip = ip;
    }
    public void setIP(String ip) {
        this.ip = ip;
    }
    public String getIP() {
        return this.ip;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getName() {
        return this.name;
    }
}
```

