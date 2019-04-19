# 【ngxin】配置实现apk和ipa文件的下载 - Big Smile - CSDN博客
2018年04月29日 20:24:24[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1316
默认的情况下，nginx代理是不支持apk和ipa文件的下载的，那么这是为什么呢？
**1、MIME类型没有识别**
        我们存储的每种文件都是有格式才能被正确读取的，那我们又是怎么去识别这些格式的文件的呢？那就要用到MIME类型来规范对于文件的识别了。nginx不能很好的识别apk和ipa类型的文件也就在于此。所以第一步我们就要给nginx添加mime类型，让它能够识别这两种文件类型。
首先cd到nginx的conf目录下，找到mime.type，然后再里面加上如下两种：
```
application/vnd.android.package-archive apk;
application/iphone pxl ipa;
```
然后就可以正确识别了。然而还不够。
**2、访问限制打开**
我们在配置反向代理的时候，加入了文件类型的限制：
![](https://img-blog.csdn.net/20180429202244641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这里之前是没有加入的，现在加入了就可以了
