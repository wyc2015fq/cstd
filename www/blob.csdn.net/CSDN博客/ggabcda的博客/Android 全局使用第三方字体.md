# Android 全局使用第三方字体 - ggabcda的博客 - CSDN博客





2017年08月16日 15:53:22[ggabcda](https://me.csdn.net/ggabcda)阅读数：386








给APP全局设置字体主要分为两个方面来介绍

一、给原生界面设置第三方字体

　　1、准备工作-下载第三方字体：传送门

　　　　将文件放入工程assets目录下。（一般个人习惯单独命名一个文件夹放字体文件，也可直接放入根目录，但记得改引用路径）

![](http://images2017.cnblogs.com/blog/545676/201708/545676-20170815112911506-1182557663.png)



　　2、代码实现

　　　　a、自定义application，将第三方的字体，替换当前系统默认字体

![](http://images2017.cnblogs.com/blog/545676/201708/545676-20170815114243646-1376406210.png)

　　　　b、定义style



```
<style name="AppTheme.NoActionBar">
        <item name="windowActionBar">false</item>
        <item name="windowNoTitle">true</item>
        <item name="android:typeface">monospace</item>
</style>
```

　　　　c、设置在manifests文件中设置application



```
<application
        android:name=".application.WeexApplication"
        android:allowBackup="true"
        android:icon="@drawable/app_icon"
        android:label="@string/app_name"
        android:roundIcon="@drawable/app_icon"
        android:supportsRtl="true"
        android:theme="@style/AppTheme.NoActionBar">
```

　　　　注：需要注意name是自定义application路径，theme是自定义风格

二、给网页设置第三方字体

　　1、加载本地HTML页面使用本地字体

　　这种情况比较容易，只需要在本地文件上添加JS代码即可

![](http://images2017.cnblogs.com/blog/545676/201708/545676-20170815115504490-395960702.png)

```
<style>

        @font-face {
            font-family: 'MyCustomFont';
            src: url('file:///android_asset/fonts/textstyle.ttf');
        }
        p{
            font-family:"MyCustomFont";
            font-size: x-large;
        }

        body {
            margin: 0;
        }
    </style>
```

```
<body  style='font-family:MyCustomFont;'>
```



　　2、加载网络HTML页面使用本地字体

 　　将网络页面字体转换可以参考如下网址：http://blog.csdn.net/aiynmimi/article/details/52777965



