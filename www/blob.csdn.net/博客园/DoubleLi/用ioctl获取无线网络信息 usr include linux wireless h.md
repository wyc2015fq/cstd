# 用ioctl获取无线网络信息 /usr//include/linux/wireless.h - DoubleLi - 博客园







**1、UNIX Network Programming环境搭建**

Unix NetWork Programming――环境搭建（解决unp.h等源码编译问题）

http://blog.csdn.net/a649518776/article/details/6724121

注：按照连接操作即可，**编译要加 -lunp  选项**

**2、用户态和核心态的交换方式**

在 linux下，要实现核心态和用户态数据的交互，有多种方式：可以通用socket创建特殊套接字，利用套接字实现数据交互；通过proc文件系统创建文 件来进行数据交互；还可以使用设备文件的方式，访问设备文件会调用设备驱动相应的例程，设备驱动本身就是核心态和用户态的一个接口，Tun/tap驱动就 是利用设备文件实现用户态和核心态的数据交互。

**3、与系统错误有关的函数**

（1）errno

头文件：#include

函数原型：errno

功能：记录系统的最后一次错误代码

参数：无

返回值：错误代号（整型值）

例子：

if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) { 
        fprintf(stderr, "Cannot open socket /n"); 
**fprintf(stderr, "errno = %d /n", errno); ****  fprintf(stderr, "Error description is : %s/n",strerror(errno)); **        exit(1);

} 
printf("Socket opened successfully /n");

（2）strerror   strerror_r

头文件：#include

函数原型：

char *strerror(int errnum);

char *_strerror(const char *strErrMsg);

wchar_t * _wcserror(int errnum);

wchar_t * __wcserror(const wchar_t *strErrMsg);

参数：errnum――错误代码，strErrMsg――用户提供的信息。

返回值：指向错误信息的指针。

例子：同（1）

（3）perror

头文件：#include #include//

函数原型：void perror(const char *s); perror ("open_port");

函数功能：

perror ( )用 来 将 上 一 个 函 数 发 生 错 误 的 原 因 输 出 到 标 准 设备 (stderr) 。参数 s 所指的字符串会先打印出,后面再加上错误原因字符串。此错误原因依照全局变量errno 的值来决定要输出的字符串。

在库函数中有个errno变量，每个errno值对应着以字符串表示的错误类型。当你调用"某些"函数出错时，该函数已经重新设置了errno的值。perror函数只是将你输入的一些信息和现在的errno所对应的错误一起输出。

参数：用户输入的字符串

返回值：无

例子：

if(( sockfd = socket(AF_INET,SOCK_DGRAM,0))<0) 
    { 
        perror("error"); 
        exit(1); 
    } 
    else 
    { 
        printf("socket created successfully!/n"); 
        printf("socket id:%d/n",sockfd); 
        printf("remote ip:%s/n",REMOTEIP); 
        printf("remote port: %d/n/n",REMOTEPORT); 
    }

**总结：貌似perror比较方便**

**4、memset**

头文件：在C中 <memory.h> or 在C++中

函数原型：void *memset(void *s, int ch, size_t n);

函数功能：将s中前n个字节替换为ch并返回s；作用是在一段内存块中填充某个给定的值，它是对较大的**结构体或数组进行清零操作**的一种最快方法。

**注：如果不用memeset清零，可能会出现野值。**

**5、iwreq结构体  **

http://schwer.bewaff.net/code/_wireless/wireless.h

（1）头文件：#include


