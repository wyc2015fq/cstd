# Android Studio2.2 中支持NDK开发HelloJNI例子 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年11月26日 17:52:49[gloomyfish](https://me.csdn.net/jia20003)阅读数：2280








### Android Studio 2.2 中支持NDK开发HelloJNI例子

首先说一下运行的开发环境 

* Win7 64位 

* Android Studio 2.2 

* NDK版本是64位 r13b 

首先在AndroidStuido中创建一个空白项目，创建好之后，选择【File】->【project structure】显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20161126172707036)

添加好NDK支持，记得提前下载安装好就行啦。这里直接选择到安装好的路径即可。然后新建一个Java类，添加两个本地方法，保存之后。package basictutorial.gloomyfish.com.myndkdemo;

/**
 * Created by Administrator on 2016/11/26.
 */

public class HelloJNI {

    public native static String greet();

    public native static int sum(int a, int b);
}
看一下项目目录结构如下： 
![这里写图片描述](https://img-blog.csdn.net/20161126172904242)

选择【Build】->【Make Project】之后。就会在你的项目的如下目录： 

myndkdemo\app\build\intermediates\classes\debug生成编译好的class文件。 

然后在Android Studio中打开终端命令行 
![这里写图片描述](https://img-blog.csdn.net/20161126173010165)

通过cd目录进入到当前项目main目录，显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20161126173224501)

然后执行: 

javah -d jni -classpath .;%project_dir%/app\build\intermediates\classes\debug; basictutorial.gloomyfish.com.myndkdemo.HelloJNI 

执行javah命令就会生成jni目录与C++的头文件。然后到JNI目录里面新建一个文件取名hello.cpp。在Android Studio中刷新一下你的目录就可以看到 
![这里写图片描述](https://img-blog.csdn.net/20161126173522534)

双击打开Hello.cpp文件，把头文件中生成的两个方法copy到cpp文件中。然后做如下修改
```
#include <jni.h>
#include <basictutorial_gloomyfish_com_myndkdemo_HelloJNI.h>
#include <math.h>

/**
 * return string
 */
JNIEXPORT jstring JNICALL Java_basictutorial_gloomyfish_com_myndkdemo_HelloJNI_greet
  (JNIEnv *env, jclass thiz) {
    char* result = "this is call from JNI C++ side";
    jstring param3 = env->NewStringUTF(result);
    return param3;
}
/**
 * calculate and return
 */
JNIEXPORT jint JNICALL Java_basictutorial_gloomyfish_com_myndkdemo_HelloJNI_sum
  (JNIEnv *env, jclass thiz, jint a, jint b) {
    jint sum = pow(a, 2) + pow(b, 2);
    return sum;
}
```

然后开始通过【Build】->【Build Project】开始编译，如果得到如下错误 
![这里写图片描述](https://img-blog.csdn.net/20161126174005648)

在gradle.properties文件中添加如下语句 

android.useDeprecatedNdk=true 

然后继续执行编译动作，基本就OK了，这样我们就是实现了两个C++的功能一个是返回字符串进行问候、另外一个是计算两个数的平方和返回结果。设计一个Android界面可以输入两个数值、然后点击计算调用C++功能完成计算之后返回结果显示通过在Android Activity中调用实现整个过程，代码如下：
```java
package basictutorial.gloomyfish.com.myndkdemo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{

    static {
        System.loadLibrary("HelloJNI");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button calcBtn = (Button)this.findViewById(R.id.calculate_button);
        calcBtn.setOnClickListener(this);
        TextView txtView = (TextView)this.findViewById(R.id.my_textView);
        txtView.setText(HelloJNI.greet());
    }

    @Override
    public void onClick(View v) {
        EditText eidtText1 = (EditText)this.findViewById(R.id.editText1);
        EditText eidtText2 = (EditText)this.findViewById(R.id.editText2);
        String atext = eidtText1.getText().toString();
        String btext = eidtText2.getText().toString();
        int a = Integer.parseInt(atext);
        int b = Integer.parseInt(btext);
        TextView resultView = (TextView)this.findViewById(R.id.sum_textView);
        resultView.setText("平方和计算结果：" + HelloJNI.sum(a, b));
    }
}
```

运行显示结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20161126175849982)

XML的内容如下：
```xml
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:id="@+id/activity_main"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context="demo.jni.gloomyfish.com.jnidemoapp.MainActivity">

    <TextView
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:id="@+id/my_textView"
        android:text="Hello World!" />
    <LinearLayout
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_below="@id/my_textView"
        android:layout_alignParentLeft="true"
        android:layout_alignParentStart="true"
        android:layout_marginTop="10dp"
        android:id="@+id/input_numbers">
        <EditText
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:inputType="numberDecimal"
            android:layout_weight="1"
            android:text="10"
            android:hint="请输入数字..."
            android:ems="10"
            android:id="@+id/editText1" />

        <EditText
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_weight="1"
            android:inputType="numberDecimal"
            android:text="10"
            android:hint="请输入数字..."
            android:ems="10"
            android:id="@+id/editText2" />
    </LinearLayout>


    <TextView
        android:text="TextView"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"
        android:layout_below="@+id/input_numbers"
        android:layout_alignParentLeft="true"
        android:layout_alignParentStart="true"
        android:layout_marginTop="10dp"
        android:id="@+id/sum_textView" />
    <Button
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_below="@+id/sum_textView"
        android:layout_alignParentLeft="true"
        android:layout_alignParentStart="true"
        android:text="计算"
        android:layout_marginTop="10dp"
        android:id="@+id/calculate_button"/>
</RelativeLayout>
```

build.gradle的ndk相关脚本如下：

```bash
defaultConfig {
        applicationId "basictutorial.gloomyfish.com.myndkdemo"
        minSdkVersion 14
        targetSdkVersion 24
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "android.support.test.runner.AndroidJUnitRunner"

        ndk {
            moduleName "HelloJNI"
            ldLibs "log", "z", "m"
            abiFilters "armeabi", "armeabi-v7a", "x86"
        }
    }
```







