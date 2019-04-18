# PC-PC-单片机(Arduino)通信实例 - weixin_33985507的博客 - CSDN博客
2013年12月17日 14:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
请仔细理解相关参数，如端口设置、IP设置、COM口设置......等等.....不要盲目COPY.....这涉及2台电脑和一个单片机，其中一台电脑作为服务器并与单片机相连，负责通过网络与客户端通信(socket)和通过数据线与单片机通过COM口通信(我实验时用的是COM9,其他电脑可能不同需要调整：具体在设备管理器中查看)。
1、Arduino板待烧入代码：(这里硬件的代码没啥好说的，只要你会写个简单Arduino代码并且自己烧过就知道该怎么做啦)
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
//////////////////////////////////////////////////////////
//Arduino 1.0.x-----Arduino Uno----COM9
//使用3-10号引脚连接8个LED
//////////////////////////////////////////////////////////
int incomingByte = 0;   //输入的数据存储变量
int count=1;
void setup() {
    Serial.begin(9600);// opens serial port, sets data rate to 9600 bps
    for(int i=3;i<=10;i++)//打开3-10号引脚
        pinMode(i,OUTPUT);
}
void loop() {
    while(count){
        Serial.println("please input 0 1 2 3 4 5 6 7 8 9 a");
        count=0;
    }//第一次操作说明，count为了第一次限制
    if (Serial.available() > 0){
        incomingByte = Serial.read();//读取
        Serial.println(incomingByte, DEC);//写入
    }//从COM口读取数据
    
    switch (incomingByte)//数据处理并对引脚进行调控
    {
    case 48://输入0时将3-10号引脚设为高电位，使所有的灯灭
        for(int i=3;i<=10;i++){
            digitalWrite(i,HIGH);
        }break;
    case 49://输入1从3-10号引脚电位依次设低电位维持0.5s，然后设为高电位，实现流水灯效果
        for(int i=3;i<=10;i++){
            digitalWrite(i,LOW);
            delay(500);
            digitalWrite(i,HIGH);
        }break;
    case 50://输入2流水灯，和输入1时方向相反
        for(int i=10;i>=3;i--){
            digitalWrite(i,LOW);
            delay(500);
            digitalWrite(i,HIGH);
        }break;    
    case 51: digitalWrite(3,LOW);break;//输入3，将3号电位调低，只让3号引脚的灯亮
    case 52: digitalWrite(4,LOW);break; //.....
    case 53: digitalWrite(5,LOW);break; //.....
    case 54: digitalWrite(6,LOW);break; //.....
    case 55: digitalWrite(7,LOW);break; //.....
    case 56: digitalWrite(8,LOW);break; //.....
    case 57: digitalWrite(9,LOW);break; //.....
    case 97: digitalWrite(10,LOW);break;//.....
    default: 
        for(int i=3;i<=10;i++){
            digitalWrite(i,HIGH);
        }break;//将所电位调高关闭所有灯
    }
}
View Code
```
2、服务器和客户端代码：
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package serial;
   
public class SerialBuffer{
    private String Content = "";
    private String CurrentMsg, TempContent;
    private boolean available = false;
    private int LengthNeeded = 1;
       
    public synchronized String GetMsg(int Length){
        LengthNeeded = Length;
        notifyAll();
        if (LengthNeeded > Content.length()){
            available = false;
            while (available == false){
                try{
                    wait();
                } catch (InterruptedException e) {}
            }
        }
        CurrentMsg = Content.substring(0, LengthNeeded);
        TempContent = Content.substring(LengthNeeded);
        Content = TempContent;
        LengthNeeded = 1;
        notifyAll();
        return CurrentMsg;
    }//本函数从串口(缓冲区)中读取指定长度的一个字符串。
     //参数Length指定所返回字符串的长度。 
       
    public synchronized void PutChar(int c){
        Character d = new Character((char) c);
        Content = Content.concat(d.toString());
        if (LengthNeeded < Content.length()){
            available = true;
        }
        notifyAll();
    }
}
//SerialBuffer.java (用来保存从串口所接收数据的缓冲区)
//在往缓冲区写入数据或者是从缓冲区读取数据的时候，必须保证数据的同步，
//因此GetMsg和PutChar函数均被声明为synchronized并在具体实现中采取措施实现的数据的同步。
A\SerialBuffer
```
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package serial;
import java.io.*;
    