struct iwreq {	union	{	char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */	} ifr_ifrn;	/* Data part */	union	{	/* Config - generic */	char	name[IFNAMSIZ];	/* Name : used to verify the presence of wireless extensions.	* Name of the protocol/provider... */	struct iw_point	essid;	/* Extended network name */	struct iw_param	nwid;	/* network id (or domain - the cell) */	struct iw_freq	freq;	/* frequency or channel : * 0-1000 = channel	* > 1000 = frequency in Hz */	struct iw_param	sens;	/* signal level threshold */	struct iw_param	bitrate;	/* default bit rate */	struct iw_param	rts;	/* RTS threshold threshold */	struct iw_param	frag;	/* Fragmentation threshold */	__u32	mode;	/* Operation mode */	struct iw_point	encoding;	/* Encoding stuff : tokens */	struct iw_param	power;	/* PM duration/timeout */	struct sockaddr	ap_addr;	/* Access point address */ struct iw_point	data;	/* Other large parameters */	}	u;};（2）struct	iw_point{ caddr_t	pointer;	/* Pointer to the data (in user space) */ __u16	length;	/* number of fields or size in bytes */ __u16	flags;	/* Optional params */};struct	iw_param{ __s32	value;	/* The value of the parameter itself */ __u8	fixed;	/* Hardware should not use auto select */ __u8	disabled;	/* Disable the feature */ __u16	flags;	/* Various specifc flags (if any) */};其中typedef   void*   caddr_t 即caddr_t是void*型typedef unsigned short __u16 即__u16是unsigned short型**(以_t结尾的一般说明是typedef定义后的数据类型，typedef主要解决跨平台编译问题)**
**6、ifreq**

/* Interface request structure used for socket ioctl's.  All interface 
ioctl's must have parameter definitions which begin with ifr_name. 
The remainder may be interface specific.  */

struct ifreq 
{ 
# define IFHWADDRLEN 6 
# define IFNAMSIZ IF_NAMESIZE 
   union 
     { 
char ifrn_name[IFNAMSIZ]; /* Interface name, e.g. "en0".  */ 
     } ifr_ifrn;

   union 
     { 
struct sockaddr ifru_addr; 
struct sockaddr ifru_dstaddr; 
struct sockaddr ifru_broadaddr; 
struct sockaddr ifru_netmask; 
struct sockaddr ifru_hwaddr; 
short int ifru_flags; 
int ifru_ivalue; 
int ifru_mtu; 
struct ifmap ifru_map; 
char ifru_slave[IFNAMSIZ]; /* Just fits the size */ 
char ifru_newname[IFNAMSIZ]; 
__caddr_t ifru_data; 
     } ifr_ifru; 
}; 
# define ifr_name ifr_ifrn.ifrn_name /* interface name */ 
# define ifr_hwaddr ifr_ifru.ifru_hwaddr /* MAC address */ 
# define ifr_addr ifr_ifru.ifru_addr /* address */ 
# define ifr_dstaddr ifr_ifru.ifru_dstaddr /* other end of p-p lnk */ 
# define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast address */ 
# define ifr_netmask ifr_ifru.ifru_netmask /* interface net mask */ 
# define ifr_flags ifr_ifru.ifru_flags /* flags */ 
# define ifr_metric ifr_ifru.ifru_ivalue /* metric */ 
# define ifr_mtu ifr_ifru.ifru_mtu /* mtu */ 
# define ifr_map ifr_ifru.ifru_map /* device map */ 
# define ifr_slave ifr_ifru.ifru_slave /* slave device */ 
# define ifr_data ifr_ifru.ifru_data /* for use by interface */ 
# define ifr_ifindex ifr_ifru.ifru_ivalue    /* interface index      */ 
# define ifr_bandwidth ifr_ifru.ifru_ivalue /* link bandwidth */ 
# define ifr_qlen ifr_ifru.ifru_ivalue /* queue length */ 
# define ifr_newname ifr_ifru.ifru_newname /* New name */ 
# define _IOT_ifreq _IOT(_IOTS(char),IFNAMSIZ,_IOTS(char),16,0,0) 
# define _IOT_ifreq_short _IOT(_IOTS(char),IFNAMSIZ,_IOTS(short),1,0,0) 
# define _IOT_ifreq_int _IOT(_IOTS(char),IFNAMSIZ,_IOTS(int),1,0,0)



**注：其中sockaddr**

struct sockaddr {

> 
uint8_t sa_len;

sa_family_t sa_family;    /*地址族：AF_XXX*/

char sa_data[14];          /*14字节协议地址*/


}



**7、struct union**

**参考：http://hi.baidu.com/amoyedll/blog/item/900a811f4b515d164034176a.html**

（1）

