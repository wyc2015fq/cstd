# Intent 在Activity中的简单使用笔记梳理 - weixin_33985507的博客 - CSDN博客
2017年07月07日 11:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
Intent在Android中起到至关重要的作用，在启动下一个Activity，发送广播，发送通知等方面，承担着传递数据的作用，是不可或缺的。
其中，Intent可以分为显示Intent与隐式Intent；又可以分为带返回值的Intent和不带返回值的Intent。
1.不带返回值的Intent的使用：
Intent intent = new Intent(activity.this,nextActivity.class);
这里的第一个参数是当前的activity，第二个参数是要执行到的下一个activity；
intent传递参数：
intent.putExtra(key,value);这里的key为了防止命名重复，可以采用包名+变量名的命名方式。
在这之后启动activity，startActivity(intent);
而在下一个activity接收传递的值，则只需要在onCreate()方法中用getIntent()的对应方法去获取。比如获取一个Boolean类型的变量：
Boolean value = getIntent().getBooleanExtra(key);
2.带返回值的Intent使用
activity中使用带返回值的Intent,第一步的定义是一样的，但是在启动activity时，则使用另一个方法：startActivityForResult(intent,requestCode);其中参数requestCode将在你接收返回值时用到.在接收返回值时，可能当前的activity有多个activity有返回值，而根据你的requestCode就可以判断是哪个activity返回的值，从而做出相应的处理。
而在下一个activity中，接收传递的值是一样的；返回值则需要调用相应的方法。
setResult(int resultCode,Intent data);其中resultCode表示返回的result值，判断是否返回成功，一般写Activity.OK，而data则是Intent返回类型，这里封装了要返回的值。
在父Activity中接收返回的值，则需要重写onActivityResult(int requestCode,int resultCode,Intent data){}方法；
这里的requestCode则是刚才自己发送时候的requestCode,这里要用到了；resultCode则是刚才子Activity中返回的值，也要用到了；data则是返回的数据。
![4360051-22e3fd07d61304a9.png](https://upload-images.jianshu.io/upload_images/4360051-22e3fd07d61304a9.png)
3.隐式的Intent
Activity可以在AndroidManifest.xml声明activity时，在<intent-filter>下配置action与category，这样的话在使用Intent的时候，可以通过:Intent intent = new Intent("actionName");
intent.addCategory("category");的方式来实现隐式的启用。
此外，还有一些系统的隐式Intent：
如：浏览器打开网页：
Intent intent = new Intent(Intent.ACTION_VIEW);
intent.setData(Uri.parse("网址"));
调用拨号界面：
Intent intent = new Intent(Intent.ACTION_DIAL);
intent.setData(Uri.parse("te;:10086"));
注：这是调用拨号界面，而不是直接拨打电话。在android升级到6.0之后，出现了运行时调用权限，而拨打电话则就是这样的权限。此外，拨号界面也是需要申请权限的。
发送消息：
Intent intent = new Intent(Intent.ACTION_SEND);
intent.putExtra();
intent = Intent.CreateChooser(i,getString(R.String));//获取所有的activity
获取联系人：
一个有返回值的Intent使用:
![4360051-1a1037c4bec2df27.png](https://upload-images.jianshu.io/upload_images/4360051-1a1037c4bec2df27.png)
![4360051-8e298e6f28d55081.png](https://upload-images.jianshu.io/upload_images/4360051-8e298e6f28d55081.png)
