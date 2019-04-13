
# Design Pattern - Mediator（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:20:56[Chimomo](https://me.csdn.net/chimomo)阅读数：76


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
### Definition
Define an object that encapsulates how a set of objects interact. Mediator promotes loose coupling by keeping objects from referring to each other explicitly, and it lets you vary their interaction independently.
### Participants
The classes and/or objects participating in this pattern are:
Mediator(IChatroom)Defines an interface for communicating with Colleague objects
ConcreteMediator(Chatroom)Implements cooperative behavior by coordinating Colleague objects
Knows and maintains its colleagues
Colleague classes(Participant)Each Colleague class knows its Mediator object
Each colleague communicates with its mediator whenever it would have otherwise communicated with another colleague

### Sample Code in Java
---
This structural code demonstrates the Mediator pattern facilitating loosely coupled communication between different objects and object types. The mediator is a central hub through which all interaction must take place.
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * The 'Colleague' class
 *
 * @author Chimomo
 */
class Colleague {
    // The mediator
    protected Mediator mediator;
    /**
     * Initializes a new instance of the "Colleague" class
     *
     * @param mediator The mediator
     */
    Colleague(Mediator mediator) {
        this.mediator = mediator;
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * A 'ConcreteColleague' class
 *
 * @author Chimomo
 */
class ConcreteColleague1 extends Colleague {
    /**
     * Initializes a new instance of the "ConcreteColleague1" class
     *
     * @param mediator The mediator
     */
    ConcreteColleague1(Mediator mediator) {
        super(mediator);
    }
    /**
     * Notify
     *
     * @param message The message
     */
    void notify(String message) {
        System.out.println("Colleague1 gets message: " + message);
    }
    /**
     * Send
     *
     * @param message The send
     */
    public void send(String message) {
        this.mediator.send(message, this);
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * A 'ConcreteColleague' class
 *
 * @author Chimomo
 */
class ConcreteColleague2 extends Colleague {
    /**
     * Initializes a new instance of the "ConcreteColleague1" class
     *
     * @param mediator The mediator
     */
    ConcreteColleague2(Mediator mediator) {
        super(mediator);
    }
    /**
     * Notify
     *
     * @param message The message
     */
    void notify(String message) {
        System.out.println("Colleague2 gets message: " + message);
    }
    /**
     * Send
     *
     * @param message The message
     */
    public void send(String message) {
        this.mediator.send(message, this);
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * The 'ConcreteMediator' class
 *
 * @author Chimomo
 */
class ConcreteMediator extends Mediator {
    // The colleague1
    private ConcreteColleague1 colleague1;
    // The colleague2
    private ConcreteColleague2 colleague2;
    /**
     * Set concrete colleague1
     *
     * @param colleague1 The colleague1
     */
    void setConcreteColleague1(ConcreteColleague1 colleague1) {
        this.colleague1 = colleague1;
    }
    /**
     * Set concrete colleague2
     *
     * @param colleague2 The colleague2
     */
    void setConcreteColleague2(ConcreteColleague2 colleague2) {
        this.colleague2 = colleague2;
    }
    /**
     * Send
     *
     * @param message The message
     * @param colleague The colleague
     */
    @Override
    public void send(String message, Colleague colleague) {
        if (colleague == this.colleague1) {
            this.colleague2.notify(message);
        } else {
            this.colleague1.notify(message);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * The 'Mediator' abstract class
 *
 * @author Chimomo
 */
abstract class Mediator {
    /**
     * Send
     *
     * @param message The message
     * @param colleague The colleague
     */
    public abstract void send(String message, Colleague colleague);
}
```
```python
package chimomo.learning.java.designpattern.mediator.sample;
/**
 * Startup class for Structural Mediator Design Pattern
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application
     *
     * @param args The arguments
     */
    public static void main(String[] args) {
        ConcreteMediator m = new ConcreteMediator();
        ConcreteColleague1 c1 = new ConcreteColleague1(m);
        ConcreteColleague2 c2 = new ConcreteColleague2(m);
        m.setConcreteColleague1(c1);
        m.setConcreteColleague2(c2);
        c1.send("How are you?");
        c2.send("Fine, thank you");
    }
}
/* ------ Running Results ------
Colleague2 gets message: How are you?
Colleague1 gets message: Fine, thank you
*/
```

---
This real-world code demonstrates the Mediator pattern facilitating loosely coupled communication between different Participants registering with a Chat room. The Chatroom is the central hub through which all communication takes place. At this point only one-to-one communication is implemented in the Chatroom, but would be trivial to change to one-to-many.
```python
package chimomo.learning.java.designpattern.mediator.realworld;
/**
 * The 'Mediator' abstract class
 *
 * @author Chimomo
 */
abstract class AbstractChatRoom {
    /**
     * Register participant
     *
     * @param participant The participant
     */
    public abstract void register(Participant participant);
    /**
     * Send message
     *
     * @param from The from
     * @param to The to
     * @param message The message
     */
    public abstract void send(String from, String to, String message);
}
```
```python
package chimomo.learning.java.designpattern.mediator.realworld;
/**
 * A 'ConcreteColleague' class
 *
 * @author Chimomo
 */
class Beatle extends Participant {
    /**
     * Initializes a new instance of the "Beatle" class
     *
     * @param name The name
     */
    Beatle(String name) {
        super(name);
    }
    /**
     * Receive
     *
     * @param from The from
     * @param message The message
     */
    public void receive(String from, String message) {
        System.out.print("To a Beatle: ");
        super.receive(from, message);
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.realworld;
import java.util.HashMap;
import java.util.Map;
/**
 * The 'ConcreteMediator' class
 *
 * @author Chimomo
 */
class ChatRoom extends AbstractChatRoom {
    // The participants
    private Map<String, Participant> participants = new HashMap<>();
    /**
     * Register participant
     *
     * @param participant The participant
     */
    @Override
    public void register(Participant participant) {
        if (!this.participants.containsValue(participant)) {
            this.participants.put(participant.getName(), participant);
        }
        participant.setChatRoom(this);
    }
    /**
     * Send message
     *
     * @param from The from
     * @param to The to
     * @param message The message
     */
    @Override
    public void send(String from, String to, String message) {
        Participant participant = this.participants.get(to);
        if (participant != null) {
            participant.receive(from, message);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.realworld;
/**
 * A 'ConcreteColleague' class
 *
 * @author Chimomo
 */
class NonBeatle extends Participant {
    /**
     * Initializes a new instance of the "NonBeatle" class
     *
     * @param name The name
     */
    NonBeatle(String name) {
        super(name);
    }
    /**
     * Receive
     *
     * @param from The from
     * @param message The message
     */
    public void receive(String from, String message) {
        System.out.print("To a NonBeatle: ");
        super.receive(from, message);
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.realworld;
/**
 * The 'AbstractColleague' class
 *
 * @author Chimomo
 */
class Participant {
    // The chat room
    private ChatRoom chatRoom;
    // The participant name
    private String name;
    /**
     * Initializes a new instance of the "Participant" class
     *
     * @param name The name
     */
    Participant(String name) {
        this.name = name;
    }
    /**
     * Set chat room
     *
     * @param chatRoom The chat room
     */
    void setChatRoom(ChatRoom chatRoom) {
        this.chatRoom = chatRoom;
    }
    /**
     * Get name
     *
     * @return The name
     */
    public String getName() {
        return this.name;
    }
    /**
     * Set name
     *
     * @param name The name
     */
    public void setName(String name) {
        this.name = name;
    }
    /**
     * Receive
     *
     * @param from The from
     * @param message The message
     */
    public void receive(String from, String message) {
        System.out.println(String.format("%s to %s: '%s'", from, this.name, message));
    }
    /**
     * Send
     *
     * @param to The to
     * @param message The message
     */
    public void send(String to, String message) {
        this.chatRoom.send(this.name, to, message);
    }
}
```
```python
package chimomo.learning.java.designpattern.mediator.realworld;
/**
 * Startup class for Real-World Mediator Design Pattern
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application
     *
     * @param args The arguments
     */
    public static void main(String[] args) {
        // Create chat room
        ChatRoom chatRoom = new ChatRoom();
        // Create participants and register them
        Participant george = new Beatle("George");
        Participant paul = new Beatle("Paul");
        Participant ringo = new Beatle("Ringo");
        Participant john = new Beatle("John");
        Participant yoko = new NonBeatle("Yoko");
        chatRoom.register(george);
        chatRoom.register(paul);
        chatRoom.register(ringo);
        chatRoom.register(john);
        chatRoom.register(yoko);
        // Chatting participants
        yoko.send("John", "Hi John!");
        paul.send("Ringo", "All you need is love");
        ringo.send("George", "My sweet lord");
        paul.send("John", "Can't buy me love");
        john.send("Yoko", "My sweet love");
    }
}
/* ------ Running Results ------
To a Beatle: Yoko to John: 'Hi John!'
To a Beatle: Paul to Ringo: 'All you need is love'
To a Beatle: Ringo to George: 'My sweet lord'
To a Beatle: Paul to John: 'Can't buy me love'
To a NonBeatle: John to Yoko: 'My sweet love'
*/
```