struct 和union都是由多个不同的数据类型成员组成, 但在任何同一时刻, union中只存放了一个被选中的成员, 而struct的所有成员都存在。在struct中，各成员都占有自己的内存空间，它们是同时存在的。一个struct变量的总长度等于所有成员长度之 和。在Union中，所有成员不能同时占用它的内存空间，它们不能同时存在。Union变量的长度等于最长的成员的长度。


（2）对于union的不同成员赋值, 将会对其它成员重写, 原来成员的值就不存在了,而对于struct的不同成员赋值是互不影响的。**注：（１）、（２）两点是相通的，即union的成员共用一块内存（该块内存的大小是union****成员中占用内存长度最长的），而struct各成员占用自己的内存空间。**（3）例子

#include

void main() 
{ 
union{                                                /*定义一个联合*/ 
   int i; 
   struct{                                     /*在联合中定义一个结构*/ 
    char first; 
    char second; 
   }half; 
}number; 
number.i=0x4241; /*联合成员赋值*/ 
cout<<number.half.first<<number.half.second<<endl; 
number.half.first='a';                  /*联合中结构成员赋值*/ 
number.half.second='b'; 
cout<<hex<<number.i<<endl; /*每一个变量前都加一个 << hex    ,并且int型可以转换，而float、double是转化不了的*/ 
}

输出结果为: 
AB 
6261

在这里i 和 half结构是共用内存 
number.i=0x4241给i赋值后,内存中以二进制存储0100 0010 0100 0001 
按顺序对应到结构中 
halt.first=01000010   转换成10进制就是66（字母A的asc码） 
halt.second=01000001 转换成10进制是65   （字母B的asc码） 
所以输出后就是 AB
**8、wireless.h       ioctl中有关无线网络的request请求参数**
http://schwer.bewaff.net/code/_wireless/wireless.h


