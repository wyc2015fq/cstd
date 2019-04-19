# Android实现3G类usb模式自动转换 - xqhrs232的专栏 - CSDN博客
2012年01月09日 15:11:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：892
原文地址::[http://blog.csdn.net/maeom/article/details/6583858](http://blog.csdn.net/maeom/article/details/6583858)
环境系统：Android-2.3  板子：友善之臂提供s3c6410
当一个新的usb插入时，内核会发送一个uevent时间。上层接收到后，做出相应的处理。
如：我使用的为HuaWei ET127的3G网卡，插入arm板上后。没有正确识别成为ttyACM*设备。无法进行AT命令发送。这时。我们需要转换模式。
    我使用的是usb_modeswitch可以去官网下载，然后使用arm-gcc进行编译。
首先看一下消息流向
在Android-2.3/system/core/libsysutils/src/NetlinkListener.cpp文件中
[view plain](http://blog.csdn.net/maeom/article/details/6583858#)[copy to clipboard](http://blog.csdn.net/maeom/article/details/6583858#)[print](http://blog.csdn.net/maeom/article/details/6583858#)[?](http://blog.csdn.net/maeom/article/details/6583858#)
- bool NetlinkListener::onDataAvailable(SocketClient *cli)  
- {  
- int socket = cli->getSocket();  
- int count;  
- if ((count = recv(socket, mBuffer, sizeof(mBuffer), 0)) < 0) {  
- returnfalse;  
-     }     
- //这里buffer形式为：add@/devices/platform/s3c2410-ohci/usb1/1-1/1-1.2
-     NetlinkEvent *evt = new NetlinkEvent();  
- if (!evt->decode(mBuffer, count)) {  
- goto out;  
-     }   
- 
-     onEvent(evt);  
- out:  
- delete evt;  
- returntrue;  
- }  
bool NetlinkListener::onDataAvailable(SocketClient *cli)
{
    int socket = cli->getSocket();
    int count;
    if ((count = recv(socket, mBuffer, sizeof(mBuffer), 0)) < 0) {
        return false;
    }   
    //这里buffer形式为：add@/devices/platform/s3c2410-ohci/usb1/1-1/1-1.2
    NetlinkEvent *evt = new NetlinkEvent();
    if (!evt->decode(mBuffer, count)) {
        goto out;
    } 
    onEvent(evt);
out:
    delete evt;
    return true;
}这里的decode调用的为
[view plain](http://blog.csdn.net/maeom/article/details/6583858#)[copy to clipboard](http://blog.csdn.net/maeom/article/details/6583858#)[print](http://blog.csdn.net/maeom/article/details/6583858#)[?](http://blog.csdn.net/maeom/article/details/6583858#)
- //Android-2.3/system/core/libsysutils/src/NetlinkEvent.cpp  这里循环将buffer数据提取出来
- bool NetlinkEvent::decode(char *buffer, int size) {  
- while (s < end) {  
- if (first) {  
-             ......  
- for (p = s; *p != '@'; p++);  
-             mPath = strdup(p);  
-             .......  
-         } else {  
- if (!strncmp(s, "ACTION=", strlen("ACTION="))) {  
- char *a = s + strlen("ACTION=");  
- if (!strcmp(a, "add"))  
-                     mAction = NlActionAdd;  
- elseif (!strcmp(a, "remove"))  
-                     mAction = NlActionRemove;  
- elseif (!strcmp(a, "change"))  
-                     mAction = NlActionChange;  
-             } elseif (!strncmp(s, "SEQNUM=", strlen("SEQNUM=")))  
-                 mSeq = atoi(s + strlen("SEQNUM="));  
- elseif (!strncmp(s, "SUBSYSTEM=", strlen("SUBSYSTEM=")))  
-                 mSubsystem = strdup(s + strlen("SUBSYSTEM="));  
- else
-                 mParams[param_idx++] = strdup(s);  
-         }     
-     }  
- }  
//Android-2.3/system/core/libsysutils/src/NetlinkEvent.cpp  这里循环将buffer数据提取出来
bool NetlinkEvent::decode(char *buffer, int size) {
    while (s < end) {
        if (first) {
            ......
            for (p = s; *p != '@'; p++);
            mPath = strdup(p);
            .......
        } else {
            if (!strncmp(s, "ACTION=", strlen("ACTION="))) {
                char *a = s + strlen("ACTION=");
                if (!strcmp(a, "add"))
                    mAction = NlActionAdd;
                else if (!strcmp(a, "remove"))
                    mAction = NlActionRemove;
                else if (!strcmp(a, "change"))
                    mAction = NlActionChange;
            } else if (!strncmp(s, "SEQNUM=", strlen("SEQNUM=")))
                mSeq = atoi(s + strlen("SEQNUM="));
            else if (!strncmp(s, "SUBSYSTEM=", strlen("SUBSYSTEM=")))
                mSubsystem = strdup(s + strlen("SUBSYSTEM="));
            else
                mParams[param_idx++] = strdup(s);
        }   
    }
}继续调用onEvent函数，会有两处调用onEvent函数
1：为vold  2：为netd 程序。 两者类似。 只看下vold 
[view plain](http://blog.csdn.net/maeom/article/details/6583858#)[copy to clipboard](http://blog.csdn.net/maeom/article/details/6583858#)[print](http://blog.csdn.net/maeom/article/details/6583858#)[?](http://blog.csdn.net/maeom/article/details/6583858#)
- //Android-2.3/system/vold/NetlinkHandler.cpp
- void NetlinkHandler::onEvent(NetlinkEvent *evt) {  
-     VolumeManager *vm = VolumeManager::Instance();  
- constchar *subsys = evt->getSubsystem();  
- 
- if (!strcmp(subsys, "block")) {  
-         vm->handleBlockEvent(evt);  
-     } elseif (!strcmp(subsys, "usb_composite")) {  
-         vm->handleUsbCompositeEvent(evt);  
- //这里没有usb，我们添加一项，如下3行 
-     } elseif (!strcmp(subsys, "usb")) {  
-         vm->handleUsbEvent(evt);    //此函数需要在 Event类中添加
-     }     
- }  
//Android-2.3/system/vold/NetlinkHandler.cpp
void NetlinkHandler::onEvent(NetlinkEvent *evt) {
    VolumeManager *vm = VolumeManager::Instance();
    const char *subsys = evt->getSubsystem();
    if (!strcmp(subsys, "block")) {
        vm->handleBlockEvent(evt);
    } else if (!strcmp(subsys, "usb_composite")) {
        vm->handleUsbCompositeEvent(evt);
        //这里没有usb，我们添加一项，如下3行
    } else if (!strcmp(subsys, "usb")) {
        vm->handleUsbEvent(evt);    //此函数需要在 Event类中添加
    }   
}
[view plain](http://blog.csdn.net/maeom/article/details/6583858#)[copy to clipboard](http://blog.csdn.net/maeom/article/details/6583858#)[print](http://blog.csdn.net/maeom/article/details/6583858#)[?](http://blog.csdn.net/maeom/article/details/6583858#)
- //在/Android-2.3/system/vold/VolumeManager.h
- class VolumeManager {  
- public:  
- //增加如下一行 
- void handleUsbEvent(NetlinkEvent *evt);  
- }  
//在/Android-2.3/system/vold/VolumeManager.h
class VolumeManager {
public:
    //增加如下一行
    void handleUsbEvent(NetlinkEvent *evt);
}
[view plain](http://blog.csdn.net/maeom/article/details/6583858#)[copy to clipboard](http://blog.csdn.net/maeom/article/details/6583858#)[print](http://blog.csdn.net/maeom/article/details/6583858#)[?](http://blog.csdn.net/maeom/article/details/6583858#)
- //文件Android-2.3/system/vold/VolumeManager.cpp中，增加函数
- void VolumeManager::handleUsbEvent(NetlinkEvent *evt) {  
- constchar *devtype = evt->findParam("DEVTYPE");  
- char *cmd;  
- //如下判断设备类型，和是否为add模式。 进行相应操作
- if(1 == evt->getAction() && !strcmp(devtype, "usb_device")) {   
- /*call usb mode switch function*/
-         asprintf(&cmd, "usb_modeswitch -W -c /12d1\\:1da1");  //这里应该调用外部shell脚本，这里为测试程序。所以写死了
-         system(cmd);  
-         free(cmd);  
-     }      
- }  
