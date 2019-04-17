# IBM MQTT basic information and support platforms - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年09月10日 21:26:15[boonya](https://me.csdn.net/boonya)阅读数：830标签：[java																[ibm																[mqtt](https://so.csdn.net/so/search/s.do?q=mqtt&t=blog)
个人分类：[PROTOCOL](https://blog.csdn.net/boonya/article/category/1400121)





REF:[https://www.ibm.com/developerworks/community/groups/service/html/communityview?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070](https://www.ibm.com/developerworks/community/groups/service/html/communityview?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070)


A very small messaging server that uses the lightweight [MQTT](mqtt.org) publish/subscribe protocol to distribute messages between applications.

**News:** RSMB has now been Open sourced into the Eclipse Mosquitto project  [http://projects.eclipse.org/projects/technology.mosquitto](http://projects.eclipse.org/projects/technology.mosquitto)

Tags: [alphaworks](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=alphaworks),[alphaworks-group](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=alphaworks-group),[collaboration](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=collaboration),[computing](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=computing),[craggs](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=craggs),[eclipse](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=eclipse),[ian](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=ian),[messaging](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=messaging),[middleware](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=middleware),[networking](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=networking),[pervasive](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=pervasive),[rsmb](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=rsmb),[servers](https://www.ibm.com/developerworks/community/groups/service/html/allcommunities?tag=servers)






## Really%20Small%20Message%20Broker![](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)![Content for custom widget](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)







## What is Really Small Message Broker?


Really Small Message Broker (aka RSMB) is a small server that uses [MQ Telemetry Transport (MQTT)](http://mqtt.org) (Version 3 and 3.1) for lightweight, low-overhead messaging.  It enables messaging to and from tiny devices such as sensors and actuators over networks that might have low bandwidth, high cost, and varying reliability. "Publishers"
 send messages to the broker, which then distributes the messages to the "subscribers" who have requested to receive those messages.

RSMB has a "bridge" that enables connections to other MQTT-capable servers; this bridge allows messages to be passed between RSMB instances as well as to other MQTT servers such as WebSphere MQ.   RSMB can run in embedded systems in order to provide a messaging
 infrastructure in remote installations and pervasive environments.  Given Really Small Message Broker's low memory requirements, it can help extend the reach of the MQTT messaging infrastructure to the smallest components.

Really Small Message Broker makes it easy to connect brokers together into a messaging infrastructure (either peer-to-peer or hierarchically). It's easy configuration and low resource requirements provide great flexibility in messaging solutions for embedded
 environments. 



## How does it work?

RSMB is a small program that sends and receives data in the form of messages to and from applications and devices over TCP/IP network connections. Data from a variety of sources (including other applications, other brokers, and sensors and meters that measure
 their physical environment) is published to the broker. The broker then sends a copy of that data to applications that have registered their interest in that data by subscribing to it.

Really Small Message Broker takes up only 80 KB of storage space and can be run in only 200 KB or fewer of memory (actual figures vary), making it ideal for installing and running on small servers and embedded devices, such as the Linksys NSLU2 or other
 low powered servers. This low resource use is possible because the broker was written in C and has a simple internal design and simple external features. The package contains binary executables for Windows and Linux, as well as documentation.

An MQTT client for C is included. MQTT clients for Java and C are also available for download in the WebSphere MQ SupportPacs IA92 and IA93. The Java client in IA92 contains a useful MQTT Exerciser GUI sample. You can also write your own clients using the[MQTT
 specification](http://mqtt.org).






## About the technology author(s)


Ian Craggs is a software engineer at the IBM United Kingdom development laboratory at Hursley, near Winchester.





## Platform requirements


**Operating systems:**
- Linux for Intel 32-bit, kernel 2.6.8 or above
- Linux for Intel 64-bit, kernel 2.6.8 or above
- Linux on IBM System z
- Linux for ARM XScale, kernel 2.0.0 or above (Crossbow Stargate or Eurotech Viper)
- Windows XP
- Apple Mac OS X Leopard
- Unslung (Linksys NSLU2)








## Installation instructions





- Download the zip file.
- Extract the zip file to a location on your computer (for example, C:\Program Files\broker or /home/laura/broker).
- On Windows, install Microsoft Visual C++ 2008 Redistributable Package, if it is not already installed.



**Starting the broker on Windows:** In the folder where you installed the broker, select the windows folder and double-click broker.exe.

**Starting the broker on Linux:**
- In a terminal, change to the directory where you installed the broker (for example: cd /home/laura/broker).
- Change to the appropriate directory for your platform. For example, on Ubuntu, to linux_ia32.
- Enter the appropriate command for your platform. For example, on Ubuntu, type the following:


./broker


Or, in order to run the broker in the background so that it will continue to run when you log out of the session, enter the following command:


nohup ./broker >> /dev/null &












## Bookmarks![](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)





|![](https://1.www.s81c.com/i/c.gif)|#### [Eclipse Mosquitto Project](http://projects.eclipse.org/projects/technology.mosquitto)Updated byIanCraggs|Jan 23, 2014|Tags:|
|----|----|
|![](https://1.www.s81c.com/i/c.gif)|#### [IBM Mobile and Device Messaging Downloads](https://www.ibm.com/developerworks/mydeveloperworks/blogs/c565c720-fe84-4f63-873f-607d87787327/entry/download?lang=en)Updated byIanCraggs|Mar 16, 2013|Tags:|
|![](https://1.www.s81c.com/i/c.gif)|#### [Eclipse Paho Project](http://www.eclipse.org/paho/)Updated byIanCraggs|Mar 16, 2013|Tags:|
|![](https://1.www.s81c.com/i/c.gif)|#### [Download Really Small Message Broker](https://www14.software.ibm.com/webapp/iwm/web/reg/pick.do?source=swg-rlsmmsbk&lang=en_US)Updated byIanCraggs|Mar 13, 2013|Tags:[alphaworks](https://www.ibm.com/developerworks/community/groups/service/html/community/bookmarks?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070&tag=alphaworks)[download](https://www.ibm.com/developerworks/community/groups/service/html/community/bookmarks?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070&tag=download)|
|![](https://1.www.s81c.com/i/c.gif)|#### [Download Really Small Message Broker](https://www14.software.ibm.com/webapp/iwm/web/preLogin.do?source=AW-0U9)Updated byIanCraggs|Mar 13, 2013|Tags:[alphaworks](https://www.ibm.com/developerworks/community/groups/service/html/community/bookmarks?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070&tag=alphaworks)[download](https://www.ibm.com/developerworks/community/groups/service/html/community/bookmarks?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070&tag=download)|
|[View All](https://www.ibm.com/developerworks/community/groups/service/html/community/bookmarks?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070)| |








## Forums![](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)




||#### [Configure MQTT-SN topic ids in RSMB](https://www.ibm.com/developerworks/community/forums/html/topic?id=22f8a5a4-ca4c-4633-81ac-0996c19f13ee)Last post by DennisG|‎Oct 30, 2014|No replies|![0 people like this](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)0|
|----|----|----|
||#### [Can broker hold the payload until consumed?](https://www.ibm.com/developerworks/community/forums/html/topic?id=5625fccd-4d6d-4bd3-ab7f-c2e36be43f80)Last post by nat101|‎Sep 9, 2014|No replies|![0 people like this](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)0|
||#### [RSMB List of listener](https://www.ibm.com/developerworks/community/forums/html/topic?id=47a15ca9-bc7b-47bc-821b-9484b3e3e2f5)Last post by Guillaume73|‎Sep 26, 2013|No replies|![0 people like this](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)0|
||#### [stdinput gets data from a file](https://www.ibm.com/developerworks/community/forums/html/topic?id=a91496f6-b657-4c1a-b999-d610fc24696f)Last post by SebastiaNet|‎Nov 23, 2012|1 reply|![0 people like this](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)0|
||#### [RSMB on windows server 2008](https://www.ibm.com/developerworks/community/forums/html/topic?id=650e12ca-6b1e-41f9-b951-d132e23a5a64)Last post by benefice_alfa|‎Oct 31, 2012|No replies|![0 people like this](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)0|
|View All| | |







## Feeds![](https://www.ibm.com/developerworks/community/connections/resources/web/com.ibm.lconn.core.styles.oneui3/images/blank.gif?etag=20150310.220517)





|![](https://1.www.s81c.com/i/c.gif)|#### [MQTT.org](https://www.ibm.com/developerworks/community/groups/service/html/community/feeds?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070&feedUuid=38358c7b-3bf8-4f7d-9132-3fa66f062fc8)Updated byIanCraggs|Mar 14, 2013|
|----|----|
|[View All](https://www.ibm.com/developerworks/community/groups/service/html/community/feeds?communityUuid=d5bedadd-e46f-4c97-af89-22d65ffee070)| |](https://so.csdn.net/so/search/s.do?q=ibm&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




