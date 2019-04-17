# linux下无线网卡的ioctl 接口 - DoubleLi - 博客园










var script = document.createElement('script'); script.src = 'http://static.pay.baidu.com/resource/baichuan/ns.js'; document.body.appendChild(script);







#define SIOCSIWSTATS 0x8B0E  /* Unused */ #define SIOCGIWSTATS 0x8B0F  /* 获取 
/proc/net/wireless的stats */  
/* Mobile IP support */ 
#define SIOCSIWSPY 0x8B10  /* 设置spy 地址*/ 
#define SIOCGIWSPY 0x8B11  /* 获取spy信息（连接质量） */ 

/* Access Point manipulation */ #define SIOCSIWAP 
0x8B14  
/* 设置AP的mac地址 */ #define SIOCGIWAP 0x8B15  
/* 获取AP的mac地址 */ 
#define SIOCGIWAPLIST 0x8B17  /* 获取周围Ap信息列表 
*/ 
#define SIOCSIWSCAN 0x8B18  /* 开始扫描 */ #define SIOCGIWSCAN 0x8B19  
/* 获取扫描信息 */ 

/* 802.11 specific support */ #define SIOCSIWESSID 0x8B1A  /* 设置ESSID  */ #define SIOCGIWESSID 
0x8B1B  /* 获取ESSID */ #define SIOCSIWNICKN 0x8B1C  /* 设置节点别名 */ #define SIOCGIWNICKN 0x8B1D  /* 获取节点别名 */ 





 var script = document.createElement('script'); script.src = 'http://static.pay.baidu.com/resource/baichuan/ns.js'; document.body.appendChild(script);








/* As the ESSID and NICKN are strings up to 32 bytes long, it doesn't fit 
 * within the 'iwreq' structure, so we need to use the 'data' member to 
 * point to a string in user space, like it is done for RANGE...  * The "flags" member indicate if the ESSID is active or not (promiscuous).  */  
/* Other parameters useful in 802.11 and some other devices */ #define SIOCSIWRATE 0x8B20  /* 设置默认传输速率(bps) */ #define SIOCGIWRATE 0x8B21  /* 获取默认传输速率 (bps) */ #define SIOCSIWRTS 0x8B22  /* 设置 RTS/CTS 的临界值
(bytes) */ 
#define SIOCGIWRTS 0x8B23  /* 获取 RTS/CTS 的临界值 (bytes) */ 
#define SIOCSIWFRAG 0x8B24  /* 设置分片传输的包大小 (bytes) */ 
#define SIOCGIWFRAG 0x8B25  
/*  获取分片传输的包大小 (bytes) */ 
#define SIOCSIWTXPOW 0x8B26  
/* 设置传输功率 (dBm) */ 




下载文档到电脑，查找使用更方便
2下载券  42人已下载


下载




还剩3页未读，继续阅读











#define SIOCGIWTXPOW 0x8B27  /* 获取传输功率(dBm) */ #define SIOCSIWRETRY 0x8B28  /* 设置重传次数和生存时
间 */ 
#define SIOCGIWRETRY 0x8B29  /*  获取重传次数和生存时
间  */  
/* Encoding stuff (scrambling, hardware security, WEP...) */ #define SIOCSIWENCODE 0x8B2A  /* 设置编码模式 */ 
#define SIOCGIWENCODE 
0x8B2B  
/* 获取编码模式 */ 
/* Power saving stuff (power management, unicast and multicast) */ 
#define SIOCSIWPOWER 0x8B2C  /* 设置电源管理模式 */ #define SIOCGIWPOWER 0x8B2D  /* 获取电源管理模式*/ 

/* -------------------- DEV PRIVATE IOCTL LIST -------------------- */  
/* These 16 ioctl are wireless device private. 
 * Each driver is free to use them for whatever purpose it chooses, 
 * however the driver *must* export the description of those ioctls 









 * with SIOCGIWPRIV and *must* use arguments as defined below. 
 * If you don't follow those rules, DaveM is going to hate you (reason : 
 * it make mixed 32/64bit operation impossible).  */ 
#define SIOCIWFIRSTPRIV 
0x8BE0 
#define SIOCIWLASTPRIV 0x8BFF 
/* Previously, we were using SIOCDEVPRIVATE, but we now have our 
 * separate range because of collisions with other tools such as  * 'mii-tool'. 
 * We now have 32 commands, so a bit more space -).  * Also, all 'odd' commands are only usable by root and don't return the 
 * content of ifr/iwr to user (but you are not obliged to use the set/get 
 * convention, just use every other two command). 
 * And I repeat : you are not obliged to use them with iwspy, but you 
 * must be compliant with it.  */ 










/* ------------------------- IOCTL STUFF ------------------------- */  
/* The first and the last (range) */ #define SIOCIWFIRST 0x8B00 
#define SIOCIWLAST SIOCIWLASTPRIV  
/* 0x8BFF */









