# OpenCV学习笔记（三十）——解开VideoInput面纱highgui

2011年11月25日 17:27:07

yang_xian521

阅读数：15079

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

最近做一个东西，摄像头使用的高清摄像头，采集出来的视频是D1格式（720*480）。使用VideoCapture发现速度很忙，网上的朋友说VideoCapture提供的读写功能采用VFW，效率低下且有些格式支持不好。而 OpenCV 2.0 内置了videoInput Library，可以自动在VFW和DirectShow间切换。videoInput是老外写的功能强大的开源视频处理库。是一个第三方库，2.0~2.2的版本专门有一个3rdparty对该库进行支持，而在最新的2.3版本中，已经讲videoInput库集成到highgui中了，想使用它的话，只需要在cmake中设置宏WITH_VIDEOiNPUT=OFF/ON即可。好像我使用的2.3.1自带的那个build文件夹下面的库就是在ON条件下编译得到的，所以就不用cmake重新编译了。2.3中使用手册和教程对VideoInput类只字未提，我只好自己摸索了。还好有源代码可以看，开源伟大。

网上见过其他朋友写过2.2实现VideoInput的使用，我实验发现2.3中的使用方法基本没有变化。后面再把配套例程奉上，先把VideoInput类内的公有成员函数一一介绍一下，该类还有个相关的类是VideoDevice。包括控制是否在控制台输出信息开关setVerbose函数，打印出可用视频设备信息的函数listDevices，之后可以得到设备名函数getDeviceName，视频捕捉的回调函数设置函数setUseCallback，调整捕捉帧率的函数setIdealFramerate（默认30fps，可修改，但不能被保证准确，directshow会尝试一个邻近的帧率），防止设备休眠重新连接的函数setAutoReconnectOnFreeze，开启设备函数setupDevice，在setpuDevice之前可以设置视频制式，调用函数为setFormat，检测是否有新的帧函数isFrameNew，检测视频是否开启isDeviceSetup，获得数据的函数getPixels（注意这里获得的数据时uchar型的指针），显示视频设置窗口函数showSettingsWindow，控制视频设置的相关函数有setVideoSettingFilter、setVideoSettingFilterPct、getVideoSettingFilter、setVideoSettingCamera、setVideoSettingCameraPct、getVideoSettingCamera，获得视频宽高信息的函数有getWidth、getHeight、getSize，停止设备函数stopDevice，重启设备函数restartDevice。

讲了这么多函数，还是直接上代码说话吧，我这是找的VideoInput注释中的一个例程。



```cpp
//create a videoInput object
	videoInput VI;
	
	//Prints out a list of available devices and returns num of devices found
	int numDevices = VI.listDevices();	
	
	int device1 = 0;  //this could be any deviceID that shows up in listDevices
	int device2 = 1;  //this could be any deviceID that shows up in listDevices
	
	//if you want to capture at a different frame rate (default is 30) 
	//specify it here, you are not guaranteed to get this fps though.
	//VI.setIdealFramerate(dev, 60);	
	
	//setup the first device - there are a number of options:
	
	VI.setupDevice(device1); 						  //setup the first device with the default settings
	//VI.setupDevice(device1, VI_COMPOSITE); 			  //or setup device with specific connection type
	//VI.setupDevice(device1, 320, 240);				  //or setup device with specified video size
	//VI.setupDevice(device1, 320, 240, VI_COMPOSITE);  //or setup device with video size and connection type
 
	//VI.setFormat(device1, VI_NTSC_M);					//if your card doesn't remember what format it should be
														//call this with the appropriate format listed above
														//NOTE: must be called after setupDevice!
	
	//optionally setup a second (or third, fourth ...) device - same options as above
	VI.setupDevice(device2); 						  
 
	//As requested width and height can not always be accomodated
	//make sure to check the size once the device is setup
 
	int width 	= VI.getWidth(device1);
	int height 	= VI.getHeight(device1);
	int size	= VI.getSize(device1);
	
	unsigned char * yourBuffer1 = new unsigned char[size];
	unsigned char * yourBuffer2 = new unsigned char[size];
	
	//to get the data from the device first check if the data is new
	if(VI.isFrameNew(device1)){
		VI.getPixels(device1, yourBuffer1, false, false);	//fills pixels as a BGR (for openCV) unsigned char array - no flipping
		VI.getPixels(device1, yourBuffer2, true, true); 	//fills pixels as a RGB (for openGL) unsigned char array - flipping!
	}
	
	//same applies to device2 etc
	
	//to get a settings dialog for the device
	VI.showSettingsWindow(device1);
	
	
	//Shut down devices properly
	VI.stopDevice(device1);
	VI.stopDevice(device2);
```