/* * This file define a set of standard wireless extensions * * Version :	9 16.10.99 * * Authors :	Jean Tourrilhes - HPL - <jt@hpl.hp.com> */#ifndef _LINUX_WIRELESS_H#define _LINUX_WIRELESS_H/************************** DOCUMENTATION **************************//* * Basically, the wireless extensions are for now a set of standard ioctl * call + /proc/net/wireless * * The entry /proc/net/wireless give statistics and information on the * driver. * This is better than having each driver having its entry because * its centralised and we may remove the driver module safely. * * Ioctl are used to configure the driver and issue commands. This is * better than command line options of insmod because we may want to * change dynamically (while the driver is running) some parameters. * * The ioctl mechanimsm are copied from standard devices ioctl. * We have the list of command plus a structure descibing the * data exchanged... * Note that to add these ioctl, I was obliged to modify : *	net/core/dev.c (two place + add include) *	net/ipv4/af_inet.c (one place + add include) * * /proc/net/wireless is a copy of /proc/net/dev. * We have a structure for data passed from the driver to /proc/net/wireless * Too add this, I've modified : *	net/core/dev.c (two other places) *	include/linux/netdevice.h (one place) *	include/linux/proc_fs.h (one place) * * Do not add here things that are redundant with other mechanisms * (drivers init, ifconfig, /proc/net/dev, ...) and with are not * wireless specific. * * These wireless extensions are not magic : each driver has to provide * support for them... * * IMPORTANT NOTE : As everything in the kernel, this is very much a * work in progress. Contact me if you have ideas of improvements... *//***************************** INCLUDES *****************************/#include /* for "caddr_t" et al	*/#include /* for "struct sockaddr" et al	*/#include /* for IFNAMSIZ and co... *//**************************** CONSTANTS ****************************//* --------------------------- VERSION --------------------------- *//* * This constant is used to know the availability of the wireless * extensions and to know which version of wireless extensions it is * (there is some stuff that will be added in the future...) * I just plan to increment with each new version. */#define WIRELESS_EXT	9/* * Changes : * * V2 to V3 * -------- *	Alan Cox start some incompatibles changes. I've integrated a bit more. *	- Encryption renamed to Encode to avoid US regulation problems *	- Frequency changed from float to struct to avoid problems on old 386 * * V3 to V4 * -------- *	- Add sensitivity * * V4 to V5 * -------- *	- Missing encoding definitions in range *	- Access points stuff * * V5 to V6 * -------- *	- 802.11 support (ESSID ioctls) * * V6 to V7 * -------- * - define IW_ESSID_MAX_SIZE and IW_MAX_AP * * V7 to V8 * -------- *	- Changed my e-mail address *	- More 802.11 support (nickname, rate, rts, frag) *	- List index in frequencies * * V8 to V9 * -------- *	- Support for 'mode of operation' (ad-hoc, managed...) *	- Support for unicast and multicast power saving *	- Change encoding to support larger tokens (>64 bits) *	- Updated iw_params (disable, flags) and use it for NWID *	- Extracted iw_point from iwreq for clarity *//* -------------------------- IOCTL LIST -------------------------- *//* Basic operations */#define SIOCSIWNAME	0x8B00	/* Unused ??? */#define SIOCGIWNAME	0x8B01	/* get name */#define SIOCSIWNWID	0x8B02	/* set network id (the cell) */#define SIOCGIWNWID	0x8B03	/* get network id */#define SIOCSIWFREQ	0x8B04	/* set channel/frequency */#define SIOCGIWFREQ	0x8B05	/* get channel/frequency */#define SIOCSIWMODE	0x8B06	/* set operation mode */#define SIOCGIWMODE	0x8B07	/* get operation mode */#define SIOCSIWSENS	0x8B08	/* set sensitivity */#define SIOCGIWSENS	0x8B09	/* get sensitivity *//* Informative stuff */#define SIOCSIWRANGE	0x8B0A	/* Unused ??? */#define SIOCGIWRANGE	0x8B0B	/* Get range of parameters */#define SIOCSIWPRIV	0x8B0C	/* Unused ??? */#define SIOCGIWPRIV	0x8B0D	/* get private ioctl interface info *//* Mobile IP support */#define SIOCSIWSPY	0x8B10	/* set spy addresses */#define SIOCGIWSPY	0x8B11	/* get spy info (quality of link) *//* Access Point manipulation */#define SIOCSIWAP	0x8B14	/* set access point MAC addresses */#define SIOCGIWAP	0x8B15	/* get access point MAC addresses */#define SIOCGIWAPLIST	0x8B17	/* get list of access point in range *//* 802.11 specific support */#define SIOCSIWESSID	0x8B1A	/* set ESSID (network name) */#define SIOCGIWESSID	0x8B1B	/* get ESSID */#define SIOCSIWNICKN	0x8B1C	/* set node name/nickname */#define SIOCGIWNICKN	0x8B1D	/* get node name/nickname *//* As the ESSID and NICKN are strings up to 32 bytes long, it doesn't fit * within the 'iwreq' structure, so we need to use the 'data' member to * point to a string in user space, like it is done for RANGE... * The "flags" member indicate if the ESSID is active or not (promiscuous). *//* Other parameters usefull in 802.11 and some other devices */#define SIOCSIWRATE	0x8B20	/* set default bit rate (bps) */#define SIOCGIWRATE 0x8B21	/* get default bit rate (bps) */#define SIOCSIWRTS	0x8B22	/* set RTS/CTS threshold (bytes) */#define SIOCGIWRTS	0x8B23	/* get RTS/CTS threshold (bytes) */#define SIOCSIWFRAG	0x8B24	/* set fragmentation thr (bytes) */#define SIOCGIWFRAG	0x8B25	/* get fragmentation thr (bytes) *//* Encoding stuff (scrambling, hardware security, WEP...) */#define SIOCSIWENCODE	0x8B2A	/* set encoding token & mode */#define SIOCGIWENCODE	0x8B2B	/* get encoding token & mode *//* Power saving stuff (power management, unicast and multicast) */#define SIOCSIWPOWER	0x8B2C	/* set Power Management settings */#define SIOCGIWPOWER	0x8B2D	/* get Power Management settings *//* Get statistics */#define SIOCGIWSTAT	0x8B2E	/* get an struct iw_statistics *//* ------------------------- IOCTL STUFF ------------------------- *//* The first and the last (range) */#define SIOCIWFIRST	0x8B00#define SIOCIWLAST	0x8B30/* Even : get (world access), odd : set (root access) */#define IW_IS_SET(cmd)	(!((cmd) & 0x1))#define IW_IS_GET(cmd)	((cmd) & 0x1)/* ------------------------- PRIVATE INFO ------------------------- *//* * The following is used with SIOCGIWPRIV. It allow a driver to define * the interface (name, type of data) for its private ioctl. * Privates ioctl are SIOCDEVPRIVATE -> SIOCDEVPRIVATE + 0xF */#define IW_PRIV_TYPE_MASK	0x7000	/* Type of arguments */#define IW_PRIV_TYPE_NONE	0x0000#define IW_PRIV_TYPE_BYTE	0x1000	/* Char as number */#define IW_PRIV_TYPE_CHAR	0x2000	/* Char as character */#define IW_PRIV_TYPE_INT	0x4000	/* 32 bits int */#define IW_PRIV_TYPE_FLOAT	0x5000#define IW_PRIV_SIZE_FIXED	0x0800	/* Variable or fixed nuber of args */#define IW_PRIV_SIZE_MASK	0x07FF	/* Max number of those args *//* * Note : if the number of args is fixed and the size < 16 octets, * instead of passing a pointer we will put args in the iwreq struct... *//* ----------------------- OTHER CONSTANTS ----------------------- *//* Maximum frequencies in the range struct */#define IW_MAX_FREQUENCIES	16/* Note : if you have something like 80 frequencies, * don't increase this constant and don't fill the frequency list. * The user will be able to set by channel anyway... *//* Maximum bit rates in the range struct */#define IW_MAX_BITRATES	8/* Maximum of address that you may set with SPY */#define IW_MAX_SPY	8/* Maximum of address that you may get in the list of access points in range */#define IW_MAX_AP	8/* Maximum size of the ESSID and NICKN strings */#define IW_ESSID_MAX_SIZE	32/* Modes of operation */#define IW_MODE_AUTO	0	/* Let the driver decides */#define IW_MODE_ADHOC	1	/* Single cell network */#define IW_MODE_INFRA	2	/* Multi cell network, roaming, ... */#define IW_MODE_MASTER	3	/* Synchronisation master or Access Point */#define IW_MODE_REPEAT	4	/* Wireless Repeater (forwarder) */#define IW_MODE_SECOND	5	/* Secondary master/repeater (backup) *//* Maximum number of size of encoding token available * they are listed in the range structure */#define IW_MAX_ENCODING_SIZES	8/* Maximum size of the encoding token in bytes */#define IW_ENCODING_TOKEN_MAX	32	/* 256 bits (for now) *//* Flags for encoding (along with the token) */#define IW_ENCODE_INDEX	0x00FF	/* Token index (if needed) */#define IW_ENCODE_FLAGS	0xF000	/* Flags defined below */#define IW_ENCODE_DISABLED	0x8000	/* Encoding disabled */#define IW_ENCODE_ENABLED	0x0000	/* Encoding enabled */#define IW_ENCODE_RESTRICTED	0x4000	/* Refuse non-encoded packets */#define IW_ENCODE_OPEN	0x2000	/* Accept non-encoded packets *//* Power management flags available (along with the value, if any) */#define IW_POWER_ON	0x0000	/* No details... */#define IW_POWER_TYPE	0xF000	/* Type of parameter */#define IW_POWER_PERIOD	0x1000	/* Value is a period/duration of */#define IW_POWER_TIMEOUT	0x2000	/* Value is a timeout (to go asleep) */#define IW_POWER_MODE	0x0F00	/* Power Management mode */#define IW_POWER_UNICAST_R	0x0100	/* Receive only unicast messages */#define IW_POWER_MULTICAST_R	0x0200	/* Receive only multicast messages */#define IW_POWER_ALL_R	0x0300	/* Receive all messages though PM */#define IW_POWER_FORCE_S 0x0400	/* Force PM procedure for sending unicast */#define IW_POWER_REPEATER	0x0800	/* Repeat broadcast messages in PM period *//****************************** TYPES ******************************//* --------------------------- SUBTYPES --------------------------- *//* *	Generic format for most parameters that fit in an int */struct	iw_param{ __s32	value;	/* The value of the parameter itself */ __u8	fixed;	/* Hardware should not use auto select */ __u8	disabled;	/* Disable the feature */ __u16	flags;	/* Various specifc flags (if any) */};/* *	For all data larger than 16 octets, we need to use a *	pointer to memory alocated in user space. */struct	iw_point{ caddr_t	pointer;	/* Pointer to the data (in user space) */ __u16	length;	/* number of fields or size in bytes */ __u16	flags;	/* Optional params */};/* *	A frequency *	For numbers lower than 10^9, we encode the number in 'm' and *	set 'e' to 0 *	For number greater than 10^9, we pide it by the lowest power *	of 10 to get 'm' lower than 10^9, with 'm'= f / (10^'e')... *	The power of 10 is in 'e', the result of the pision is in 'm'. */struct	iw_freq{	__u32	m;	/* Mantissa */	__u16	e;	/* Exponent */	__u8	i;	/* List index (when in range struct) */};/* *	Quality of the link */struct	iw_quality{	__u8	qual;	/* link quality (%retries, SNR or better...) */	__u8	level;	/* signal level */	__u8	noise;	/* noise level */	__u8 updated;	/* Flags to know if updated */};/* *	Packet discarded in the wireless adapter due to *	"wireless" specific problems... */struct	iw_discarded{	__u32	nwid;	/* Wrong nwid */	__u32	code;	/* Unable to code/decode */	__u32	misc;	/* Others cases */};/* ------------------------ WIRELESS STATS ------------------------ *//* * Wireless statistics (used for /proc/net/wireless) */struct iw_statistics{	__u16	status;	/* Status	* - device dependent for now */	struct iw_quality	qual;	/* Quality of the link	* (instant/mean/max) */	struct iw_discarded	discard;	/* Packet discarded counts */};/* ------------------------ IOCTL REQUEST ------------------------ *//* * The structure to exchange data for ioctl. * This structure is the same as 'struct ifreq', but (re)defined for * convenience... * * Note that it should fit on the same memory footprint ! * You should check this when increasing the above structures (16 octets) * 16 octets = 128 bits. Warning, pointers might be 64 bits wide... */struct	iwreq {	union	{	char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */	} ifr_ifrn;	/* Data part */	union	{	/* Config - generic */	char	name[IFNAMSIZ];	/* Name : used to verify the presence of wireless extensions.	* Name of the protocol/provider... */	struct iw_point	essid;	/* Extended network name */	struct iw_param	nwid;	/* network id (or domain - the cell) */	struct iw_freq	freq;	/* frequency or channel :	* 0-1000 = channel	* > 1000 = frequency in Hz */	struct iw_param	sens;	/* signal level threshold */	struct iw_param bitrate;	/* default bit rate */	struct iw_param	rts;	/* RTS threshold threshold */	struct iw_param	frag;	/* Fragmentation threshold */	__u32	mode;	/* Operation mode */	struct iw_point	encoding;	/* Encoding stuff : tokens */	struct iw_param	power;	/* PM duration/timeout */	struct sockaddr	ap_addr;	/* Access point address */ struct iw_point	data;	/* Other large parameters */	}	u;};/* -------------------------- IOCTL DATA -------------------------- *//* *	For those ioctl which want to exchange mode data that what could *	fit in the above structure... *//* *	Range of parameters */struct	iw_range{	/* Informative stuff (to choose between different interface) */	__u32	throughput;	/* To give an idea... */	/* In theory this value should be the maximum benchmarked	* TCP/IP throughput, because with most of these devices the * bit rate is meaningless (overhead an co) to estimate how * fast the connection will go and pick the fastest one.	* I suggest people to play with Netperf or any benchmark...	*/	/* NWID (or domain id) */	__u32	min_nwid;	/* Minimal NWID we are able to set */	__u32	max_nwid;	/* Maximal NWID we are able to set */	/* Frequency */	__u16	num_channels;	/* Number of channels [0; num - 1] */	__u8 num_frequency; /* Number of entry in the list */	struct iw_freq	freq[IW_MAX_FREQUENCIES];	/* list */	/* Note : this frequency list doesn't need to fit channel numbers */	/* signal level threshold range */	__s32	sensitivity;	/* Quality of link & SNR stuff */	struct iw_quality	max_qual;	/* Quality of the link */	/* Rates */	__u8	num_bitrates;	/* Number of entries in the list */	__s32 bitrate[IW_MAX_BITRATES];	/* list, in bps */	/* RTS threshold */	__s32	min_rts;	/* Minimal RTS threshold */	__s32	max_rts;	/* Maximal RTS threshold */	/* Frag threshold */	__s32	min_frag;	/* Minimal frag threshold */	__s32	max_frag;	/* Maximal frag threshold */	/* Power Management duration & timeout */	__s32	min_pmd;	/* Minimal PM duration */	__s32	max_pmd;	/* Maximal PM duration */	__s32 min_pmt;	/* Minimal PM timeout */	__s32	max_pmt;	/* Maximal PM timeout */	/* Encoder stuff */	__u16	encoding_size[IW_MAX_ENCODING_SIZES];	/* Different token sizes */	__u8	num_encoding_sizes;	/* Number of entry in the list */	__u8	max_encoding_tokens;	/* Max number of tokens */};/* * Private ioctl interface information */ struct	iw_priv_args{	__u32	cmd;	/* Number of the ioctl to issue */ __u16	set_args;	/* Type and number of args */	__u16	get_args;	/* Type and number of args */	char	name[IFNAMSIZ];	/* Name of the extension */};#endif	/* _LINUX_WIRELESS_H */ 

