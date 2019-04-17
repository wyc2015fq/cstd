# Mina State machine状态机讲解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月16日 15:30:05[boonya](https://me.csdn.net/boonya)阅读数：3378
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina State machine(Apache Mina User guide Chapter14 State
 machine)](http://mina.apache.org/mina-project/userguide/ch14-state-machine/ch14-state-machine.html)

如果您使用的是Mina与复杂网络开发应用程序交互，你可能在某些时候发现自己达到的美好状态模式尝试解决一些复杂性。然而，在你这样做之前你可能想检查mina-statemachine试图解决一些状态模式的不足之处。

### 1.1.1. **一个简单的例子**

让我们演示mina-statemachine如何以一个简单的例子工作。下图显示了一个典型的录音机一个状态机。椭圆形表示状态而箭头表示转换。每个转换都与一个事件标记名称，触发转换。

![](https://img-blog.csdn.net/20160616151400434)

最初,录音机处于空的状态。当插入磁带加载事件触发和录音机移动到加载状态。在加载驱逐事件将触发一个回到空而播放事件将触发播放状态。等等……我认为接下来的你都明白了。

现在让我们编写一些代码。外部世界(代码与磁带甲板)只会看到TapeDeck接口:

```java
public interface TapeDeck {

    void load(String nameOfTape);

    void eject();

    void start();

    void pause();

    void stop();}
```

接下来我们将编写类包含实际的代码执行发生转变时的状态机。首先,我们将定义状态。状态都定义为常量字符串对象和使用@State注解:

```java
public class TapeDeckHandler {

    @State public static final String EMPTY   = "Empty";

    @State public static final String LOADED  = "Loaded";

    @State public static final String PLAYING = "Playing";

@State public static final String PAUSED  = "Paused";}
```

现在,当我们定义的状态可以设置对应于每个转换的代码。每个转换将在TapeDeckHandler对应方法。每个转换方法是使用@Transition注解定义触发转换的事件id(在)，一开始状态的转换(中)和最终状态的转换(下):

```java
public class TapeDeckHandler {

    @State public static final String EMPTY = "Empty";

    @State public static final String LOADED = "Loaded";

    @State public static final String PLAYING = "Playing";

    @State public static final String PAUSED = "Paused";

 

    @Transition(on = "load", in = EMPTY, next = LOADED)

    public void loadTape(String nameOfTape) {

        System.out.println("Tape '" + nameOfTape + "' loaded");

    }

 

    @Transitions({

        @Transition(on = "play", in = LOADED, next = PLAYING),

        @Transition(on = "play", in = PAUSED, next = PLAYING)

    })

    public void playTape() {

        System.out.println("Playing tape");

    }

 

    @Transition(on = "pause", in = PLAYING, next = PAUSED)

    public void pauseTape() {

        System.out.println("Tape paused");

    }

 

    @Transition(on = "stop", in = PLAYING, next = LOADED)

    public void stopTape() {

        System.out.println("Tape stopped");

    }

 

    @Transition(on = "eject", in = LOADED, next = EMPTY)

    public void ejectTape() {

        System.out.println("Tape ejected");

    }
}
```

请注意,TapeDeckHandler类没有实现TapeDeck接口。这是故意如此的。

现在,让我们看看这段代码。在loadTape函数上添加 @Transition注解：

```java
<span style="font-size:12px;">@Transition(on = "load", in = EMPTY, next = LOADED)public void loadTape(String nameOfTape) {</span>
```

指定当录音机处于空的状态和事件发生时的负载loadTape方法将被调用,然后录音机将继续加载状态。pauseTape
 @Transition注解,stopTape和ejectTape方法应该不需要任何进一步的解释。playTape上的注解方法看起来虽然略有不同。在上面的图中,可以看出当录音机在加载或暂停状态事件将磁带。有相同的方法要求多个转换@Transitions注解必须使用:

```java
@Transitions({

    @Transition(on = "play", in = LOADED, next = PLAYING),

    @Transition(on = "play", in = PAUSED, next = PLAYING)})public void playTape() {
```

@Transitions注解仅仅列出多个转换的带注解的方法将被调用。

**更多关于@Transition参数**
- 如果您省略的参数,它将默认为“*”将匹配任何事件。
- 如果您省略下一个参数,它将默认为“_self_”是当前状态的一个别名。创建一个循环过渡状态机中所有你要做的就是省略下一个参数。
- 重量参数可用于定义以什么顺序转换将搜索。转换为特定的状态将按升序进行排序根据他们的重量值。体重默认是0。

现在最后一个步骤是创建一个从注释StateMachine对象类,并使用它来创建一个代理对象实现TapeDeck:

```java
public static void main(String[] args) {

    TapeDeckHandler handler = new TapeDeckHandler();

    StateMachine sm = StateMachineFactory.getInstance(Transition.class).create(TapeDeckHandler.EMPTY, handler);

    TapeDeck deck = new StateMachineProxyBuilder().create(TapeDeck.class, sm);

 

    deck.load("The Knife - Silent Shout");

    deck.play();

    deck.pause();

    deck.play();

    deck.stop();

    deck.eject();
}
```

代码行：

```java
TapeDeckHandler handler = new TapeDeckHandler();StateMachine sm = StateMachineFactory.getInstance(Transition.class).create(TapeDeckHandler.EMPTY, handler);
```

从TapeDeckHandler的实例创建StateMachine实例。类的调用StateMachineFactory.getInstance(…)告诉工厂,我们使用@Transition注解来构建状态机。我们指定空作为开始的状态。一个StateMachine基本上是一个有向图。状态对象图中的对应节点而过渡对象对应于边缘。每个@Transition注解用于TapeDeckHandler实例对应一个过渡。

**@Transition和Transition的区别是什么?**

@Transition是你使用的注解标记方法之间的过渡状态发生时应该使用。在幕后mina-statemachine将创建MethodTransition类的实例为每个@Transition带注释的方法。MethodTransition实现转换接口。作为mina-statemachine用户你永远不会直接使用Transition或MethodTransition类型的转变。



通过调用StateMachineProxyBuilder，TapeDeck实例被创建:

```java
<span style="font-size:12px;">TapeDeck deck = new StateMachineProxyBuilder().create(TapeDeck.class, sm);</span>
```

StateMachineProxyBuilder.create()方法将返回的代理对象应该实现的接口和StateMachine实例将接收代理上的方法调用生成的事件。

当代码执行的输出应该是:

```
Tape 'The Knife - Silent Shout' loaded

Playing tape

Tape paused

Playing tape

Tape stopped

Tape ejected
```

**这一切都由Mina来做?**

正如您可能已经注意到Mina没什么具体的这类例子。但也不要惊慌。稍后我们将看到如何创建状态机对米娜的IoHandler接口。

### 1.1.2. **它怎样工作？**

让我们来看代理调用方法会发生什么。

#### 1.1.2.1. **查找StateContext对象**

StateContext对象很重要,因为它的当前状态。当一个方法被代理调用将要求StateContextLookup实例得到StateContext方法的参数。通常,StateContextLookup实现将循环通过方法参数和寻找一个特定类型的对象,并使用它来检索一个StateContext对象。如果没有指定StateContext
 StateContextLookup将创建一个并将其存储在对象。

当代理MinaIoHandler时，我们将使用IoSessoinStateContextLookup实例查找一个IoSession方法参数。它将使用IoSession的属性来存储每个MinaStateContext会话的一个单独的实例。这样相同的状态机可以用于所有Mina会话而互相没有干扰。

在上面的例子中，当我们使用StateMachineProxyBuilder创建代理时，我们从来没有指定使用什么StateContextLookup实现。如果未指定将使用SingletonStateContextLookup。SingletonStateContextLookup完全忽视了方法参数传递给它,它将总是返回相同的StateContext对象。这显然不会同时使用相同的状态机时非常有用,当我们代理IoHandle时很多客户会这样。

#### 1.1.2.2. **方法调用转为事件对象**

所有方法调用代理对象将被翻译成事件对象代理。一个事件有一个id和零个或多个参数。id对应的方法和事件参数的名称对应方法参数。方法调用deck.load("The
 Knife - Silent Shout")对应于事件{{id = "load", arguments = ["The Knife - Silent Shout"]}。事件对象还包含一个引用StateContext对象。

#### 1.1.2.3. **调用StateMachine**

一旦创建了事件对象代理将调用StateMachine.handle(Event)。StateMachine.handle(Event)遍历当前状态的Transition对象寻找转型实例接受当前的事件。这个过程将停止过渡后被发现。过渡对象将搜索的重量(通常@Transition指定的注解)。

#### 1.1.2.4. **执行Transition**

最后一步是调用Transition.execute(Event)的过渡匹配。过渡StateMachine执行后将更新当前状态与状态定义的转换。

Transition是一个接口。每次你使用@Transition注释MethodTransition对象将被创建。

### 1.1.3. **MethodTransition**

MethodTransition是非常重要的,需要进一步的解释。MethodTransition匹配事件如果事件的id匹配的参数@Transition注解和带注解的方法的参数赋值兼容事件的一个子集的参数。

所以,如果事件看起来像{ id =“foo”,arguments=(a,b,c)}的方法

```java
@Transition(on = "foo")public void someMethod(One one, Two two, Three three) { ... }
```

仅匹配if((a instanceof One && b instanceof Two && c instanceof Three) == true)。与匹配的匹配方法将调用事件参数绑定到方法的参数:

```java
someMethod(a, b, c);
```

Integer,Double、Float等也匹配基础数据类型的int,double,float等。

如上所述也将匹配一个子集:

```java
@Transition(on = "foo")public void someMethod(Two two) { ... }
```

匹配if ((a instanceof Two || b instanceof Two || c instanceof Two) == true)在这种情况下,第一个匹配的事件参数将被绑定到命名方法参数两个，当调用someMethod时。

如果事件id匹配，一个方法没有参数还是会匹配:

```java
@Transition(on = "foo")public void someMethod() { ... }
```

使事情更复杂的前两个方法参数匹配，也违反事件类和StateContext接口。这意味着：

```java
@Transition(on = "foo")
public void someMethod(Event event, StateContext context, One one, Two two, Three three) { ... }

@Transition(on = "foo")public void someMethod(Event event, One one, Two two, Three three) { ... }

@Transition(on = "foo")public void someMethod(StateContext context, One one, Two two, Three three) { ... }
```

还与事件{ id =“foo”,参数=[a,b,c]} if((a
 instanceof One& & b instanceof 2 & & c instanceof 3)= = true)。当前的事件对象将被绑定到事件方法参数和当前StateContext someMethod被调用时将被绑定到上下文。

像以前一样可以使用事件参数的一个子集。指定一个特定StateContext实现也可以代替使用通用接口:

```java
@Transition(on = "foo")public void someMethod(MyStateContext context, Two two) { ... }
```

方法参数的顺序是很重要的。如果方法需要访问当前事件必须首先指定为方法参数。StateContext是第二个参数如果第一个事件或第一个参数。事件参数也必须以正确的顺序匹配。MethodTransition不会试图重新排序事件参数的搜索匹配。

如果你做到这一步,恭喜你!我意识到上面的部分可能有点难以消化。希望一些例子可以让事情更清晰:

思考事件{id = "messageReceived", arguments = [ArrayList a = [...], Integer b = 1024]}.以下方法匹配这一事件:

```java
// All method arguments matches all event arguments directly
@Transition(on = "messageReceived")
public void messageReceived(ArrayList l, Integer i) { ... }

// Matches since ((a instanceof List && b instanceof Number) == true)
@Transition(on = "messageReceived")
public void messageReceived(List l, Number n) { ... }

// Matches since ((b instanceof Number) == true)
@Transition(on = "messageReceived")
public void messageReceived(Number n) { ... }

// Methods with no arguments always matches
@Transition(on = "messageReceived")
public void messageReceived() { ... }

// Methods only interested in the current Event or StateContext always matches
@Transition(on = "messageReceived")
public void messageReceived(StateContext context) { ... }

// Matches since ((a instanceof Collection) == true)
@Transition(on = "messageReceived")
public void messageReceived(Event event, Collection c) { ... }</span>
```

以下不匹配:

```java
<span style="font-size:12px;">// Incorrect ordering
@Transition(on = "messageReceived")
public void messageReceived(Integer i, List l) { ... }

// ((a instanceof LinkedList) == false)
@Transition(on = "messageReceived")
public void messageReceived(LinkedList l, Number n) { ... }

// Event must be first argument
@Transition(on = "messageReceived")
public void messageReceived(ArrayList l, Event event) { ... }

// StateContext must be second argument if Event is used
@Transition(on = "messageReceived")
public void messageReceived(Event event, ArrayList l, StateContext context) { ... }

// Event must come before StateContext
@Transition(on = "messageReceived")
public void messageReceived(StateContext context, Event event) { ... }
```

#### 1.1.3.1. **状态继承**

状态实例可能来自父母状态。如果StateMachine.handle(Event)找不到Transition匹配当前事件的当前状态，它将搜索父状态。如果没有找到匹配的父母的，其父母的父母将被搜索等等。

这个特性很有用,当你想要一些通用的代码添加到所有状态，而不必为每个状态指定@Transition注解。这是你如何创建一个层次结构的状态使用@State注解:

```java
@State    public static final String A = "A";

@State(A) public static final String B = "A->B";

@State(A) public static final String C = "A->C";

@State(B) public static final String D = "A->B->D";

@State(C) public static final String E = "A->C->E";
```

#### 1.1.3.2. **使用状态继承的错误处理**

让我们回到TapeDeck例子。如果你调用deck的deck.play()没有磁带会怎样?让我们试一试:

```java
public static void main(String[] args) {

    ...

    deck.load("The Knife - Silent Shout");

    deck.play();

    deck.pause();

    deck.play();

    deck.stop();

    deck.eject();

    deck.play();}

...Tape stoppedTape ejectedException in thread "main" o.a.m.sm.event.UnhandledEventException: Unhandled event: org.apache.mina.statemachine.event.Event@15eb0a9[id=play,...]

    at org.apache.mina.statemachine.StateMachine.handle(StateMachine.java:285)

    at org.apache.mina.statemachine.StateMachine.processEvents(StateMachine.java:142)

    ...
```

哦!我们得到一个UnhandledEventException因为当我们处于空的状态没有transition处理事件。我们可以添加一个特殊的transition到所有状态处理无与伦比的事件对象:

```java
@Transitions({

    @Transition(on = "*", in = EMPTY, weight = 100),

    @Transition(on = "*", in = LOADED, weight = 100),

    @Transition(on = "*", in = PLAYING, weight = 100),

    @Transition(on = "*", in = PAUSED, weight = 100)})public void error(Event event) {

    System.out.println("Cannot '" + event.getId() + "' at this time");
}</span>
```

现在,当您运行上面的main()方法中,你不会得到一个异常。输出应该是:

```java
...

Tape stopped

Tape ejected

Cannot 'play' at this time.
```

现在这似乎工作得很好,对吧?但是如果我们有30个状态,而不是只有4个呢 ?然后我们需要30个@Transition注解错误()方法。这样不好。让我们使用状态继承:

```java
public static class TapeDeckHandler {

    @State public static final String ROOT = "Root";

    @State(ROOT) public static final String EMPTY = "Empty";

    @State(ROOT) public static final String LOADED = "Loaded";

    @State(ROOT) public static final String PLAYING = "Playing";

    @State(ROOT) public static final String PAUSED = "Paused";

 

    ...

 

    @Transition(on = "*", in = ROOT)

    public void error(Event event) {

        System.out.println("Cannot '" + event.getId() + "' at this time");

    }
}
```

结果将是相同的但事情会更容易保持，因为这最后一种方法。

#### 1.1.3.3. **mina-statemachine与IoHandler**

现在我们要把我们的录音机磁带放到TCP服务器和扩展更多的功能。服务器将接收命令加载,播放,停止,等。要么是积极的响应+或者消极响应 -。协议是基于文本的,所有命令和响应的utf
 - 8行文本被CRLF终止(即。\ r \ n在Java)。这里有一个例子会话:

```java
telnet localhost 12345

S: + Greetings from your tape deck!

C: listS: + (1: "The Knife - Silent Shout", 2: "Kings of convenience - Riot on an empty street")

C: load 1S: + "The Knife - Silent Shout" loaded

C: play

S: + Playing "The Knife - Silent Shout"

C: pauseS: + "The Knife - Silent Shout" paused

C: playS: + Playing "The Knife - Silent Shout"

C: infoS: + Tape deck is playing. Current tape: "The Knife - Silent Shout"

C: ejectS: - Cannot eject while playingC: stop

S: + "The Knife - Silent Shout" stopped

C: ejectS: + "The Knife - Silent Shout" ejected

C: quitS: + Bye! Please come back!
```

完整的代码TapeDeckServer org.apache.mina.example可用本节中描述。tapedeck包mina-example模块的Subversion存储库。代码使用MinaProtocolCodecFilter将字节from/to命令对象。有一个命令实现为每种类型的请求服务器识别。我们不会描述编解码器实现的任何细节。

现在,让我们看一看这个服务器是如何工作的。重要的类,实现状态机是TapeDeckServer类。我们做的第一件事是定义状态:

```java
@State public static final String ROOT = "Root";

@State(ROOT) public static final String EMPTY = "Empty";

@State(ROOT) public static final String LOADED = "Loaded";

@State(ROOT) public static final String PLAYING = "Playing";

@State(ROOT) public static final String PAUSED = "Paused";
```

没有什么新鲜的。然而,处理事件的方法现在看起来不同了。让我们看一下playTape方法:

```java
@IoHandlerTransitions({

    @IoHandlerTransition(on = MESSAGE_RECEIVED, in = LOADED, next = PLAYING),

    @IoHandlerTransition(on = MESSAGE_RECEIVED, in = PAUSED, next = PLAYING)})
    public void playTape(TapeDeckContext context, IoSession session, PlayCommand cmd) {

    session.write("+ Playing \"" + context.tapeName + "\"");
}
```

这段代码不直接使用通用@Transition和@Transitions注解而是Mina特定@IoHandlerTransition和@IoHandlerTransitions注释。这是首选在创建状态机Mina的IoHandler接口允许您使用一个Java枚举的事件id而不是字符串作为我们之前使用。也有相应的注释Mina的IoFilter接口。

我们现在使用MESSAGE_RECEIVED而不是“play”的事件名称(在@IoHandlerTransition属性)。这个常数是在org.apache.mina.statemachine.event中定义的。IoHandlerEvents当然有价值“messageReceived”对应于Mina的IoHandler接口方法messageReceived（）。由于Java5的静态导入我们不必写出类的名称保持不变。我们只需要引入：

```java
import static org.apache.mina.statemachine.event.IoHandlerEvents.*;
```

声明在引入部分。

另一件事改变了我们使用自定义StateContext实现,TapeDeckContext。该类用于跟踪当前胶带的名称:

```java
static class TapeDeckContext extends AbstractStateContext {

    public String tapeName;
}
```

**为什么不将磁带名称存储在IoSession?**

我们可以存储磁带的名称作为IoSession但推荐使用自定义属性StateContext,因为它提供了类型安全。

最后一件事需要注意的是playTape()方法,它接受PlayCommand作为最后的参数。最后一个参数对应的消息参数的IoHandler
 messageReceived(IoSession session,Object message)方法。这意味着playTape()方法只会被称为如果客户端发送的字节可以作为PlayCommand解码。



录音机可以播放任何磁带之前需要加载磁带。当收到客户提供的LoadCommand磁带数量将被用来使胶带的名称从可用磁带的磁带阵列负载:

```java
@IoHandlerTransition(on = MESSAGE_RECEIVED, in = EMPTY, next = LOADED)
public void loadTape(TapeDeckContext context, IoSession session, LoadCommand cmd) {

    if (cmd.getTapeNumber() < 1 || cmd.getTapeNumber() > tapes.length) {

        session.write("- Unknown tape number: " + cmd.getTapeNumber());

        StateControl.breakAndGotoNext(EMPTY);

    } else {

        context.tapeName = tapes[cmd.getTapeNumber() - 1];

        session.write("+ \"" + context.tapeName + "\" loaded");

    }
}
```

这段代码使用StateControl类重载下一个状态。如果用户指定一个未知的磁带数量我们不应该移动到加载状态,而是留在空状态就可以了：

```java
StateControl.breakAndGotoNext(EMPTY);
```

StateControl类更在后面一节中描述。

connect()方法开始时总是被称为一个session会话，当Mina调用sessionOpened
 IoHandler()时:

```java
@IoHandlerTransition(on = SESSION_OPENED, in = EMPTY)
public void connect(IoSession session) {

    session.write("+ Greetings from your tape deck!");
}
```

它所做的是编写对客户机的问候。状态机仍将处于空的状态。

pauseTape(),stopTape()和ejectTape()方法非常类似于playTape()和不会被描述在任何细节。listTapes(),信息()和退出()方法应该简单了解了,。请注意这些最后的三种方法是如何用于根状态。这意味着列表,信息和退出命令可以发布在任何状态。

现在让我们看一看错误处理。在当前客户端发送一个不合法的命令状态的时候，error()方法将调用。

```java
@IoHandlerTransition(on = MESSAGE_RECEIVED, in = ROOT, weight = 10)
public void error(Event event, StateContext context, IoSession session, Command cmd) {

    session.write("- Cannot " + cmd.getName() + " while "

           + context.getCurrentState().getId().toLowerCase());
}
```

error()是一个比listTapes(), info() 和 quit() 更重量级的方法，以防止那些命令调用。注意error()使用StateContext对象的当前状态的id。字符串常量的值与@State注解(Empty,Load等)将由mina-statemachine状态id所使用。

当ProtocolDecoder调用抛出CommandSyntaxException异常，commandSyntaxError()方法将被调用。它只是输出,发送的客户端行无法转化成一个命令。

exceptionCaught()将会被任何异常捕获所调用，除了CommandSyntaxException异常（它有一个比commandSyntaxError()更重量级的方法）。它立即关闭session会话。

如果没有其他@IoHandlerTransition方法匹配的事件，最终@IoHandlerTransition的unhandledEvent()方法将被调用。我们需要这个,因为我们没有@IoHandlerTransition注释所有状态的所有可能类型的事件(如。我们从来没有处理messageSent事件)。没有这个mina-statemachine抛出一个异常事件决定是否由状态机处理。

最后一段代码我们要看一看的代码创建IoHandler代理和main()方法:

```java
private static IoHandler createIoHandler() {

    StateMachine sm = StateMachineFactory.getInstance(IoHandlerTransition.class).create(EMPTY, new TapeDeckServer());

 

    return new StateMachineProxyBuilder().setStateContextLookup(

            new IoSessionStateContextLookup(new StateContextFactory() {

                public StateContext create() {

                    return new TapeDeckContext();

                }

            })).create(IoHandler.class, sm);}

// This code will work with MINA 1.0/1.1:public static void main(String[] args) throws Exception {

    SocketAcceptor acceptor = new SocketAcceptor();

    SocketAcceptorConfig config = new SocketAcceptorConfig();

    config.setReuseAddress(true);

    ProtocolCodecFilter pcf = new ProtocolCodecFilter(

            new TextLineEncoder(), new CommandDecoder());

    config.getFilterChain().addLast("codec", pcf);

    acceptor.bind(new InetSocketAddress(12345), createIoHandler(), config);}

// This code will work with MINA trunk:public static void main(String[] args) throws Exception {

    SocketAcceptor acceptor = new NioSocketAcceptor();

    acceptor.setReuseAddress(true);

    ProtocolCodecFilter pcf = new ProtocolCodecFilter(

            new TextLineEncoder(), new CommandDecoder());

    acceptor.getFilterChain().addLast("codec", pcf);

    acceptor.setHandler(createIoHandler());

    acceptor.setLocalAddress(new InetSocketAddress(PORT));

    acceptor.bind();
}
```

createIoHandler()创建一个StateMachine就像我们之前做的除了我们指定IoHandlerTransition.class而不是Transition.class的调用StateMachineFactory.getInstance(…)。这是必要的,因为我们现在使用@IoHandlerTransition注解。同时,这一次我们使用IoSessionStateContextLookup和自定义StateContextFactory当我们创建IoHandler代理。如果我们不使用IoSessionStateContextLookup所有客户将共享相同的状态机并不是可取的。

main()方法创建SocketAcceptor和附加一个ProtocolCodecFilter解码/编码命令对象过滤器链。最后,它绑定到端口12345上使用一个IoHandler
 createIoHandler()方法创建实例。

### 1.1.4. **高级话题**

#### 1.1.4.1. **程序动态改变状态**

#### 1.1.4.2. **递归调用状态机**

 注：关于高级话题官方用户指南没有写更多，但是给出了研究方向。







