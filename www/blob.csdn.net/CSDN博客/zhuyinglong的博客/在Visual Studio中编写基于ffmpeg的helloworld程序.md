# 在Visual Studio中编写基于ffmpeg的helloworld程序 - zhuyinglong的博客 - CSDN博客





2017年09月13日 18:12:25[初见时的微笑](https://me.csdn.net/zhuyinglong2010)阅读数：212







1、新建一个空的WIN32控制台程序，添加cpp源文件，头文件（为了代码看起来方便，将与ffmpeg相关的声明写在的该文件中），如下图所示：


![](https://img-blog.csdn.net/20170913173333012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、在cpp源文件所在的目录中拷入include及lib文件，代码目录结构如下图：
![](https://img-blog.csdn.net/20170913173813923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3、右击进入项目属性对话框，配置方法如下图所示：

![](https://img-blog.csdn.net/20170913172716848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4、代码编写


config.h中的代码如下：

```cpp
#ifndef _CONFIGS_H_
#define _CONFIGS_H_

//C99整数范围常量. [纯C程序可以不用, 而C++程序必须定义该宏.]
#define  __STDC_LIMIT_MACROS
//C99整数常量宏. [纯C程序可以不用, 而C++程序必须定义该宏.]
#define  __STDC_CONSTANT_MACROS

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"swscale.lib")

extern "C"  
{  
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libpostproc/postprocess.h"
#include "libavcodec/avcodec.h"  
};
#endif
```
注意：该程序并不需要包含如此多的头文件及lib文件，为了方便以后使用添加。

helloworld.cpp代码是根据雷神的helloworld代码改写的C++代码，如下：



```cpp
#include <iostream>
#include "configs.h"

using namespace std;

int protocolinfo();
int avformatinfo();
int avcodecinfo();
int avfilterinfo();
int configurationinfo();

int main()
{
	av_register_all();
    avfilter_register_all();
	cout << "==============configurationinfo=============="<< endl;
    configurationinfo();
	cout << "==============protocolinfo==============" << endl;
	protocolinfo();
	cout << "==============avformatinfo==============" << endl;
	avformatinfo();
	cout << "==============avcodecinfo==============" << endl;
    avcodecinfo();
	cout << "==============avfilterinfo==============" << endl;
    avfilterinfo();   
	return 0;
}

int protocolinfo()
{
	void *opaque = NULL;
	const char *name;

	while ((name = avio_enum_protocols(&opaque, 0)))
	{ 
		cout << "[In]"<< name << endl;
	}
	cout << "====>Supported Output file protocols:" << endl;
	while ((name = avio_enum_protocols(&opaque, 1)))
	{
		cout <<"[Out]"<< name << endl;
	}
	return 0;
}

int avformatinfo()
{
	AVInputFormat *if_temp = av_iformat_next(NULL);
	AVOutputFormat *of_temp = av_oformat_next(NULL);

	while (if_temp != NULL) 
	{
		cout << "[In]" << if_temp->name << endl;
		if_temp = if_temp->next;
	}
	while (of_temp != NULL) 
	{
		cout << "[Out]" << of_temp->name << endl;
		of_temp = of_temp->next;
	}
	return 0;
}

int avcodecinfo()
{
	AVCodec *c_temp = av_codec_next(NULL);
	while (c_temp != NULL) 
	{
		if (c_temp->decode != NULL) 
		{
			cout << "[Dec]";
		}
		else 
		{
			cout << "[Enc]";
		}
		switch (c_temp->type) 
		{
			case AVMEDIA_TYPE_VIDEO:
				cout<<"[Video]";
				break;
			case AVMEDIA_TYPE_AUDIO:
				cout<<"[Audio]";
				break;
			default:
				cout<<"[Other]";
				break;
		}

		cout << c_temp->name << endl;
		c_temp = c_temp->next;
	}
	return 0;
}

int avfilterinfo()
{
	AVFilter *f_temp = (AVFilter *)avfilter_next(NULL);

	while (f_temp != NULL) {
		cout << f_temp->name << endl;
		f_temp = f_temp->next;
	}
	return 0;
}


int configurationinfo()
{
	cout << avcodec_configuration() << endl;
	return 0;
}
```


5、运行


编译完成之后在exe所在的Debug文件中拷入dll库，如下图所示：

![](https://img-blog.csdn.net/20170913180415906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


运行即可得到该版本ffmpeg库的基本信息，由于输出内容过长dos窗口显示不全，可使用">"将程序输入进行重定向到文件中，方便查看，如下图所示。



![](https://img-blog.csdn.net/20170913172704221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eWluZ2xvbmcyMDEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6、关于代码释义

强烈推荐使用source insight查看代码，上文中所涉及到的ffmpeg相关函数在头文件中都有详细的说明，在此不做介绍。

7、下载

ffmpeg-helloworld


github: [https://github.com/zhuyinglong/ffmpeg-helloworld](https://github.com/zhuyinglong/ffmpeg-helloworld)