**9、ioctl获取ESSID 和AP的MAC**

/***************code description***************
get current connecting ESSID and its AP's MAC.
by fys 2012/8/9
****************code description***************/
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
using namespace std;

#define IW_INTERFACE "wlan0"

extern int errno;
struct iwreq wreq;

int main (void)
{
    int i;
    int sockfd;
    memset(&wreq, 0, sizeof(struct iwreq));
    sprintf(wreq.ifr_name, IW_INTERFACE);//?
    /*create a socket to get info from ioctl*/
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        //fprintf(stderr, "Cannot open socket /n");
        //fprintf(stderr, "errno = %d /n", errno);
        //fprintf(stderr, "Error description is : %s/n",strerror(errno));
        perror("Cannot creat socket,error");
        exit(1);
    }
    else {
        printf("Socket opened successfully /n");
    }
    /*create a socket to get info from ioctl*/

    char buffer[32];
    memset(buffer, 0, 32);
    wreq.u.essid.pointer = buffer;//if not write these codes , the program maybe wrong.
    wreq.u.essid.length = 32;

    /*get ESSID from ioctl*/
    if (ioctl(sockfd,SIOCGIWESSID, &wreq) == -1) {
        perror("IOCTL SIOCGIWESSID Failed,error");
        exit(2);
    }
    else {
        printf("IOCTL SIOCGIWESSID Successfull/n");
    }
    /*get ESSID from ioctl*/
    /*output current connecting essid*/
    printf("%s/n",wreq.u.essid.pointer);
    /*output current connecting essid*/
    /*get AP info from ioctl*/
    if (ioctl(sockfd,SIOCGIWAP, &wreq) == -1) {
        perror("IOCTL SIOCGIWAP Failed,error");
        exit(2);
    }
    else {
        printf("IOCTL SIOCGIWAP Successfull/n");
    }
    /*get AP info from ioctl*/

    /* output current connecting AP MAC*/
    for (i = 0; i < 6; i++) {
        unsigned char *APaddr = (unsigned char *) wreq.u.ap_addr.sa_data;
        printf("%02x", (int) APaddr[i]);//mac[i] means 1 byte ,i.e. 8 bits.
        if (i != 5)
            printf("%c", ':');
         else
            printf("/n");
    }
    /* output current connecting AP MAC*/
}












