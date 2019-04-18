# JNI传递字符串数组J-StringArray - wishchinYang的专栏 - CSDN博客
2015年08月04日 11:56:12[wishchin](https://me.csdn.net/wishchin)阅读数：4212
         编译器对语言的转换以寻地址的方式进行序列化和反序列化，因此对于不固定类型或者不显示给出大小的对象不能直接解析，所以没有出现jstringArray这样的类型，只能一个一个编写。
        参考链接：[安卓开发提高篇-Jarray](http://tech.ddvip.com/2013-03/1363285417191986.html)
1.使用的C++函数
```cpp
//调用标定相机的函数!wishchin !!!
int CFrameProcess::CamCabPro(
	std::vector<std::string>  &filenames,
	Mat &intrinsic_Mat, Mat &disCoeff,
	Mat &object_points, Mat &axis)
{
	vector<Mat>  inputFrames;
	for (int i=0;i< filenames.size();++i)
	{
		Mat t = cv::imread(filenames[i].c_str(),1);
		inputFrames.push_back(t);
	}
	camCalib myCamCalib;
	MyCamera cam;
	bool isDetected = myCamCalib.cameraCab(inputFrames , cam);
	intrinsic_Mat  = cam.intrinsic_Mat.clone() ;
	disCoeff       = cam.disCoeff.clone()  ;
	object_points  = cam.object_pointsM.clone() ;
	axis           = cam.axisM.clone() ;
	return isDetected;
}
```
2.JNI接口函数
```cpp
extern "C" jint
Java_com_example_feeljulygpsmap_FeelJulyActivity_trackInitParamS
(JNIEnv *jenv, jclass
		,jobject thiz
		,jstring file0
		,jstring file1
		,jstring file2
		,jstring file3
		,jstring file4
		,jstring file5
		,jstring file6
		,jstring file7
		,jstring file8
		,jstring file9
		,jstring file10
		,jlong intrinsic_Mat,jlong disCoeff,jlong object_pointsM ,jlong axisM
)
{
	std::vector<std::string>  filenames(11);
	string stdFileName="";
	jStr2StdStr(jenv, file0,stdFileName);
	filenames[0]=stdFileName;
	jStr2StdStr(jenv, file1,stdFileName);
	filenames[1]=stdFileName;
	jStr2StdStr(jenv, file2,stdFileName);
	filenames[2]=stdFileName;
	jStr2StdStr(jenv, file3,stdFileName);
	filenames[3]=stdFileName;
	jStr2StdStr(jenv, file4,stdFileName);
	filenames[4]=stdFileName;
	jStr2StdStr(jenv, file5,stdFileName);
	filenames[5]=stdFileName;
	jStr2StdStr(jenv, file0,stdFileName);
	filenames[6]=stdFileName;
	jStr2StdStr(jenv, file7,stdFileName);
	filenames[7]=stdFileName;
	jStr2StdStr(jenv, file8,stdFileName);
	filenames[8]=stdFileName;
	jStr2StdStr(jenv, file9,stdFileName);
	filenames[9]=stdFileName;
	jStr2StdStr(jenv, file10,stdFileName);
	filenames[10]=stdFileName;
	((CFrameProcess*)thiz)->CamCabPro(
			filenames,
			*((Mat*)intrinsic_Mat)  , *((Mat*)disCoeff) ,
			*((Mat*)object_pointsM) , *((Mat*)axisM)    );
	return 1;
}
```
转换函数：
```cpp
inline void jStr2StdStr(JNIEnv *jenv, jstring SSrc,std::string SDst)
{
	const char* jnamestr = jenv->GetStringUTFChars(SSrc, NULL);
	string stdFileName(jnamestr);
	SDst =stdFileName;
}
```
3.Java层的定义和调用
调用：
```java
@SuppressLint("SdCardPath")
    private int trackInitParamJ(CamParam  mCamParam,Vector<String> filenames)
    {
    	//使用java初始化，读取文件！
    	//trackInitParam(mCamParam);
    	filenames.add(0,"/sdcard/DevCv/Picture/209.jpg");
    	filenames.add(1,"/sdcard/DevCv/Picture/240.jpg");
    	filenames.add(2,"/sdcard/DevCv/Picture/287.jpg");
    	filenames.add(3,"/sdcard/DevCv/Picture/300.jpg");
    	filenames.add(4,"/sdcard/DevCv/Picture/339.jpg");
    	filenames.add(5,"/sdcard/DevCv/Picture/356.jpg");
    	filenames.add(6,"/sdcard/DevCv/Picture/371.jpg");
    	filenames.add(7,"/sdcard/DevCv/Picture/404.jpg");
    	filenames.add(8,"/sdcard/DevCv/Picture/414.jpg");
    	filenames.add(9,"/sdcard/DevCv/Picture/743.jpg");
    	filenames.add(10,"/sdcard/DevCv/Picture/761.jpg");
    	trackInitParamS(
    			mNativeObjCvT
    			,filenames.get(0)
    			,filenames.get(1)
    			,filenames.get(2)
    			,filenames.get(3)
    			,filenames.get(4)
    			,filenames.get(5)
    			,filenames.get(6)
    			,filenames.get(7)
    			,filenames.get(8)
    			,filenames.get(9)
    			,filenames.get(10)
    			,mCamParam.intrinsic_Mat.getNativeObjAddr()
    			,mCamParam.disCoeff.getNativeObjAddr()
    			,mCamParam.object_pointsM.getNativeObjAddr() 
    			,(mCamParam.axisM).getNativeObjAddr()
    			);
    	
    	return 1;
    }
```
定义：
```java
private static native long trackInitParamS(
    		long thiz
    		,String file0
    		,String file1
    		,String file2
    		,String file3
    		,String file4
    		,String file5
    		,String file6
    		,String file7
    		,String file8
    		,String file9
    		,String file10
    		,long intrinsic_Mat, long disCoeff, long object_pointsM , long axisM );
```

