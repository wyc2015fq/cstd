# Java串口通信 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月24日 21:37:51[boonya](https://me.csdn.net/boonya)阅读数：943










原文来自：[http://blog.csdn.net/kabini/article/details/1601324](http://blog.csdn.net/kabini/article/details/1601324)





1串口通信简介

嵌入式系统或传感器网络的很多应用和测试都需要通过PC机与嵌入式设备或传感器节点进行通信。其中，最常用的接口就是RS-232串口和并口（鉴于USB接口的复杂性以及不需要很大的数据传输量，USB接口用在这里还是显得过于奢侈，况且目前除了SUN有一个支持USB的包之外，我还没有看到其他直接支持USB的Java类库）。SUN的CommAPI分别提供了对常用的RS232串行端口和IEEE1284并行端口通讯的支持。RS-232-C(又称EIA
 RS-232-C，以下简称RS232)是在1970年由美国电子工业协会(EIA)联合贝尔系统、调制解调器厂家及计算机终端生产厂家共同制定的用于串行通讯的标准。RS232是一个全双工的通讯协议，它可以同时进行数据接收和发送的工作。

**1.1 常见的Java串口包**

目前，常见的Java串口包有SUN在1998年发布的串口通信API：comm2.0.jar（Windows下）、comm3.0.jar（Linux/Solaris）；IBM的串口通信API以及一个开源的实现。鉴于在Windows下SUN的API比较常用以及IBM的实现和SUN的在API层面都是一样的，那个开源的实现又不像两家大厂的产品那样让人放心，这里就只介绍SUN的串口通信API在Windows平台下的使用。

**1.2 串口包的安装（Windows下）**

到SUN的网站下载javacomm20-win32.zip，包含的东西如下所示：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/kabini/5840ccf9083347aab0632a9b79bfde3f.png)

按照其使用说明（Readme.html）的说法，要想使用串口包进行串口通信，除了设置好环境变量之外，还要将win32com.dll复制到<JDK>/bin目录下；将comm.jar复制到<JDK>/lib；把javax.comm.properties也同样拷贝到<JDK>/lib目录下。然而在真正运行使用串口包的时候，仅作这些是不够的。因为通常当运行“java MyApp”的时候，是由JRE下的虚拟机启动MyApp的。而我们只复制上述文件到JDK相应目录下，所以应用程序将会提示找不到串口。解决这个问题的方法很简单，我们只须将上面提到的文件放到JRE相应的目录下就可以了。

值得注意的是，在网络应用程序中使用串口API的时候，还会遇到其他更复杂问题。有兴趣的话，你可以查看CSDN社区中“**关于网页上Applet用javacomm20读取客户端串口的问题**”的帖子。

2串口API概览

**2.1javax.comm.CommPort**

这是用于描述一个被底层系统支持的端口的抽象类。它包含一些高层的IO控制方法，这些方法对于所有不同的通讯端口来说是通用的。SerialPort 和ParallelPort都是它的子类，前者用于控制串行端口而后者用于控这并口，二者对于各自底层的物理端口都有不同的控制方法。这里我们只关心SerialPort。

**2.2javax.comm.CommPortIdentifier**

这个类主要用于对串口进行管理和设置，是对串口进行访问控制的核心类。主要包括以下方法

l确定是否有可用的通信端口

l为IO操作打开通信端口

l决定端口的所有权

l处理端口所有权的争用

l管理端口所有权变化引发的事件（Event）

**2.3javax.comm.SerialPort**

这个类用于描述一个RS-232串行通信端口的底层接口，它定义了串口通信所需的最小功能集。通过它，用户可以直接对串口进行读、写及设置工作。

**2.4 串口API实例**

大段的文字怎么也不如一个小例子来的清晰，下面我们就一起看一下串口包自带的例子---SerialDemo中的一小段代码来加深对串口API核心类的使用方法的认识。

**2.4.1列举出本机所有可用串口**

void listPortChoices() {

            CommPortIdentifier portId;

            Enumeration en = CommPortIdentifier.getPortIdentifiers();

            // iterate through the ports.

            while (en.hasMoreElements()) {

                portId = (CommPortIdentifier) en.nextElement();

                if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {

                    System.out.println(portId.getName());

                }

            }

            portChoice.select(parameters.getPortName());

        }

以上代码可以列举出当前系统所有可用的串口名称，我的机器上输出的结果是COM1和COM3。

**2.4.2串口参数的配置**

