# Android开发框架afinal实践 - 专注于数据挖掘算法研究和应用 - CSDN博客





2014年10月22日 15:16:09[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2390








ADT下使用Android开发框架afinal，具体参考网址：http://blog.csdn.net/liranke/article/details/9499957







开发步骤：

1. 创建一个Android工程;

2. 加载aFinal包：

   将下载的jar包添加到Android工程里面。相信大家都会，不过也可能会遇到当添加了Afinal包后，继承FinalActivity的Activity会报java.lang.classNotFound这种错误，这时你最好将Afinal包直接拷贝到Android工程的lib文件夹里面。这时你看Android的Dependencies包里面如果有了Afinal包就再次运行APK应该就不会有错了。

3. 一般地，你的Activity是继承自Activity，把它改为继承自FinalActivity，如果有叉，只需要importFinalActivity 即可.



4. 在AndroidManifest.xml中添加以下权限：

   <uses-permission?android:name="android.permission.INTERNET" />

   <uses-permission?android:name="android.permission.WRITE_EXTERNAL_STORAGE" />



5. 修改控件的定义，以及相关联的事件监听器。

activity_main.xml上增加button1


最后，呈现的代码如下,以MyFinalActivity.java为例：

package com.example.finaltest;


import net.tsz.afinal.FinalActivity;  //新引入的类

import net.tsz.afinal.annotation.view.ViewInject;  //新引入的类

import android.os.Bundle;

import android.app.Activity;

import android.view.Menu;

import android.view.MenuItem;

import android.view.View;

import android.widget.Button;

import android.widget.TextView;

import android.support.v4.app.NavUtils;



public class MyFinalActivity extends FinalActivity  {



//无需调用findViewById和setOnclickListener等

    @ViewInject(id=R.id.button1,click="btnClick") Button button;



    @Override

    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_my_final);

    }


    //不用setOnclickListener

    public void btnClick(View v){

    button.setText("aFinal tes ok !!!");

     }


    @Override

    public boolean onCreateOptionsMenu(Menu menu) {

        getMenuInflater().inflate(R.menu.activity_my_final, menu);

        return true;

    }




