# ROS 下实现相机图像采集与图像传输到服务器,socket图传 - 站在巨人的肩膀上coding - CSDN博客





2018年05月06日 23:26:51[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：1236








# 前言



        本文介绍一种Qt下进行ROS开发的完美方案，同时给出一个使用TCPROS进行图像传输的一个例子，使用的是ros-industrial的Levi-Armstrong在2015年12月开发的一个Qt插件ros_qtc_plugin，这个插件使得Qt“新建项目”和“新建文件”选项中出现ROS的相关选项，让我们可以直接在Qt下创建、编译、调试ROS项目，也可以直接在Qt项目中添加ROS的package、urdf、launch，感谢Levi-Armstrong。目前这个插件还在不断完善，有问题或者其他功能建议可以在ros_qtc_plugin的[项目主页的讨论区](https://github.com/ros-industrial/ros_qtc_plugin/issues)提出。

本文是用的操作系统是ubuntu kylin 14.04中文版，ROS版本是indigo，Qt版本是Qt5.5.1(Qt Creator 4.0.3) 

本文地址：[http://blog.csdn.net/u013453604/article/details/52186375](http://blog.csdn.net/u013453604/article/details/52186375)
视频教程：[ros_qtc_plugin插件作者Levi-Armstrong录制的插件使用教程](http://download.csdn.net/detail/u013453604/9601165)
参考： 
[ROS wiki IDEs](http://wiki.ros.org/IDEs#Qt_Creator_Plugin_for_ROS)
[1. Setup ROS Qt Creator Plug in](https://github.com/ros-industrial/ros_qtc_plugin/wiki/1.-Setup-ROS-Qt-Creator-Plug-in)
[2. Setup Qt Creator for ROS](https://github.com/ros-industrial/ros_qtc_plugin/wiki/2.-Setup-Qt-Creator-for-ROS)
[3. Debugging Catkin Workspace](https://github.com/ros-industrial/ros_qtc_plugin/wiki/3.-Debugging-Catkin-Workspace)
[4. Where to find Qt Creator Plug in Support](https://github.com/ros-industrial/ros_qtc_plugin/wiki/4.-Where-to-find-Qt-Creator-Plug-in-Support)
[github ros-industrial/ros_qtc_plugin项目主页](https://github.com/ros-industrial/ros_qtc_plugin)
[插件使用问题](https://github.com/ros-industrial/ros_qtc_plugin/issues?q=is%3Aissue+is%3Aclosed)

# 第一部分、入门

## 一、开发环境准备

//安装开发插件ros_qtc_plugin

//安装方法见历史博客【[传送门](https://blog.csdn.net/LOVE1055259415/article/details/80219749)】


## 二、新建ROS功能包与节点及编译测试
//查看我的历史博客【[传送门](https://blog.csdn.net/love1055259415/article/details/80065581)】



## 三、demo练手

### 3.1图像发布节点

#### 3.1.1具体代码及解析

```cpp
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "img_pub");
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::Publisher pub = it.advertise("camera/image",1);

  cv::Mat image = cv::imread(argv[1],CV_LOAD_IMAGE_COLOR);
  sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(),"bgr8",image).toImageMsg();

  ros::Rate loop_rate(5);
  while(nh.ok())
  {
    pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }

  ROS_INFO("Hello world!");
}
```


#### 3.1.2修改cmakeList.txt文件

    添加以下代码，然后回到工作空间，执行catkin_make
- find_package

```
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
  rospy
  std_msgs
  image_transport
)
#find_package(OpenCV)   #这里我注销了好像也没有报错
```
- 添加头文件包含

```
include_directories(
# include
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)
```

- 生成可执行文件

```
add_executable(img_pub src/img_pub.cpp)
```
- 连接到库文件

```
target_link_libraries(img_pub
    ${catkin_LIBRARIES}
  #  ${OpenCV_LIBRARIES}  #这里我注销了好像也没有报错
)
```



#### 3.1.3修改package.xml文件

package.xml文件中存放的是创建功能包时候节点所依赖的其他功能包，如果创建时候没有添加，则必须在这里手动添加。同时，在cmakeList.txt文件中添加以下内容。关于package.xml文件的具体讲解戳这里：https://www.cnblogs.com/qixianyu/p/6669687.html。


```
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  roscpp
  rospy
  std_msgs
  image_transport
)
```


//博主把这两句删了catkin_make也还是能够通过，先不加，如果报错再改


```
<build_depend>opencv2</build_depend>
  <build_depend>image_transport</build_depend>

  <exec_depend>image_transport</exec_depend>
  <exec_depend>opencv2</exec_depend>
```

#### 3.1.4编译

点击qtcreator左下角的小锤子，开始编译，如果有错误就会在下方显示，双击跳转到出错位置

![](https://img-blog.csdn.net/20180506201053381)

发现是需要在cmakeList.txt文件中添加

```
find_package(catkin REQUIRED COMPONENTS
  image_transport
)
```
注意，在构建自己的功能包的时候如果依赖了其他功能包，必须在find_package中添加依赖的包名。添加这句话后重新编译。



### 3.2图像订阅节点

### 3.2.1具体代码及分析

```
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "img_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/test/image",1,imageCallback);
  ros::spin();
  cv::destroyWindow("view");

  return 0;
}
```

#### 3.2.2修改cmakeList.txt和package.xml文件

这个文件中没有使用其他依赖功能包，所以package.xml文件暂时不需要做其他修改

cmakeList.txt文件中需要做一些修改：添加
- 链接库文件


```
target_link_libraries(img_listener
    ${catkin_LIBRARIES}
  # ${OpenCV_LIBRARIES}
)
```
- 生成可执行文件
`add_executable(img_listener src/img_listener.cpp`
#### 3.2.3代码编译

查阅资料发现，可以在~/.bashrc中添加如下代码，创建快捷方式cw,cs,cm。分别执行' command'中的命令


```
#ROS alais command
alias cw='cd ~/Travel_Assistance_Robot'
alias cs='cd ~/Travel_Assistance_Robot/src'
alias cm='cd ~/Travel_Assistance_Robot && catkin_make'
```

### 3.3执行

```
rosrun image_trans img_pub cal.png
```

然后可以查看话题，发现有我们自己发布的/test/image的话题


![](https://img-blog.csdn.net/20180506202304677)


`rosrun image_trans img_listener`
显示图像

![](https://img-blog.csdn.net/20180506203345623)


### 3.4 话题关系查看与节点关闭

#### 3.4.1节点交互

为了查看话题之间的关系，我们可以使用rgt_graph指令查看




```bash
rosrun rqt_graph rqt_graph
```

![](https://img-blog.csdn.net/20180506210120237)

#### 3.4.2节点关闭

想要优雅的关掉节点，请使用`rosnode kill`命令。当然你使用`ctrl+c`或者直接关闭终端也可以关闭节点，但这种方法不免过于简单粗暴，而且并未完全的关闭节点，因为节点管理器那里仍然有记录


![](https://img-blog.csdn.net/20180506210335815)




# 参考资料

[1][http://wiki.ros.org/image_transport/Tutorials](http://wiki.ros.org/image_transport/Tutorials)


[2][http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29](http://wiki.ros.org/ROS/Tutorials/WritingPublisherSubscriber%28c%2B%2B%29)

[3]https://blog.csdn.net/github_30605157/article/details/50990493

[4]https://blog.csdn.net/xiaocainiaoshangxiao/article/details/13295625


*****************************************我是萌萌哒的分割线***************************************************




# 第二部分、实战

## 一、图像采集节点

//2.1、启动相机launch文件

//2.2、相机启动launch文件阅读，代码阅读，调试

//节点间功能的切换

```
roslaunch ueye_cam rgb8.launch
```

打开相机后，显示如下

![](https://img-blog.csdn.net/20180506205627732)


## 二、图像显示节点

   这个就是上面的话题监听节点，定时从指定话题获取图像

![](https://img-blog.csdn.net/20180506205812279)




## 三、图像传输节点

//节点上的图像传输到服务器上

        我们需要基于校园网来传输图像，两个不同的设备连接在校园网上。问题是校园网上的节点有很多，当机器人移动到不同的位置的时候，所处的节点变化，IP地址也发生变化，此时如何和服务器进行通信？校园网采用了动态IP地址分配方式DHCP。一旦连接上校园网，只要不断开IP地址不应该不变吗？

        TCP/IP的话，客户端需要指定服务器的IP地址，服务器端需要知道客户端的IP地址。然后建立三次握手连接和关闭连接（四次握手）。

[校园网](https://www.baidu.com/s?wd=%E6%A0%A1%E5%9B%AD%E7%BD%91&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y1PyPhnWm3m1mvuH-9mvDs0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnW0YrHbvn1b4)的的IP地址一般都是和[MAC地址绑定](https://www.baidu.com/s?wd=MAC%E5%9C%B0%E5%9D%80%E7%BB%91%E5%AE%9A&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y1PyPhnWm3m1mvuH-9mvDs0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnW0YrHbvn1b4)的，就好比路由器中的静态地址保留功能，但是也有例外，有的是动态的分配，每次登陆，都会随机的分配。这个要看学校的实际情况，如果学校是动态分配的，那改成静态的IP地址是没法上网的，也有可能造成IP地址冲突。

需要解决的问题：如何获取动态分配DHCP的地址？目前发现有这种方式：

- 安装arp-scan工具[[在github上下载最新版本](https://github.com/royhills/arp-scan)，否则有些获取不到主机名]，使用rp-scan工具扫出局域网所有的IP地址

arp协议是一个数据链路层协议，负责IP地址和Mac地址的转换。


```python
sudo apt-get arp-scan
```

通过硬件地址筛选


```python
sudo arp-scan --interface=wlan0 --localnet | grep 54:35:30:19:68:8f
```

en0是网卡的设备名称，可以通过ifconfig命令获得，有多种网卡时注意不要写错


当我的IP为 inet addr:10.88.60.14时候，通过该方式只能够查到10.88网络上的主机列表，而看不到其他网络上的主机列表。比如看不到10.95网络上的主机列表。10.95.6.210。
- 在学校申请固定IP地址【赵海武】，那电脑每次开机IP地址会发生改变吗？

## 3.1、客户端图像发送程序

### 3.1.1、TX2上图像传输节点，从指定话题上获取消息，传输图像

https://blog.csdn.net/hanshuning/article/details/50581725

使用装有ROS插件的qtcrreator来调试。这里需要先设置一下：

点击左边的带三角的run,在右边add attach to node，选择需要调试的节点；同时，add run step，运行需要运行的节点


![](https://img-blog.csdn.net/20180507182841388)


然后就可以happy的调试了。

F5执行

F9设置断点


F10跳过，执行下一步指令

F11进入，具体实现调试

注意：这里有一个小坑，就是工程代码里面切记不要有中文，否则调试时候会进入汇编代码中，如下图所示：

![](https://img-blog.csdn.net/20180508190318755)


设置断点后调试，直接进入了汇编代码界面

![](https://img-blog.csdn.net/20180508190451914)
因为我的代码有中文路径：

![](https://img-blog.csdn.net/20180508190538106)

改成全英文路径，重新调试即可：

![](https://img-blog.csdn.net/20180508190631900)


### 3.1.2、读取本地图片，并发送

### （client）

```
/*socket image transfer
* 2018.5.10
* wzw
* qt head**************************/
#include <QCoreApplication>
#include <QDebug>

//socket headfile
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for qDebug
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <time.h>                //for time_t and time
#include <arpa/inet.h>
#include <unistd.h>    //close(client_socket);

//opencv headfile
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using  namespace cv;
using  namespace std;

#define HELLO_WORLD_SERVER_PORT   7754
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug("test ok");
    //get server ip addr
    if (argc != 2)
    {
        qDebug("Usage: %s ServerIPAddress\n",argv[0]);
        exit(1);
    }
    qDebug("ServerIPAddress %s\n",argv[1]);

    /**************
     * socket struct
     * ***********/
    //设置一个socket地址结构client_addr,代表客户机internet地址, 端口
    struct sockaddr_in client_addr;
    bzero(&client_addr,sizeof(client_addr)); //把一段内存区的内容全部设置为0
   // memset(&client_addr,sizeof(client_addr));
    client_addr.sin_family = AF_INET;    //internet协议族
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);//INADDR_ANY表示自动获取本机地址
    client_addr.sin_port = htons(0);    //0表示让系统自动分配一个空闲端口

    /*****
     * socket descriptor
     * 创建用于internet的流协议(TCP)socket,用client_socket代表客户机socket
     * ****/
    int client_socket = socket(AF_INET,SOCK_STREAM,0);
    if( client_socket < 0)
        {
            qDebug("Create Socket Failed!\n");
            exit(1);
        }

    /******
     * bind port
     * 把客户机的socket和客户机的socket地址结构联系起来,zhiding client_socket ip,point to addr
     * *******/
    if( bind(client_socket,(struct sockaddr*)&client_addr,sizeof(client_addr)))
        {
            qDebug("Client Bind Port Failed!\n");
            exit(1);
        }

    /*****
     * get server addr from argv[1],
     * set server params
     * 设置一个socket地址结构server_addr,代表服务器的internet地址, 端口
     * server_addr.sin_addr.s_addr=inet_addr(argv[1]);//same as up inet_aton
     * ****/
    //
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if(inet_aton(argv[1],&server_addr.sin_addr) == 0)
        {
            qDebug("Server IP Address Error!\n");
            exit(1);
        }
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);
    /***
     * connect request,to server ip.success return 0;fail is 1
     * 向服务器发起连接,连接成功后client_socket代表了客户机和服务器的一个socket连接
     * **/
     if(connect(client_socket,(struct sockaddr*)&server_addr, server_addr_length) < 0)
        {
            qDebug("Can Not Connect To %s!\n",argv[1]);
            exit(1);
        }
    qDebug("success connect To %s!\n",argv[1]);

    /*********************  data transfer test  ****************************/
    /*****
     * data prepare,set buffer
     * bzero == memset
     * ****/
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);
    //从服务器接收数据到buffer中
    int length = recv(client_socket,buffer,BUFFER_SIZE,0);
    if(length < 0)
        {
            qDebug("Recieve Data From Server %s Failed!\n", argv[1]);
            exit(1);
        }
    qDebug("\n%s\n",buffer);
    bzero(buffer,BUFFER_SIZE);
    // 向服务器发buffer中的数据
    bzero(buffer,BUFFER_SIZE);
    strcpy(buffer,"Hello, World! From Client\n");
    int send_flag=send(client_socket,buffer,BUFFER_SIZE,0);
    if(!send_flag)
        qDebug("send error\n");
    qDebug("send success\n");

    /********************* 向服务器发送image  ****************************/
    //1.load image,get imagesize
    Mat s_img=imread("1.jpg");
    imshow("s_img",s_img);
    vector<uchar> encode_img;
    imencode(".jpg", s_img, encode_img);//
    /****
     * test
     * ************/
    // align value
    /*
    Mat test_img(650,552,CV_8UC3);
    uchar* pxvec=test_img.ptr<uchar>(0);
    int i,j;
    for(i=0;i<s_img.rows;i++)
    {
        pxvec = test_img.ptr<uchar>(i);
        // 3 channels range,BGR
        for(j=0;j<s_img.cols*s_img.channels();j++)
        {
            pxvec[j]=244;
        }
    }

    imshow("test_img",test_img);
    imshow("s_img_encode",s_img);
*/
    //s_img-->vector
    /*
    int i,j;
    uchar* pxvec = s_img.ptr<uchar>(0);
    for(i=0;i<s_img.rows;i++)
    {
        pxvec = s_img.ptr<uchar>(i);
        // 3 channels range,BGR
        for(j=0;j<s_img.cols*s_img.channels();j++)
        {
            //qDebug("px value is %d",pxvec[j]) ;
            encode_img.push_back(pxvec[j]);
        }
    }
    */
    //get send_buffer
    int encode_img_size=encode_img.size();
    int s_img_size=s_img.rows*s_img.cols*3;
    qDebug("filesize is %d,width*hight*3 is %d\n",encode_img_size,s_img_size);

    uchar* send_buffer=new uchar[encode_img.size()];
    copy(encode_img.begin(),encode_img.end(),send_buffer);

    //2.send file_name
    int toSend=encode_img_size, receive  = 0, finished = 0;
    QString photoName;
    char* file_name;
    char char_len[10];
    photoName=QString("1.jpg");
    file_name=photoName.toLatin1().data();
    // file_name,qDebug file_name be empty
    //qDebug("file name is %s\n",file_name);
    bzero(buffer,BUFFER_SIZE);
    send_flag=send(client_socket, file_name, 10, 0);
    if(!send_flag)
      {
        qDebug(" send file_name failed\n ");
        exit(1);
      }
    qDebug("success send file_name \n");
    //3.send image length
    sprintf(char_len, "%d", toSend);
    send(client_socket, char_len, 10, 0);//hello world!!hei hei(xiao)!!  strlen(char_len)这里要写一个固定长度，然后让服务器端读出一个固定长度，否则会出错
    qDebug("char_len is %s\n",char_len);

    // send test

    //4.send image data
    while(toSend  >  0)
    {
        int size = qMin(toSend, 1000);//以前是1000
        if((receive = send(client_socket, send_buffer + finished, size, 0)))  //send wenzi
        {
            if(receive==-1)
            {
                printf ("receive error");
                break;
            }
            else
            {
                toSend -= receive;// shengxia de unsend
                finished += receive; //sended
            }
        }
    }

    //5.close socket
    close(client_socket);
    qDebug("close socket\n");
    return a.exec();
}
```


### （server）

```
#include <QCoreApplication>
//本文件是服务器的代码
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include <stdio.h>        // for QDebug
#include <stdlib.h>        // for exit
#include <string.h>        // for bzero
#include <time.h>                //for time_t and time
#include <unistd.h>
//#include <printf>

#define HELLO_WORLD_SERVER_PORT 7754
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ////////////////////////////////////
    //服务器代码
    ///////////////////////////////////

    //设置一个socket地址结构server_addr,代表服务器internet地址, 端口
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr)); //把一段内存区的内容全部设置为0
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    char file_name[10] = "25.jpg";
    char* file_buffer = new char[552 * 650 * 3];


    //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
    if( server_socket < 0)
        {
            printf("Create Socket Failed!");
            exit(1);
        }

    //把socket和socket地址结构联系起来
    if( bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr)))
        {
            printf("Server Bind Port : %d Failed!", HELLO_WORLD_SERVER_PORT);
            exit(1);
        }

        listen(server_socket, LENGTH_OF_LISTEN_QUEUE);

    while (1) //服务器端要一直运行
    {

        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);
        //accept() kai shi jie shou shu ju
        int new_server_socket = accept(server_socket,(struct sockaddr*)&client_addr,&length);
        if ( new_server_socket < 0)
        {
            printf("Server Accept Failed!\n");
            break;
        }

        char buffer[BUFFER_SIZE];
        bzero(buffer, BUFFER_SIZE);

        //send hello world
        strcpy(buffer,"Hello,World! 从服务器来！");
        strcat(buffer,"\n"); //C语言字符串连接

        send(new_server_socket,buffer,BUFFER_SIZE,0);

        bzero(buffer,BUFFER_SIZE);
        //接收客户端发送来的信息到buffer中
        length = recv(new_server_socket,buffer,BUFFER_SIZE,0);
        printf("%s\n",buffer);
        printf("regional file_name is %s\n",file_name);
        recv(new_server_socket, file_name, 10, 0);
        printf("received file_name is %s\n", file_name);

        char char_len[10];
        long file_length,read_length;
        char directory[100] = "/home/sa/abc/";
        int finished = 0;

        recv(new_server_socket, char_len, 10, 0);
        read_length = atoi(char_len);
        file_length = read_length;
        printf("received file_length is %d\n", read_length);

        while (read_length > 1000)
        {
            int receive = recv(new_server_socket, file_buffer + finished, 1000, 0);
            //strcpy(file_buff, buff);
            read_length -= receive;
            finished += receive;
        }
        read_length = recv(new_server_socket, file_buffer + finished, 1000, 0);

        FILE* fp = fopen(strcat(directory, file_name), "wb");
        if (fp == NULL)
            printf("create file failed!\n");
        else
        {

            fwrite(file_buffer, 1, file_length, fp);
            fclose(fp);
            printf("create file succed!\n");
        }

        //关闭与客户端的连接
        close(new_server_socket);
    }
    //关闭监听用的socket
    close(server_socket);

    return a.exec();
}
```


### 3.1.3从指定话题读取摄像头头像，并按序号保存

```
/*****
 * sub img from img topic:/camera/image_raw
 * *****/
//qt head

/***
 * ros img headfile
 * ****/
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <iostream>

using namespace std;
/****
 * socket headfile
 * ******/

unsigned int fileNum = 1;
bool imageSaveFlag;
void imageTransCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    //imgshow
    cv::imshow("show image", cv_bridge::toCvShare(msg, "bgr8")->image);
    char key;
    key=cv::waitKey(33);
    if(key==32)
      imageSaveFlag=true;
    if(imageSaveFlag)
    {
      stringstream fileName;
      stringstream filePath;
      fileName<<"goal rgbImage"<<fileNum<<".jpg";
      filePath<<"/home/nvidia/Travel_Assistance_Robot/image/"<<fileNum<<".jpg";
      string fn=fileName.str();
      string fp=filePath.str();
      cv::imwrite(fp,cv_bridge::toCvShare(msg, "bgr8")->image);
      imageSaveFlag =false;
      fileNum ++;
      cout<<fileName<<"had saved."<<endl;
    }
    //imgwrite



   //socket image trans

  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "img_listener");
  ros::NodeHandle nh;
  cv::namedWindow("show image");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("/camera/image_raw",1,imageTransCallback);


  ros::spin();

  cv::destroyWindow("view");

  return 0;
}
```


### 3.1.4、结合3.1.1和3.1.2，从指定话题获取图像，并使用socket将图像传输至服务器

//github 

[https://github.com/zuwuwang/qt_ros_ws/blob/master/src/guitest/src/socketsend_node.cpp
https://github.com/zuwuwang/qt_ros_ws/blob/master/src/guitest/include/guitest/socketsend_node.hpp](https://github.com/zuwuwang/qt_ros_ws/commits/master)


//将获话题消息放到回调函数中

//ROS下定时从节点获取图像


//将socket节点放到回调函数中


vim 查看图像的二进制数据



`Vim 可以用来查看和编辑二进制文件`

`vim -b fileName``   加上-b参数，以二进制打开`

`然后输入命令  :%!xxd -g 1  切换到十六进制模式显示,:wq退出`




## 三、服务器异常检测结果回传

        客户端C++代码发送图像，服务器端用python实现了接收图像，连续接收。然后接收到图像之后，输入到MCNN进行检测，检测结果回传。

// 桑永龙，发送检测结果

# 参考资料

[1]https://blog.csdn.net/zhuoyueljl/article/details/53557822

[2]  




