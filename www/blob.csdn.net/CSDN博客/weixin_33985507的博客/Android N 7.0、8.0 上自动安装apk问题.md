# Android N 7.0、8.0 上自动安装apk问题 - weixin_33985507的博客 - CSDN博客
2018年07月10日 09:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## Android 7.0 上安装apk android.os.FileUriExposedException问题
> 
如果你的系统版本是 8.0+，那你需要多加一个权限，否则无法跳转到安装页
```
<uses-permission android:name="android.permission.REQUEST_INSTALL_PACKAGES"/>
```
如果安装报错，可能是临时文件访问路径没有配置，或者百度上找到的安装代码是旧版本的，7.0以后不在支持，文章最下面有适配的代码。
```
android.os.FileUriExposedException: file:///storage/emulated/0/trgis/1511427343635.apk exposed beyond app through Intent.getData()
```
今天做自动更新的时候，自己下载好的apk安装包调用系统的安装服务就报错，很是郁闷，因为之前的代码是好着的，后来查了下资料，原来是Android N 7.0版本之后不支持之前的写法了，好了直接上解决方案。
1.在AndroidManifest.xml  application标签中添加如下代码
```
<provider
            android:name="android.support.v4.content.FileProvider"
            android:authorities="你的包名.fileProvider"
            android:grantUriPermissions="true"
            android:exported="false">
            <meta-data
                android:name="android.support.FILE_PROVIDER_PATHS"
                android:resource="@xml/file_paths" />
        </provider>
```
*注意*
> 
authorities：你app的包名.fileProvider
grantUriPermissions：必须是true，表示授予 URI 临时访问权限
exported：必须是false
resource：中的@xml/file_paths是我们接下来要添加的文件
2.在res/xml下新建file_paths.xml
```
<?xml version="1.0" encoding="utf-8"?>
<resources>
<paths>
    <external-path
        name="files_root"
        path="" />
</paths>
</resources>
```
*注意*
> 
path：需要临时授权访问的路径（.代表所有路径）
name：就是你给这个访问路径起个名字
3.适配AndroidN
- 
以前我们直接 Uri.fromFile(apkFile)构建出一个Uri,现在我们使用
FileProvider.getUriForFile(context, BuildConfig.APPLICATION_ID + ".fileProvider", apkFile);
- 
BuildConfig.APPLICATION_ID直接是应用的包名
```
Intent intent = new Intent(Intent.ACTION_VIEW);
if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
   /* Android N 写法*/
   intent.setFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
   Uri contentUri = FileProvider.getUriForFile(MainActivity.this, BuildConfig.APPLICATION_ID + ".fileProvider", new File("apk地址"));
   intent.setDataAndType(contentUri, "application/vnd.android.package-archive");
} else {
   /* Android N之前的老版本写法*/
   intent.setDataAndType(Uri.fromFile(new File("apk地址")), "application/vnd.android.package-archive");
   intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
}
startActivity(intent);
```
## 关注
如果有问题，请在下方评论，或者加群讨论 `200909980`
关注下方微信公众号，可以及时获取到各种技术的干货哦，如果你有想推荐的帖子，也可以联系我们的。
![7704655-0d81c89c18cc0c54](https://upload-images.jianshu.io/upload_images/7704655-0d81c89c18cc0c54)
这里写图片描述
