# 【Android小练手】安卓端实时获取HttpUrl数据 - CD's Coding - CSDN博客





2015年07月16日 10:02:37[糖果天王](https://me.csdn.net/okcd00)阅读数：2278








## 前言

关于上一篇文章中基于servlet的HttpUrl服务端，以时间为例的实时信息更新。

既然有写Server端，自然会要想如何应用这段信息，于是尝试着在安卓端想写一个demo，使用一个距离传感器，它们实时采集数据然后通过UART的方式把数据发送到PC端，然后PC端通过WIFI无线网络利用HttpUrl通信方式再将数据发送到手机，手机将数据按照一定的格式进行显示。




## 应用背景（实用性）

在解决了基础物质生活需求的今天，随着科技的不断进步，人们的生活受到了潜移默化的影响，生活质量也不断得到提高。促使人们开始追求家庭生活现代化、居住环境舒适化、安全化等，为此智能家居也随之而生，所谓的智能家居是以住宅为平台，利用综合布线技术、网络通信技术、安全防范技术、自动控制技术、音视频技术将家居生活有关的设施集成，构建高效的住宅设施与家庭日程事务的管理系统，提升家居安全性、便利性、舒适性、艺术性，并实现环保节能的居住环境。随着物联网络的出现，为智能家居系统的发展及应用加入了助推剂。物联网是指各类传感器与现有的互联网相互衔接的一种新技术。物联网让智能家居的功能产生了飞跃，它改变的将不仅仅是家电产品的功能，而是生活方式，给消费者带来的便捷令人向往。





## 需求分析

本次从实际出发，目的为实现使用手机应用通过无线对一个距离传感器实现监测和控制，是智能联网系统的基本需求。

获取传感器数值

通过串口传输数据

服务器端监听串口数据发送给手机端

手机端通过无线实时显示传感器数据





## 配置及环境



### 硬件配置

[@ZoeCur](http://my.csdn.net/u011702043) 1只

Sharp GP2Y0A21YK距离传感器

Nokia 5110 屏幕

TM4C Lanchpad

USB数据线 （模拟串口）

PC机(ThinkPad T430)

Android系统智能手机

### 开发环境

Windows 8

Keil v4

JDK 1.8

Eclipse EE（javacomm20-win32／RXTX_win64）

ADT

### 运行环境

Windows 8

JRE 1.8

javacomm20-win32／RXTX_win64

Android 4.4.4 （三星N7100）






## 设计理念

### Servlet设计

![](https://img-blog.csdn.net/20150716093652668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### Method设计

![](https://img-blog.csdn.net/20150716093657312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### Client-Android 设计

![](https://img-blog.csdn.net/20150716093702572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











## 数据分析

为了得到传感器返回的电压值与实际距离的关系，需要获得这两者间的函数关系，于是用厘米尺量取几个点用以数据拟合。



T=[999,860,660,450,370,270,200,150,130];

D=[5,10,15,20,25,30,40,60,80];

使用Matlab进行拟合

![](https://img-blog.csdn.net/20150716094145582?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


General model Exp2:

     f(x) = a*exp(b*x) + c*exp(d*x)

Coefficients (with 95% confidence bounds):

       a =        1944  (-371.5, 4260)

       b =    -0.02985  (-0.03951, -0.02019)

       c =       51.71  (42.08, 61.33)

       d =   -0.002019  (-0.002434, -0.001603)

Goodness of fit:

  SSE: 7.813

  R-square: 0.9984

  Adjusted R-square: 0.9974

  RMSE: 1.25


选用函数为：f(x)=1944*exp(-0.02985*x)+51.71*exp(-0.002019*x)考虑到计算复杂度和计算精度的问题，放弃高斯拟合选用了契合度稍低但容易计算的自然对数拟合函数。


将其写作函数，用来对电压值转换为距离：



```java
public String changeToDist(String str)
    {
    	double x=(double)Integer.parseInt(str);
		double val = 1944.0 * Math.exp(-0.02985*x) + 51.71 * Math.exp(-0.002019*x);
		String ret="";
		if(val>80.0)ret = " >80.00 cm";
			else if(val<5.0) ret = " <5.00 cm";
				else{
						ret = val + "";
						ret = ret.substring(0,6) + "cm";
					}
		System.out.println("[Log] Info_sensor="+ret);
		return ret;
    }
```






![](https://img-blog.csdn.net/20150716095058157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


服务器实时传送数值如上，可见数值转换正常。




## 关于麻烦的安卓端



```java
package com.example.sensor_app;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity extends Activity {
	private Button butt3 = null; 
	private TextView text1 = null;
	String value = null;
	private MyTask mTask;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	 super.onCreate(savedInstanceState);
         setContentView(R.layout.activity_main);
         
         butt3 = (Button) this.findViewById(R.id.button1);
         text1 = (TextView) this.findViewById(R.id.text1);
         
         butt3.setOnClickListener(new Button.OnClickListener()
     	{
 			@Override
 			public void onClick(View arg0) {
 				// TODO Auto-generated method stub
 				mTask = new MyTask();
 				mTask.execute("http://192.168.191.1:8080/SensorApp/ReadDemo?op=getValue");
 				butt3.setEnabled(false);
 			}			
     	});
    }
    
    private class MyTask extends AsyncTask<String,Integer,String>{
    	
    	
		@Override
		protected void onPreExecute() {
			//Log.i(TAG,"onPreExecute() called");
			text1.setText("loading");
			
		}

		@Override
		protected String doInBackground(String... params) {
			//Log.i(, "doInBackGround()call");
			
			try {  
                HttpClient client = new DefaultHttpClient();  
                HttpGet get = new HttpGet(params[0]);  
                HttpResponse response = null;
              
                while(true){
                	response = client.execute(get); 
	                if (response.getStatusLine().getStatusCode() == HttpStatus.SC_OK) {  
	                    HttpEntity entity = response.getEntity();  
	                    InputStream is = entity.getContent();  
	                    long total = entity.getContentLength();  
	                    ByteArrayOutputStream baos = new ByteArrayOutputStream();  
	                    byte[] buf = new byte[1024];  
	                    int count = 0;  
	                    int length = -1;  
	                    while ((length = is.read(buf)) != -1) {  
	                        baos.write(buf, 0, length);  
	                        count += length;  
	                        //调用publishProgress公布进度,最后onProgressUpdate方法将被执行  
	                       // publishProgress((int) ((count / (float) total) * 100));  
	                        //为了演示进度,休眠500毫秒 
	                       
	                        
	                    }  
	                    //更新当前值
	                    value = new String(baos.toByteArray(), "gb2312");
	                    //调用onProgressUpdate(Integer... values)
	                    publishProgress((int) ((count / (float) total) * 100));
	                   //一秒后继续更新
	                    Thread.sleep(1000); 
	                    //return new String(baos.toByteArray(), "gb2312");  
	                }
	                /*Thread.sleep(1000);  
	                i++;
	                publishProgress(i,5);*/
                }  
            } catch (Exception e) {  
                //Log.e(TAG, e.getMessage());  
            }  
            return null;  
		}

		@Override
		protected void onProgressUpdate(Integer... values) {
			// TODO Auto-generated method stub
			text1.setText(value);
		}

		@Override
		protected void onPostExecute(String result) {
			// TODO Auto-generated method stub
			text1.setText(result); 
		}
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
```


难点在于开线程以及同步实时更新数值。



关于控件的信号槽问题是Android的通病，不过与本次实验尝试无关，就不在这里写这些的处理方法了。




![](https://img-blog.csdn.net/20150716095231213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





调试完毕后，连接手机打开ADB调试，开始运行安卓客户端。


![](https://img-blog.csdn.net/20150716095601806)![](https://img-blog.csdn.net/20150716095606105)

如图，此为在安卓OS中的APP图标及打开时的初始界面。

紧接着，在JavaEE中打开服务端，开始运行，上图为服务端控制台数据，也同时编写Log日志，实时向客户端传值。





![](https://img-blog.csdn.net/20150716095613627)![](https://img-blog.csdn.net/20150716095617492)




电路板纹理的正六边形为启动按钮，安卓端接收到服务端确认后，将实时显示服务器传来的数据，

由于服务端的串口数据接收间隔为5秒，当接收到新数据时会显示<NEW>以作提示。




## 总结

此次课程设计中，我负责的主要是服务端与安卓客户端的部分，辅以对数据在Matlab中进行数据拟合。

过程中查阅了许多资料，为了新功能进行了若干次尝试，这是一次非常有收获的经历。



从串口获取数据之后，服务器需要持续地将串口数据读取到程序中，通过我拟合出的“传感器电压值-实际距离值”函数将String类型的信息转换成实际距离，加上传感器型号与服务器时间后传递给网络端口。另一个难点在于一次请求之后要无限实时信息传递，加上各类异常和新数据的判定，最终终于完成了之后，成就感和自豪油然而生。





## Release

SourceCode： 

https://github.com/okcd00/Colorful_Projects/tree/master/J2EE_Projects/Android-Server-Sensor%20%E4%BF%A1%E6%81%AF%E4%BC%A0%E9%80%92




