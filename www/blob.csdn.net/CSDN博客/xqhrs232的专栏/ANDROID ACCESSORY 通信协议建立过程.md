# ANDROID ACCESSORY 通信协议建立过程 - xqhrs232的专栏 - CSDN博客
2016年12月05日 15:53:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：591
原文地址::[http://blog.sina.com.cn/s/blog_6100a4f101018cbe.html](http://blog.sina.com.cn/s/blog_6100a4f101018cbe.html)
相关文章
1、ANDROID ACCESSORY 通信协议建立过程 
 ----[http://zwz94.blog.163.com/blog/static/3206039520123374251251/](http://zwz94.blog.163.com/blog/static/3206039520123374251251/)
2、[Android USB下的Accessory模式](http://blog.csdn.net/feiyangxiaomi/article/details/19617179)----[http://blog.csdn.net/feiyangxiaomi/article/details/19617179](http://blog.csdn.net/feiyangxiaomi/article/details/19617179)
3、Android Open Accessory Protocol----[http://wenku.baidu.com/link?url=d5WZvIF9GgxKCzXT58qXpUrYjkgzdFOSxrU8wLSpcTChlwwxiJ4lT_qwFD9ZERbwbd3prBE2iHbrPRu547AaZGyynPzJzP-DpuoSA7g4zC7](http://wenku.baidu.com/link?url=d5WZvIF9GgxKCzXT58qXpUrYjkgzdFOSxrU8wLSpcTChlwwxiJ4lT_qwFD9ZERbwbd3prBE2iHbrPRu547AaZGyynPzJzP-DpuoSA7g4zC7)
## http://zwz94.blog.163.com/blog/static/3206039520123374251251/
## http://zwz94.blog.163.com/blog/#m=0
http://developer.android.com/tools/adk/index.html
## Implementing the Android Accessory Protocol
An Android USB accessory must adhere to Android Accessory Protocol, which defines how an accessory detects and sets up communication with an Android-powered device. In general, an accessory should carry out the following steps:
- 
Wait for and detect connected devices
- 
Determine the device's accessory mode support
- 
Attempt to start the device in accessory mode if needed
- 
Establish communication with the device if it supports the Android accessory protocol
The following sections go into depth about how to implement these steps.
### Wait for and detect connected devices
Your accessory should have logic to continuously check for connected Android-powered devices. When a device is connected, your accessory should determine if the device supports accessory mode.
accessory作为host，设备插入后，accessory需要能够检测出该设备是否支持accessory功能。
### Determine the device's accessory mode support
When an Android-powered device is connected, it can be in one of three states:
- 
The attached device supports Android accessory mode and is already in accessory mode.
- 
The attached device supports Android accessory mode, but it is not in accessory mode.
- 
The attached device does not support Android accessory mode.
During the initial connection, the accessory should check the vendor and product IDs of the connected device's USB device descriptor. The vendor ID should match Google's ID (0x18D1) and the product ID should be 0x2D00 or 0x2D01 if the device is already in accessory
 mode (case A). If so, the accessory can now [establish communication with the device](http://developer.android.com/guide/topics/usb/adk.html#establish) through bulk transfer endpoints
 with its own communication protocol. There is no need to start the device in accessory mode.
google默认的accessory vid/pid，如果工作在accessory模式，可直接与android设备建立专属的通信协议。不用再启动accessory模式了。
**Note:** 0x2D00 is reserved for Android-powered devices that support accessory mode. 0x2D01 is reserved for devices that support accessory mode as well as the ADB (Android Debug Bridge) protocol, which exposes a second interface with two bulk
 endpoints for ADB. You can use these endpoints for debugging the accessory application if you are simulating the accessory on a computer. In general, do not use this interface unless your accessory is implementing a passthrough to ADB on the device.
If the vendor and product ID do not match, there is no way to distinguish between states b and c, so the accessory [attempts
 to start the device in accessory mode](http://developer.android.com/guide/topics/usb/adk.html#start) to figure out if the device is supported.
### Attempt to start the device in accessory mode
If the vendor and product IDs do not correspond to an Android-powered device in accessory mode, the accessory cannot discern whether the device supports accessory mode and is not in that state, or if the device does not support accessory mode at all. This is
 because devices that support accessory mode but aren't in it initially report the device's manufacturer vendor ID and product ID, and not the special Android Open Accessory ones. In either case, the accessory should try to start the device into accessory mode
 to figure out if the device supports it. The following steps explain how to do this:
- 
Send a 51 control request ("Get Protocol") to figure out if the device supports the Android accessory protocol. A non-zero number is returned if the protocol is supported, which represents the version of the protocol that the device supports (currently, only
 version 1 exists). This request is a control request on endpoint 0 with the following characteristics:
requestType:USB_DIR_IN | USB_TYPE_VENDORrequest:51value:0index:0data: protocol version number (16 bits little endian sent from the device to the accessory)
- 
If the device returns a proper protocol version, send identifying string information to the device. This information allows the device to figure out an appropriate application for this accessory and also present the user with a URL if an appropriate application
 does not exist. These requests are control requests on endpoint 0 (for each string ID) with the following characteristics:
requestType:USB_DIR_OUT | USB_TYPE_VENDORrequest:52value:0index:string IDdata  zero terminated UTF8 string sent from accessory to device
The following string IDs are supported, with a maximum size of 256 bytes for each string (must be zero terminated with \0).
manufacturer name:0model name:1description:2version:3URI:4serial number:5
- 
When the identifying strings are sent, request the device start up in accessory mode. This request is a control request on endpoint 0 with the following characteristics:
requestType:USB_DIR_OUT | USB_TYPE_VENDORrequest:53value:0index:0data: none
After sending the final control request, the connected USB device should re-introduce itself on the bus in accessory mode and the accessory can re-enumerate the connected devices. The algorithm jumps back to [determining
 the device's accessory mode support](http://developer.android.com/guide/topics/usb/adk.html#determine) to check for the vendor and product ID. The vendor ID and product ID of the device will be different if the device successfully switched to accessory mode and will now correspond to Google's vendor and product IDs instead
 of the device manufacturer's IDs. The accessory can now [establish communication with the device](http://developer.android.com/guide/topics/usb/adk.html#establish).
If at any point these steps fail, the device does not support Android accessory mode and the accessory should wait for the next device to be connected.
当android设备首次和accessory连接时，accessory并不知道设备是否支持accessory模式，支持哪些accessory协议，设备也未必正好工作在accessory模式，所以需要发送命令来尝试启动accessory模式。如果启动失败，说明不支持，否则说明支持。
### Establish communication with the device
If an Android-powered device in accessory mode is detected, the accessory can query the device's interface and endpoint descriptors to obtain the bulk endpoints to communicate with the device. An Android-powered device that has a product ID of 0x2D00 has one
 interface with two bulk endpoints for input and output communication. A device with product ID of 0x2D01 has two interfaces with two bulk endpoints each for input and output communication. The first interface is for standard communication while the second
 interface is for ADB communication. To communicate on an interface, all you need to do is find the first bulk input and output endpoints, set the device's configuration to a value of 1 with a SET_CONFIGURATION (0x09) device request, then communicate using
 the endpoints.
当检测出accessory模式后，建立通讯。给设备设置合适的配置及接口。
## How the ADK board implements the Android Accessory protocol
If you have access to the ADK board and shield, the following sections describe the firmware code that you installed onto the ADK board. The firmware demonstrates a practical example of how to implement the Android Accessory protocol. Even if you do not have
 the ADK board and shield, reading through how the hardware detects and interacts with devices in accessory mode is still useful if you want to port the code over for your own accessories.
The important pieces of the firmware are the `accessory/demokit/demokit/demokit.pde` sketch, which is the code that receives and sends data to the DemoKit
 application running on the Android-powered device. The code to detect and set up communication with the Android-powered device is contained in the `accessory/arduino_libs/AndroidAccessory/AndroidAccessory.h` and`accessory/arduino_libs/AndroidAccessory/AndroidAccessory.cpp` files.
 This code includes most of the logic that will help you implement your own accessory's firmware. It might be useful to have all three of these files open in a text editor as you read through these next sections.
The following sections describe the firmware code in the context of the algorithm described in[Implementing
 the Android Accessory Protocol](http://developer.android.com/guide/topics/usb/adk.html#accessory-protocol).
### Wait for and detect connected devices
In the firmware code (`demokit.pde`), the `loop()` function
 runs repeatedly and calls`AndroidAccessory::isConnected()` to check for any connected devices. If there is a connected device, it continuously updates
 the input and output streams going to and from the board and application. If nothing is connected, it continuously checks for a device to be connected:
...AndroidAccessory acc("Google, Inc.","DemoKit","DemoKit Arduino Board","1.0","http://www.android.com","0000000012345678");...void loop(){...if(acc.isConnected()){//communicate with Android application}else{//set the accessory to its default state}...}
### Determine the connected device's accessory mode support
When a device is connected to the ADK board, it can already be in accessory mode, support accessory mode and is not in that mode, or does not support accessory mode. The`AndroidAccessory::isConnected()` method
 checks for these cases and responds accordingly when the`loop()` function calls it. This function first checks to see if the device that is connected
 hasn't already been handled. If not, it gets the connected device's device descriptor to figure out if the device is already in accessory mode by calling `AndroidAccessory::isAccessoryDevice()`.
 This method checks the vendor and product ID of the device descriptor. A device in accessory mode has a vendor ID of 0x18D1 and a product ID of 0x2D00 or 0x2D01. If the device is in accessory mode, then the ADK board can [establish
 communication with the device](http://developer.android.com/guide/topics/usb/adk.html#establish). If not, the board [attempts to start the device in accessory mode](http://developer.android.com/guide/topics/usb/adk.html#start).
boolAndroidAccessory::isConnected(void){ USB_DEVICE_DESCRIPTOR *devDesc =(USB_DEVICE_DESCRIPTOR *) descBuff;byte err; max.Task(); usb.Task();if(!connected && usb.getUsbTaskState()>= USB_STATE_CONFIGURING && usb.getUsbTaskState()!= USB_STATE_RUNNING){Serial.print("\nDevice addressed... ");Serial.print("Requesting device descriptor."); err = usb.getDevDescr(1,0,0x12,(char*) devDesc);if(err){Serial.print("\nDevice descriptor cannot be retrieved. Program Halted\n");while(1);}if(isAccessoryDevice(devDesc)){Serial.print("found android accessory device\n"); connected = configureAndroid();}else{Serial.print("found possible device. switching to serial mode\n"); switchDevice(1);}}elseif(usb.getUsbTaskState()== USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE){ connected =false;}return connected;}
### Attempt to start the device in accessory mode
If the device is not already in accessory mode, then the ADK board must determine whether or not it supports it by sending control request 51 to check the version of the USB accessory protocol that the device supports (see `AndroidAccessory::getProtocol()`).
 Protocol version 1 is the only version for now, but this can be an integer greater than zero in the future. If the appropriate protocol version is returned, the board sends control request 52 (one for each string with`AndroidAcessory:sendString()`)
 to send it's identifying information, and tries to start the device in accessory mode with control request 53. The `AndroidAccessory::switchDevice()` method
 takes care of this:
boolAndroidAccessory::switchDevice(byte addr){int protocol = getProtocol(addr);if(protocol ==1){Serial.print("device supports protocol 1\n");}else{Serial.print("could not read device protocol version\n");returnfalse;} sendString(addr, ACCESSORY_STRING_MANUFACTURER, manufacturer); sendString(addr, ACCESSORY_STRING_MODEL, model); sendString(addr, ACCESSORY_STRING_DESCRIPTION, description); sendString(addr, ACCESSORY_STRING_VERSION, version); sendString(addr, ACCESSORY_STRING_URI, uri); sendString(addr, ACCESSORY_STRING_SERIAL, serial); usb.ctrlReq(addr,0, USB_SETUP_HOST_TO_DEVICE | USB_SETUP_TYPE_VENDOR |USB_SETUP_RECIPIENT_DEVICE, ACCESSORY_START,0,0,0,0, NULL);returntrue;}If this method returns false, the board waits until a new device is connected. If it is successful, the device displays
 itself on the USB bus as being in accessory mode when the ADK board re-enumerates the bus. When the device is in accessory mode, the accessory then[establishes
 communication with the device](http://developer.android.com/guide/topics/usb/adk.html#establish-adk).
### Establish communication with the device
If a device is detected as being in accessory mode, the accessory must find the proper bulk endpoints and set up communication with the device. When the ADK board detects an Android-powered device in accessory mode, it calls the `AndroidAccessory::configureAndroid()` function:
...if(isAccessoryDevice(devDesc)){Serial.print("found android acessory device\n"); connected = configureAndroid();}...
which in turn calls the `findEndpoints()` function:
...boolAndroidAccessory::configureAndroid(void){byte err; EP_RECORD inEp, outEp;if(!findEndpoints(1,&inEp,&outEp))returnfalse;...
The `AndroidAccessory::findEndpoints()` function queries the Android-powered device's configuration descriptor and finds the bulk data endpoints in which
 to communicate with the USB device. To do this, it first gets the device's first four bytes of the configuration descriptor (only need descBuff[2] and descBuff[3]), which contains the information about the total length of data returned by getting the descriptor.
 This data is used to determine whether or not the descriptor can fit in the descriptor buffer. This descriptor also contains information about all the interfaces and endpoint descriptors. If the descriptor is of appropriate size, the method reads the entire
 configuration descriptor and fills the entire descriptor buffer with this device's configuration descriptor. If for some reason the descriptor is no longer attainable, an error is returned.
...boolAndroidAccessory::findEndpoints(byte addr, EP_RECORD *inEp, EP_RECORD *outEp){int len;byte err;uint8_t*p; err = usb.getConfDescr(addr,0,4,0,(char*)descBuff);if(err){Serial.print("Can't get config descriptor length\n");returnfalse;} len = descBuff[2]|((int)descBuff[3]<<8);if(len >sizeof(descBuff)){Serial.print("config descriptor too large\n");returnfalse;} err = usb.getConfDescr(addr,0, len,0,(char*)descBuff);if(err){Serial.print("Can't get config descriptor\n");returnfalse;}...
Once the descriptor is in memory, a pointer is assigned to the first position of the buffer and is used to index the buffer for reading. There are two endpoint pointers (input and output) that are passed into `AndroidAccessory::findEndpoints()` and
 their addresses are set to 0, because the code hasn't found any suitable bulk endpoints yet. A loop reads the buffer, parsing each configuration, interface, or endpoint descriptor. For each descriptor, Position 0 always contains the size of the descriptor
 in bytes and position 1 always contains the descriptor type. Using these two values, the loop skips any configuration and interface descriptors and increments the buffer with the`descLen` variable
 to get to the next descriptor.
**Note:** An Android-powered device in accessory mode can potentially have two interfaces, one for the default communication to the device and the other for ADB communication. The default communication interface is always indexed first, so finding
 the first input and output bulk endpoints will return the default communication endpoints, which is what the `demokit.pde` sketch does. If you are writing
 your own firmware, the logic to find the appropriate endpoints for your accessory might be different.
android设备在accessory模式下一般有两个接口，第一个是默认的通信接口，第二是adb接口。
When it finds the first input and output endpoint descriptors, it sets the endpoint pointers to those addresses. If the findEndpoints() function finds both an input and output endpoint, it returns true. It ignores any other endpoints that it finds (the endpoints
 for the ADB interface, if present).
... p = descBuff; inEp->epAddr =0; outEp->epAddr =0;while(p <</span>(descBuff + len)){uint8_t descLen = p[0];uint8_t descType = p[1]; USB_ENDPOINT_DESCRIPTOR *epDesc; EP_RECORD *ep;switch(descType){case USB_DESCRIPTOR_CONFIGURATION:Serial.print("config desc\n");break;case USB_DESCRIPTOR_INTERFACE:Serial.print("interface desc\n");break;case USB_DESCRIPTOR_ENDPOINT: epDesc =(USB_ENDPOINT_DESCRIPTOR *)p;if(!inEp->epAddr &&(epDesc->bEndpointAddress &0x80)) ep = inEp;elseif(!outEp->epAddr) ep = outEp;else ep = NULL;if(ep){ ep->epAddr = epDesc->bEndpointAddress &0x7f; ep->Attr= epDesc->bmAttributes; ep->MaxPktSize= epDesc->wMaxPacketSize; ep->sndToggle = bmSNDTOG0; ep->rcvToggle = bmRCVTOG0;}break;default:Serial.print("unkown desc type ");Serial.println( descType, HEX);break;} p += descLen;}if(!(inEp->epAddr && outEp->epAddr))Serial.println("can't find accessory endpoints");return inEp->epAddr && outEp->epAddr;}...
Back in the `configureAndroid()` function, if there were endpoints found, they are appropriately set up for communication. The device's configuration
 is set to 1 and the state of the device is set to "running", which signifies that the device is properly set up to communicate with your USB accessory. Setting this status prevents the device from being re-detected and re-configured in the`AndroidAccessory::isConnected()` function.
boolAndroidAccessory::configureAndroid(void){byte err; EP_RECORD inEp, outEp;if(!findEndpoints(1,&inEp,&outEp))returnfalse; memset(&epRecord,0x0,sizeof(epRecord)); epRecord[inEp.epAddr]= inEp;if(outEp.epAddr != inEp.epAddr) epRecord[outEp.epAddr]= outEp;in= inEp.epAddr;out= outEp.epAddr;Serial.print("inEp: ");Serial.println(inEp.epAddr, HEX);Serial.print("outEp: ");Serial.println(outEp.epAddr, HEX); epRecord[0]=*(usb.getDevTableEntry(0,0)); usb.setDevTableEntry(1, epRecord); err = usb.setConf(1,0,1);if(err){Serial.print("Can't set config to 1\n");returnfalse;} usb.setUsbTaskState( USB_STATE_RUNNING );returntrue;}
Lastly, methods to read and write to the appropriate endpoints are needed. The `demokit.pde` sketch calls these methods depending on the data that is
 read from the Android-powered device or sent by the ADK board. For instance, moving the joystick on the ADK shield writes data that is read by the DemoKit application running on the Android-powered device. Moving sliders on the DemoKit application is read
 by the `demokit.pde` sketch and changes the state of the accessory, such as lighting up or changing the color of the LED lights.
intAndroidAccessory::read(void*buff,int len,unsignedint nakLimit){return usb.newInTransfer(1,in, len,(char*)buff, nakLimit);}intAndroidAccessory::write(void*buff,int len){ usb.outTransfer(1,out, len,(char*)buff);return len;}
See the `firmware/demokit/demokit.pde` file for information about how the ADK board reads and writes data.