public class ReadSerial extends Thread
{
    private SerialBuffer ComBuffer;
    private InputStream ComPort;
        
    public ReadSerial(SerialBuffer SB, InputStream Port){
        ComBuffer = SB;
        ComPort = Port;
    }//本函数构造一个ReadSerial进程，
     //参数SB指定存放传入数据的缓冲区，
     //参数Port指定从串口所接收的数据流
    public void run(){
        int c;
        try{
            while (true){
                c = ComPort.read();
                ComBuffer.PutChar(c);
            }
        } catch (IOException e) {}
    }
}
//ReadSerial.java (从串口读取数据的程序)
//ReadSerial是一个进程，它不断的从指定的串口读取
//数据并将其存放到缓冲区中
B\ReadSerial
```
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
package serial;
import java.io.*;
import java.util.*;
import gnu.io.*;
public class SerialBean
{
    static String PortName;
    CommPortIdentifier portId;
    SerialPort serialPort;
    static OutputStream out;
    static InputStream in;
    SerialBuffer SB;
    ReadSerial   RT;
       
    public SerialBean(int PortID){
        PortName = "COM" + PortID;
    }// 本函数构造一个指向特定串口的SerialBean
    //该串口由参数PortID所指定。PortID = 1 表示COM1
       
    public int Initialize(){
        int InitSuccess = 1;
        int InitFail    = -1;
          try{
              portId = CommPortIdentifier.getPortIdentifier(PortName);
              try{
                  serialPort = (SerialPort)portId.open("Serial_Communication", 2000);
              } catch (PortInUseException e){
                  return InitFail;
              }//打开一个串口
              
              try{
                  in = serialPort.getInputStream();
                  out = serialPort.getOutputStream();
              } catch (IOException e){
                  return InitFail;
              }//读写流实例化
             
              try{
                   serialPort.setSerialPortParams(9600,
                              SerialPort.DATABITS_8,
                              SerialPort.STOPBITS_1,
                              SerialPort.PARITY_NONE);
              } catch (UnsupportedCommOperationException e){
                  return InitFail;
              }//设置串行端口通讯参数
              
          } catch (NoSuchPortException e){
              return InitFail;
          }
        
          SB = new SerialBuffer();
          RT = new ReadSerial(SB, in);
          RT.start();
          // return success information
          return InitSuccess;
    }//本函数初始化所指定的串口并返回初始化结果。如果初始化成功返回1，否则返回-1。
    //初始化的结果是该串口被SerialBean独占性使用，其参数被设置为9600, N, 8, 1。
    //如果串口被成功初始化，则打开一个进程读取从串口传入的数据并将其保存在缓冲区中。
   
    public String ReadPort(int Length){
        String Msg;
        Msg = SB.GetMsg(Length);
        if(Msg==null)Msg="ssdfsdf";
        return Msg;
    }//本函数从串口(缓冲区)中读取指定长度的一个字符串。参数Length指定所返回字符串的长度
   
    public void WritePort(String Msg){
        int c;
        try{
            for (int i = 0; i < Msg.length(); i++)
                out.write(Msg.charAt(i));
        } catch (IOException e) {}
    }//本函数向串口发送一个字符串。参数Msg是需要发送的字符串
   
    public void ClosePort(){
        RT.stop();
        serialPort.close();
    }//本函数停止串口检测进程并关闭串口
}
//SerialBean是本类库与其他应用程序的接口。
//该类库中定义了SerialBean的构造方法以及初始化串口
//从串口读取数据，往串口写入数据以及关闭串口的函数
C\ SerialBean
```
分别把这3个文件放在D盘中然后分别编译：如下(一定要注意-d后面有一个空格+一个点+一个空格).
![](https://images0.cnblogs.com/blog/506370/201312/17141657-c10c6638a4354da5b8d5414ee65db059.png)
三个java文件编译好之后，你将在D盘看到一个serial文件夹，里面有相应的3个class文件。然后把原来的java文件删除或者移到别的地方(非常重要，不然会在编译服务器和客户端时报错)。然后把服务器代码放到刚才的D盘编译 && 客户端代码放到另一台电脑编译(如下图)
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
import java.util.Scanner;
import java.io.*;
import java.net.*;
import serial.*;
import serial.ReadSerial;
public class EchoServer{
    private int port = 8000;
    private ServerSocket serverSocket;
    
    public EchoServer()throws IOException{
        serverSocket = new ServerSocket(port);
        System.out.println("服务器启动");
    }//构造函数
    
    private PrintWriter getWriter(Socket socket)throws IOException{
        OutputStream socketOut=socket.getOutputStream();
        return new PrintWriter(socketOut,true);
    }//write函数
    private BufferedReader getReader(Socket socket)throws IOException{
        InputStream socketIn=socket.getInputStream();
        return new BufferedReader(new InputStreamReader(socketIn));
    }//read函数
    public void service(){
        Scanner scanner=new Scanner(System.in);
    SerialBean SB = new SerialBean(9);
    SB.Initialize();
    
        while(true){
            Socket socket=null;
            try{
                socket=serverSocket.accept();
                System.out.println("New connection accepted"+
                   socket.getInetAddress()+":"+socket.getPort());
                BufferedReader br=getReader(socket);
                PrintWriter pw=getWriter(socket);
                
                String clientMsg=null,portMsg=null;
                while((clientMsg=br.readLine())!=null){
                    System.out.println(clientMsg);
                    pw.println("~!:"+clientMsg);
                    if(clientMsg.equals("bye"))break;
                 
              portMsg = SB.ReadPort(9);
              System.out.println(portMsg);
              SB.WritePort(clientMsg);
                }
                SB.ClosePort();
                
            }catch(IOException e){
                e.printStackTrace();
            }finally{
                try{
                    if(socket!=null)socket.close();
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }
    }//服务函数
    public static void main(String args[])throws IOException{
        new EchoServer().service();
    }
}
                
服务器代码    (这里我用的是port=8000,COM9)如果硬件的com口不一样要做相应调整
```
```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
//网络通信客户端实现
import java.net.*;
import java.io.*;
import java.util.*;
public class EchoClient{
    private String host="localhost";
    private int port=8000;
    private Socket socket;
    public EchoClient()throws IOException{
        InetAddress addr=InetAddress.getByName("192.168.16.101");socket =new Socket(addr,port);//采用网络
        //socket = new Socket(host,port);//本地通信，就是服务器和客户端在同一个电脑（一台电脑测试时用）
    }//构造函数
    public static void main(String args[])throws IOException{
        new EchoClient().talk();
    }//main函数
    private PrintWriter getWriter(Socket socket)throws IOException{
        OutputStream socketOut=socket.getOutputStream();
        return new PrintWriter(socketOut,true);
    }//重写write函数
    private BufferedReader getReader(Socket socket)throws IOException{
        InputStream socketIn=socket.getInputStream();
        return new BufferedReader(new InputStreamReader(socketIn));
    }//重写read函数
  public void talk()throws IOException{
        try{
            BufferedReader br=getReader(socket);
            PrintWriter    pw=getWriter(socket);
            BufferedReader localReader=new BufferedReader(
                new InputStreamReader(System.in));
            String msg=null;
            while((msg=localReader.readLine())!=null){
                
                pw.println(msg);
                System.out.println(br.readLine());
                if(msg.equals("bye"))break;
            }
        }catch(IOException e){
            e.printStackTrace();
        }finally{
            try{
                socket.close();
            }catch(IOException e){
                e.printStackTrace();
            }
        }
    }//对话函数
}
                
        

        
客户端代码    (这里我用的网络IP为192.168.16.101，port和服务器一制为8000，如果你在做测试时对应网络IP要改为主机IP(网络-详细信息-IPv4)
```
![](https://images0.cnblogs.com/blog/506370/201312/17142713-ad5363970414435c9de721e52355bc1d.png)
![](https://images0.cnblogs.com/blog/506370/201312/17142807-b85574e833614dc083c1af57e5448170.png)
 接着分别启动主机和服务器就可以实现PC通过网络远程控制单片机啦！
![](https://images0.cnblogs.com/blog/506370/201312/17144048-ec688d070f484ac08424c0318ef89650.png)
![](https://images0.cnblogs.com/blog/506370/201312/17144119-1a13631f0146448bb0e804af2cbe30f3.png)