串口一般有如下参数可以在该串口打开以前配置进行配置：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/kabini/09f3fe07829f44059cf7334cb5f1bc60.png)

包括波特率，输入/输出流控制，数据位数，停止位和齐偶校验。

SerialPort sPort;

try {

            sPort.setSerialPortParams(BaudRate,Databits,Stopbits,Parity);

                     //设置输入/输出控制流

                     sPort.setFlowControlMode(FlowControlIn | FlowControlOut);

        } catch (UnsupportedCommOperationException e) {}

**2.4.3串口的读写**

对串口读写之前需要先打开一个串口：

CommPortIdentifier portId = CommPortIdentifier.getPortIdentifier(PortName);

try {

            SerialPort  sPort = (SerialPort) portId.open("串口所有者名称", 超时等待时间);

        } catch (PortInUseException e) {//如果端口被占用就抛出这个异常

            throw new SerialConnectionException(e.getMessage());

        }

//用于对串口写数据

OutputStream os = new BufferedOutputStream(sPort.getOutputStream());

os.write(int data);

//用于从串口读数据

InputStream is = new BufferedInputStream(sPort.getInputStream());

int receivedData = is.read();

读出来的是int型，你可以把它转换成需要的其他类型。

这里要注意的是，由于Java语言没有无符号类型，即所有的类型都是带符号的，在由byte到int的时候应该尤其注意。因为如果byte的最高位是1，则转成int类型时将用1来占位。这样，原本是10000000的byte类型的数变成int型就成了1111111110000000，这是很严重的问题，应该注意避免。

3串口通信的通用模式及其问题

终于唠叨完我最讨厌的基础知识了，下面开始我们本次的重点--串口应用的研究。由于向串口写数据很简单，所以这里我们只关注于从串口读数据的情况。通常，串口通信应用程序有两种模式，一种是实现SerialPortEventListener接口，监听各种串口事件并作相应处理；另一种就是建立一个独立的接收线程专门负责数据的接收。由于这两种方法在某些情况下存在很严重的问题（至于什么问题这里先卖个关子J），所以我的实现是采用第三种方法来解决这个问题。

**3.1 事件监听模型**

现在我们来看看事件监听模型是如何运作的

：

l首先需要在你的端口控制类（例如SManager）加上“implements SerialPortEventListener”


l在初始化时加入如下代码：

try {

            SerialPort sPort.addEventListener(SManager);

        } catch (TooManyListenersException e) {

            sPort.close();

            throw new SerialConnectionException("too many listeners added");

        }

        sPort.notifyOnDataAvailable(true);


l覆写public void serialEvent(SerialPortEvent e)方法，在其中对如下事件进行判断：

BI -通讯中断.

　　CD -载波检测.

　　CTS -清除发送.

　　DATA_AVAILABLE -有数据到达.

　　DSR -数据设备准备好.

　　FE -帧错误.

　　OE -溢位错误.

　　OUTPUT_BUFFER_EMPTY -输出缓冲区已清空.

　　PE -奇偶校验错.

RI -　振铃指示.

一般最常用的就是DATA_AVAILABLE--串口有数据到达事件。也就是说当串口有数据到达时，你可以在serialEvent中接收并处理所收到的数据。然而在我的实践中，遇到了一个十分严重的问题。

首先描述一下我的实验：我的应用程序需要接收传感器节点从串口发回的查询数据，并将结果以图标的形式显示出来。串口设定的波特率是115200，川口每隔128毫秒返回一组数据（大约是30字节左右），周期（即持续时间）为31秒。实测的时候在一个周期内应该返回4900多个字节，而用事件监听模型我最多只能收到不到1500字节，不知道这些字节都跑哪里去了，也不清楚到底丢失的是那部分数据。值得注意的是，这是我将serialEvent()中所有处理代码都注掉，只剩下打印代码所得的结果。数据丢失的如此严重是我所不能忍受的，于是我决定采用其他方法。

**3.2 串口读数据的线程模型**

这个模型顾名思义，就是将接收数据的操作写成一个线程的形式:

public void startReadingDataThread() {

        Thread readDataProcess = new Thread(new Runnable() {

            public void run() {

                            while (newData != -1) {

                    try {

                                          newData = is.read();

                        System.out.println(newData);

                                          //其他的处理过程

                                          ……….

                                   } catch (IOException ex) {

                        System.err.println(ex);

                        return;

                    }

                     }

              readDataProcess.start();

}

在我的应用程序中，我将收到的数据打包放到一个缓存中，然后启动另一个线程从缓存中获取并处理数据。两个线程以生产者—消费者模式协同工作，数据的流向如下图所示：



![](https://p-blog.csdn.net/images/p_blog_csdn_net/kabini/serial%20data%20flow.JPG)

这样，我就圆满解决了丢数据问题。然而，没高兴多久我就又发现了一个同样严重的问题：虽然这回不再丢数据了，可是原本一个周期（31秒）之后，传感器节电已经停止传送数据了，但我的串口线程依然在努力的执行读串口操作，在控制台也可以看见收到的数据仍在不断的打印。原来，由于传感器节点发送的数据过快，而我的接收线程处理不过来，所以InputStream就先把已到达却还没处理的字节缓存起来，于是就导致了明明传感器节点已经不再发数据了，而控制台却还能看见数据不断打印这一奇怪的现象。唯一值得庆幸的是最后收到数据确实是4900左右字节，没出现丢失现象。然而当处理完最后一个数据的时候已经快1分半钟了，这个时间远远大于节点运行周期。这一延迟对于一个实时的显示系统来说简直是灾难！

后来我想，是不是由于两个线程之间的同步和通信导致了数据接收缓慢呢？于是我在接收线程的代码中去掉了所有处理代码，仅保留打印收到数据的语句，结果依然如故。看来并不是线程间的通信阻碍了数据的接收速度，而是用线程模型导致了对于发送端数据发送速率过快的情况下的数据接收延迟。这里申明一点，就是对于数据发送速率不是如此快的情况下前面者两种模型应该还是好用的，只是特殊情况还是应该特殊处理。

**3.3 第三种方法**

痛苦了许久（Boss天天催我L）之后，偶然的机会，我听说TinyOS中（又是开源的）有一部分是和我的应用程序类似的串口通信部分，于是我下载了它的1.x版的Java代码部分，参考了它的处理方法。解决问题的方法说穿了其实很简单，就是从根源入手。根源不就是接收线程导致的吗，那好，我就干脆取消接收线程和作为中介的共享缓存，而直接在处理线程中调用串口读数据的方法来解决问题（什么，为什么不把处理线程也一并取消？----都取消应用程序界面不就锁死了吗？所以必须保留）于是程序变成了这样：

public byte[] getPack(){

       while (true) {

                       // PacketLength为数据包长度

                    byte[] msgPack = new byte[PacketLength];

                    for(int i = 0; i < PacketLength; i++){

                        if( (newData = is.read()) != -1){

                            msgPack[i] = (byte) newData;

                            System.out.println(msgPack[i]);

                        }

                    }

                    return msgPack;

                            }

}

在处理线程中调用这个方法返回所需要的数据序列并处理之，这样不但没有丢失数据的现象行出现，也没有数据接收延迟了。这里唯一需要注意的就是当串口停止发送数据或没有数据的时候is.read()一直都返回-1，如果一旦在开始接收数据的时候发现-1就不要理它，继续接收，直到收到真正的数据为止。



4结束语
本文介绍了串口通信的基本知识，以及常用的几种模式。通过实践，提出了一些问题，并在最后加以解决。值得注意的是对于第一种方法，我曾将传感器发送的时间由128毫秒增加到512毫秒，仍然有很严重的数据丢失现象发生，所以如果你的应用程序需要很精密的结果，传输数据的速率又很快的话，就最好不要用第一种方法。对于第二种方法，由于是线程导致的问题，所以对于不同的机器应该会有不同的表现，对于那些处理多线程比较好的机器来说，应该会好一些。但是我的机器是Inter 奔四3.0双核CPU+512DDR内存，这样都延迟这么厉害，还得多强的CPU才行啊？所以对于数据量比较大的传输来说，还是用第三种方法吧。不过这个世界问题是很多的，而且未知的问题比已知的问题多的多，说不定还有什么其他问题存在，欢迎你通过下面的联系方式和我一起研究。

[](http://blog.csdn.net/kabini/article/details/1601324#)[](http://blog.csdn.net/kabini/article/details/1601324#)[](http://blog.csdn.net/kabini/article/details/1601324#)[](http://blog.csdn.net/kabini/article/details/1601324#)[](http://blog.csdn.net/kabini/article/details/1601324#)

[](http://blog.csdn.net/kabini/article/details/1601324#)



