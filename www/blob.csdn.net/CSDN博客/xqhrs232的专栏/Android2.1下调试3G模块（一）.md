# Android2.1下调试3G模块（一） - xqhrs232的专栏 - CSDN博客
2011年12月01日 18:34:25[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1328标签：[android																[compression																[command																[null																[encryption																[linux内核](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=encryption&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=compression&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=198546](http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=198546)
[Android2.1下调试3G模块（二）]()----[http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=203378](http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=203378)
[Android2.1下调试3G模块（三）]()----[http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=212476](http://blog.chinaunix.net/space.php?uid=25369701&do=blog&id=212476)
曾几何时在Android2.1下调试3G模块，曾几何时模块厂商提供的库不能用，曾几何时只能用自己的库痛苦地调试...这一切的一切都已成往事，昔日的成功在毫无保留下成为浮云，该忘了忘记了，该记得也跟着忘记了。现如今再次调3G模块，却不知道以前如何调试，看来还是要记下来。
我采用的是华为EM770W模块，支持WCDMA网络，由于华为提供的库只支持Android2.2，所以用Android2.1就需要自己修改库源码。
1.修改linux内核
（1）make menuconfig：
Device Drivers  --->
    <*> OHCI HCD support 
    [*] Network device support  --->
         <*>   PPP (point-to-point protocol) support
         [*]     PPP multilink support (EXPERIMENTAL)
         [*]     PPP filtering
         <*>     PPP support for async serial ports
         <*>     PPP support for sync tty ports
         <*>     PPP Deflate compression
         <*>     PPP BSD-Compress compression
         <*>     PPP MPPE compression (encryption) (EXPERIMENTAL)
         <*>     PPP over Ethernet (EXPERIMENTAL)
         <*>     PPP over L2TP (EXPERIMENTAL)
    [*] USB support  --->
         <*>   USB Serial Converter support  --->
               <*>   USB driver for GSM and CDMA modems
（2）增加EM770W的VID和PID
修改驱动文件drivers/usb/serial/option.c，增加以下代码
#define EM770W_OPTION_VENDOR_ID 0x12d1
#define EM770W_OPTION_PRODUCT_COLT 0x1001
static struct usb_device_id option_ids[] = {
{ USB_DEVICE(OPTION_VENDOR_ID, OPTION_PRODUCT_COLT) },
…
{ USB_DEVICE(EM770W_OPTION_VENDOR_ID , EM770W_ OPTION_PRODUCT_COLT) },
}
驱动修改后插上3G模块，kernel运行后可在/dev下出现ttyUSB0、ttyUSB1  、ttyUSB2、ttyUSB3、ttyUSB4、ttyUSB5和ppp设备文件 
2.修改RIL代码
（1）在ril/reference-ril/Android.mk添加一行：
LOCAL_CFLAGS += -DHUAWEI_EM770W
（2）在ril/reference-ril/atchannel.c中增加的代码
- #include<termios.h>
- 
- static
int urc_fd =
-1;/* fd of the URC channel */
- static
char s_URCBuffer[MAX_AT_RESPONSE+1];
- static
char*s_URCBufferCur 
= s_URCBuffer;
- static
pthread_t s_tid_reader_urc;
- staticconstchar
*urc_readline()
- {
-     ssize_t count;
- char
*p_read =NULL;
- char
*p_eol =NULL;
- char
*ret;
- 
- if
(*s_URCBufferCur 
=='\0'){
-         s_URCBufferCur 
= s_URCBuffer;
- *s_URCBufferCur
='\0';
-         p_read = s_URCBuffer;
- }
else{
- while(*s_URCBufferCur
==
'\r'||*s_URCBufferCur =='\n')
-             s_URCBufferCur++;
-         p_eol = findNextEOL(s_URCBufferCur);
- if
(p_eol ==NULL)
{
- size_t len;
-             len =strlen(s_URCBufferCur);
- memmove(s_URCBuffer, s_URCBufferCur, len
+ 1);
-             p_read = s_URCBuffer
+ len;
-             s_URCBufferCur 
= s_URCBuffer;
- }
- }
- while
(p_eol ==NULL)
{
- if
(0 == MAX_AT_RESPONSE
-(p_read 
- s_URCBuffer)){
-             LOGE("ERROR: Input line exceeded buffer\n");
-             s_URCBufferCur 
= s_URCBuffer;
- *s_URCBufferCur
='\0';
-             p_read = s_URCBuffer;
- }
- do
{
- count=read(urc_fd, p_read,
MAX_AT_RESPONSE -
(p_read - s_URCBuffer));
- }
while(count< 0 &&errno== EINTR);
- if
(count> 0){
-             AT_DUMP("<< ", p_read,count);
-             s_readCount 
+=count;
-             p_read[count]='\0';
- while(*s_URCBufferCur
==
'\r'||*s_URCBufferCur =='\n')
-                 s_URCBufferCur++;
-             p_eol = findNextEOL(s_URCBufferCur);
-             p_read +=count;
- }
elseif(count<= 0){
- if(count== 0){
-                 LOGD("atchannel: EOF reached");
- }else{
-                 LOGD("atchannel: read error %s",strerror(errno));
- }
- returnNULL;
- }
- }
-     ret = s_URCBufferCur;
- *p_eol 
='\0';
-     s_URCBufferCur = p_eol
+ 1;
-     LOGD("AT< %s\n", ret);
- return ret;
- }
- 
- static
void*urc_readerLoop(void*arg)
- {
- for
(;;){
- constchar*
line;
- line
= urc_readline();
- if
(line==NULL)
{
- break;
- }
- if(isSMSUnsolicited(line)){
- char*line1;
- constchar*line2;
-             line1 = strdup(line);
-             line2 = readline();
- if(line2 ==NULL)
{
- break;
- }
- if(s_unsolHandler !=NULL)
{
-                 s_unsolHandler 
(line1, line2);
- }
- free(line1);
- }
else{
-             processLine(line);
- }
- }
-     onReaderClosed();
- return
NULL;
- }
（3）修改ril/reference-ril/atchannel.c中的at_open函数，增加HUAWEI_EM770W宏控制的代码
-     pthread_attr_init(&attr);
- pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
- 
- #ifdef HUAWEI_EM770W
- int fd2 
=-1;
- while(fd2
< 0)
{
-         fd2 =open("/dev/ttyUSB2", O_RDWR);
- if
(fd2 < 0){
- perror("opening URC interface. retrying...");
- sleep(10);
- }
- }
- if(fd2
> 0)
{
-         urc_fd = fd2;
- struct termios
ios;
-         tcgetattr( fd2,&ios
);
- ios.c_lflag
= 0;
-         tcsetattr( fd2, TCSANOW,&ios
);
- }
-     ret =
pthread_create(&s_tid_reader_urc,&attr, urc_readerLoop,&attr);
- if
(ret < 0){
- perror("pthread_create");
- return-1;
- }
- #endif
- 
-     ret =
pthread_create(&s_tid_reader,&attr, readerLoop,&attr);
（4）在ril/reference-ril/reference-ril.c中修改的代码
- +#include<cutils/properties.h>
- 
- -#define PPP_TTY_PATH
"/dev/omap_csmi_tty1"
- +#define PPP_TTY_PATH
"/dev/ppp0"
- 
- -
/* Not muted */
- - at_send_command("AT+CMUT=0",NULL);
- +
/* Set muted */
- + at_send_command("AT+CMUT=1",NULL);
- 
- -
if( fd >= 0
&&
!memcmp( s_device_path,"/dev/ttyS", 9
))
{
- +
if( fd >= 0){
- 
- /*    
-      asprintf(&cmd, "AT+CGDCONT=1,\"IP\",\"%s\",,0,0", apn);
-      //FIXME check for error here
-      err = at_send_command(cmd, NULL);
-      free(cmd);
-      // Set required QoS params to default
-      err = at_send_command("AT+CGQREQ=1", NULL);
-      // Set minimum QoS params to default
-      err = at_send_command("AT+CGQMIN=1", NULL);
-      // packet-domain event reporting
-      err = at_send_command("AT+CGEREP=1,0", NULL);
-      // Hangup anything that's happening there now
-      err = at_send_command("AT+CGACT=1,0", NULL);
-      // Start data on PDP context 1
-      err = at_send_command("ATD*99***1#", &p_response);
-      if (err < 0 || p_response->success == 0) {
-          goto error;
-      }
-      */
- 
- +
property_set("ctl.start","pppd_gprs");
-    RIL_onRequestComplete(t, RIL_E_SUCCESS, response,sizeof(response));
-    at_response_free(p_response);
（5）在ril/rild/rild.c中修改的代码 
- +#if 0
- /* special override when in the emulator */
- -#if 1
- {
- staticchar* arg_overrides[3];
- staticchar arg_device[32];
- 
-  //switchUser();

