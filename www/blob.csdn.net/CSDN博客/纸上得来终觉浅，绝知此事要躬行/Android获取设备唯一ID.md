# Android获取设备唯一ID - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月19日 12:04:14[boonya](https://me.csdn.net/boonya)阅读数：27089








Android的开发者在一些特定情况下都需要知道手机中的唯一设备ID。例如，跟踪应用程序的安装，生成用于复制保护的DRM时需要使用设备的唯一ID。在本文档结尾处提供了作为参考的示例代码片段。

**范围**

本文提供有关如何读取各种Android设备的 ID的介绍，用以使用标识号。本文假定用户已经安装了Android以及开发应用程序必要的工具。并且，本文假定用户已了解Android的基本知识。

**简介**

在搭载Android操作系统的设备中，已经存在好几种类型的设备标识号。先前的所有Android设备都具有电话功能，因此查找每部设备硬件唯一的IMEI，MEID，或ESN也很容易。但仅能使用Wifi的设备或音乐播放器没有电话硬件，所以没有这种类型的唯一标识号。本文阐述了如何读取不同Android设备的标识号。

**检索Android设备ID各种方式**

以下是Android设备不同类型的识别设备ID。

· 唯一编号（IMEI，MEID，ESN，IMSI）

· MAC地址

· 序列号

· ANDROID_ID



**唯一编号（IMEI，MEID，ESN，IMSI）**

**说明**在以前，当Android设备均作为电话使用时，寻找唯一标识号比较简单：（）可用于找到（取决于网络技术）手机硬件唯一的IMEI，MEID，ESN和IMSI编号。


TelephonyManager.getDeviceId

**IMEI，MEID，ESN，IMSI的定义如下：**

•IMEI（国际移动设备识别码）唯一编号，用于识别 GSM，WCDMA手机以及一些卫星电话（移动设备识别码）全球唯一编号，用于识别CDMA移动电台设备的物理硬件，MEID出现的目的是取代ESN号段（电子序列号）（电子序列号）唯一编号，用于识别CDMA手机（国际移动用户识别码）与所有GSM和UMTS网络手机用户相关联的唯一识别编号如需要检索设备的ID，在项目中要使用以下代码：




•MEID


•ESN


•IMSI



**[java]**[view
 plain](http://blog.csdn.net/aminfo/article/details/7604451#)[copy](http://blog.csdn.net/aminfo/article/details/7604451#)



- import android.telephony.TelephonyManager;     
- 
- import android.content.Context;                                                        
- 
- 
- 
- String   imeistring = null;                                                           
- 
- String   imsistring = null;                                                                                        
- 
- {                                                                                      
- 
-     TelephonyManager    telephonyManager;                                              
- 
- 
- 
-     telephonyManager =   
- 
-          ( TelephonyManager )getSystemService( Context.TELEPHONY_SERVICE );  
- 
- 
- 
- /*
- 
-       * getDeviceId() function Returns the unique device ID.
- 
-      * for example,the IMEI for GSM and the MEID or ESN for CDMA phones. 
- 
-      */
- 
-     imeistring = telephonyManager.getDeviceId();  
- 
- 
- 
- /*
- 
-     * getSubscriberId() function Returns the unique subscriber ID,
- 
-  * for example, the IMSI for a GSM phone.
- 
-  */
- 
-    imsistring = telephonyManager.getSubscriberId();     
- 
- }  


如要只读取手机的状态，则需添加READ_PHONE_STATE许可到AndroidManifest.xml文件中。



**[java]**[view
 plain](http://blog.csdn.net/aminfo/article/details/7604451#)[copy](http://blog.csdn.net/aminfo/article/details/7604451#)



- <uses-permission android:name="android.permission.READ_PHONE_ST




**缺点**

•Android设备要具有电话功能


•其工作不是很可靠


•序列号


•当其工作时，该值保留了设备的重置信息（“恢复出厂设置”），从而可以消除当客户删除自己设备上的信息，并把设备转另一个人时发生的错误。


**Mac地址说明**

可通过检索找到设备的Wi - Fi或蓝牙硬件的Mac地址。但是，不推荐使用Mac地址作为唯一的标识号。

**缺点**设备要具备Wi – Fi功能（并非所有的设备都有Wi – Fi功能）如果设备目前正在使用Wi
 - Fi，则不能报告Mac地址


**序列号**

从Android 2.3（“姜饼”）开始，通过android.os.Build.SERIAL方法序列号可被使用。没有电话功能的设备也都需要上给出唯一的设备ID; 某些手机也可以需要这样做。序列号可以用于识别MID（移动互联网设备）或PMP（便携式媒体播放器），这两种设备都没有电话功能。通过读取系统属性值“ro.serialno”的方法，可以使用序列号作为设备ID 。
- import java.lang.reflect.Method;                                    
- 
- 
- 
- String serialnum = null;                                                                                                                                          
- 
- try {                                                              
- 
-  Class<?> c = Class.forName("android.os.SystemProperties");   
- 
-  Method get = c.getMethod("get", String.class, String.class );       
- 
-  serialnum = (String)(   get.invoke(c, "ro.serialno", "unknown" )  );     
- 
- }                                                                                  
- 
- catch (Exception ignored)                                                          
- 
- {                                
- 
- }  

缺点




序列号无法在所有Android设备上使用。


ANDROID_ID


说明


更具体地说，Settings.Secure.ANDROID_ID 是一串64位的编码（十六进制的字符串），是随机生成的设备的第一个引导，其记录着一个固定值，通过它可以知道设备的寿命（在设备恢复出厂设置后，该值可能会改变）。 ANDROID_ID也可视为作为唯一设备标识号的一个好选择。如要检索用于设备ID 的ANDROID_ID，请参阅下面的示例代码



**[java]**[view
 plain](http://blog.csdn.net/aminfo/article/details/7604451#)[copy](http://blog.csdn.net/aminfo/article/details/7604451#)



- String androidId = Settings.Secure.getString(getContentResolver(),Settings.Secure.ANDROID_ID);  





**缺点**

• 对于Android 2.2（“Froyo”）之前的设备不是100％的可靠


• 此外，在主流制造商的畅销手机中至少存在一个众所周知的错误，每一个实例都具有相同的ANDROID_ID。

**结论**

对于绝大多数应用来说，只需识别特定的安装配置，而不需要识别物理设备。所幸是，这样做就省去了麻烦。

下面是部分使用设备ID的最佳途径：


•支持各种设备类型的另一种方法是使用getDeviceID（）API和ro.serialno的组合


•有许多值得参考的原因，来提醒开发者避免试图识别特定的设备。对于那些想做一下这方面尝试的用户，最好的办法可能是使用ANDROID_ID，并在一些传统设备上做尝试。




